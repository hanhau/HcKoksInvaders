#include "include/Model.hpp"
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <iostream>
#include <filesystem>
#include <GL/glew.h>
#include "include/Util.hpp"

/* -------------------------------------------------------------------
                               MODEL
------------------------------------------------------------------- */
void processAiMesh(aiMesh* ai_mesh, const aiScene* scene,Mesh &mesh);

Model3D::Model3D() {
	m_instances.fill(glm::identity<glm::mat4>());
}

Model3D::~Model3D() {
	
}

bool Model3D::loadFromFile(const std::string path, const TextureManager& texMgr)
{
	try {
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

		if (scene == nullptr)
			throw "Error opening Assimp Scene";

		if (scene->mNumMeshes != 1)
			throw "Invalid number of meshes. EXACTLY 1 please.";

		// Load Vertices
		processAiMesh(scene->mMeshes[0],scene,m_mesh);
		importer.FreeScene();

		// Load Textures (if availiable)
		std::filesystem::path filepath(path);
		std::string filename = filepath.filename().string();
		filename = filename.substr(0, filename.find('.'));
		std::string folder = filepath.remove_filename().string();

		const std::string rel_fn = folder + filename;

		if (texMgr.exists(rel_fn + "_diffuse.png")) {
			m_mesh.m_texDiffuse.loadFromFile(rel_fn + "_diffuse.png");
		}
		if (texMgr.exists(rel_fn + "_glossy.png")) {
			m_mesh.m_texGlossy.loadFromFile(rel_fn + "_glossy.png");
		}
		if (texMgr.exists(rel_fn + "_normal.png")) {
			m_mesh.m_texNormal.loadFromFile(rel_fn + "_normal.png");
		}
		if (texMgr.exists(rel_fn + "_emit.png")) {
			m_mesh.m_texNormal.loadFromFile(rel_fn + "_emit.png");
		}

		// Setup OpenGL Buffers
		glBindVertexArray(0);

		glGenBuffers(1, &m_mesh.gl_ssbo);
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_mesh.gl_ssbo);
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, m_mesh.gl_ssbo);
		glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(glm::mat4) * m_maxInstances, (void*)0, GL_STATIC_DRAW);
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

		glGenVertexArrays(1, &m_mesh.gl_vao);
		glGenBuffers(1, &m_mesh.gl_vbo);
		glGenBuffers(1, &m_mesh.gl_ebo);

		glBindVertexArray(m_mesh.gl_vao);

		glBindBuffer(GL_ARRAY_BUFFER, m_mesh.gl_vbo);
		glBufferData(GL_ARRAY_BUFFER, m_mesh.m_vertices.size() * sizeof(Vertex), m_mesh.m_vertices.data(), GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_mesh.gl_ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_mesh.m_indices.size() * sizeof(unsigned int), m_mesh.m_indices.data(), GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex,m_normal));

		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, m_uv));

		glBindVertexArray(0);

		glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);

		// Clean Mesh Data from RAM
		m_mesh.m_numIndices = m_mesh.m_indices.size();
		m_mesh.m_indices.clear();
		m_mesh.m_numVertices = m_mesh.m_vertices.size();
		m_mesh.m_vertices.clear();

		util::checkGlCalls(__FUNCSIG__);

		return true;
	}
	catch (std::string e) {
		std::cout << "Exception: " << e << std::endl;
		return false;
	}
}

void bindTextures(
	unsigned int diffuse,
	unsigned int glossy,
	unsigned int normal,
	unsigned int emit,
	unsigned int cubemap) 
{
	if (diffuse != 0) {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, diffuse);
	}

	if (glossy != 0) {
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, glossy);
	}

	if (normal != 0) {
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, normal);
	}

	if (emit != 0) {
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, emit);
	}

	if (cubemap != 0) {
		glActiveTexture(GL_TEXTURE4);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap);
	}

	glActiveTexture(GL_TEXTURE0);

	util::checkGlCalls(__FUNCSIG__);
}

void unbindTextures() {
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 0);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, 0);
	
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, 0);
	
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, 0);
	
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	
	glActiveTexture(GL_TEXTURE0);

	util::checkGlCalls(__FUNCSIG__);
}

void processAiMesh(aiMesh* ai_mesh, const aiScene* scene, Mesh& mesh) {
	mesh.m_vertices.clear();
	mesh.m_indices.clear();
	
	mesh.m_vertices.reserve(ai_mesh->mNumVertices);

	for (unsigned int i = 0; i < ai_mesh->mNumVertices; i++) {
		Vertex vertex;

		const aiVector3D* const pos = &ai_mesh->mVertices[i];
		vertex.m_position = sf::Vector3f(pos->x, pos->y, pos->z);

		const aiVector3D* const normal = &ai_mesh->mNormals[i];
		vertex.m_normal = sf::Vector3f(normal->x,normal->y,normal->z);

		if (ai_mesh->mTextureCoords[0]) {
			sf::Vector2f uv;
			uv.x = ai_mesh->mTextureCoords[0][i].x;
			uv.y = ai_mesh->mTextureCoords[0][i].y;
			vertex.m_uv = uv;
		}

		mesh.m_vertices.push_back(vertex);
	}
	
	for (unsigned int i = 0; i < ai_mesh->mNumFaces; i++)
	{
		aiFace face = ai_mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++) {
			mesh.m_indices.push_back(face.mIndices[j]);
		}
	}
}

void Model3D::drawInstanceQueue(std::vector<ModelPosition>& pos,Program& const program,Cubemap &cubemap) {
	program.bind();

	glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 10.0f);
	//glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
	glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

	glm::mat4x4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
	glm::mat4x4 perspective = glm::perspective(.3f, (float)640 / (float)960, 1.f, 500.f);
	//glm::mat4x4 perspective = glm::perspective(95.f, (float)640 / (float)960, 0.1f, 100.f);

	program.setUniform("matProjection", perspective);
	program.setUniform("matView", view);

	const unsigned int pos_count = pos.size();
	if (pos_count > m_maxInstances)
		throw "FUCK IT";

	for (unsigned int i = 0; i < pos_count; i++)
		m_instances[i] = pos[i].getTransformationMatrix();	

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_mesh.gl_ssbo);
	glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, sizeof(glm::mat4) * pos_count, &m_instances[0]);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, m_mesh.gl_ssbo);

	bindTextures(
		m_mesh.m_texDiffuse.getNativeHandle(),
		m_mesh.m_texGlossy.getNativeHandle(),
		m_mesh.m_texNormal.getNativeHandle(),
		m_mesh.m_texEmit.getNativeHandle(),
		cubemap.getGlID()
	);

	glBindVertexArray(m_mesh.gl_vao);

	glDrawElementsInstanced(
		GL_TRIANGLES, 
		m_mesh.m_numIndices, 
		GL_UNSIGNED_INT,
		0, 
		pos_count
	);
	glBindVertexArray(0);

	unbindTextures();

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

	glUseProgram(0);

	util::checkGlCalls(__FUNCSIG__);
}