#include "../include/Model.hpp"
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/material.h>
#include <iostream>
#include <filesystem>
#include <GL/glew.h>
#include "../include/Util.hpp"
#include <Camera.hpp>
#include "../include/Log.hpp"

/* -------------------------------------------------------------------
							   MESH
------------------------------------------------------------------- */

Mesh::Mesh() :
	m_vertices(), m_indices(),
	m_numIndices(0), m_numVertices(0),
	m_texDiffuse(nullptr),
	m_texGlossy(nullptr),
	m_texNormal(nullptr),
	m_texEmit(nullptr),
	gl_vao(0),gl_vbo(0),gl_ebo(0)
{}

/* -------------------------------------------------------------------
                               MODEL
------------------------------------------------------------------- */
void processAiMesh(aiMesh* ai_mesh, const aiScene* scene,Mesh &mesh);

Model3D::Model3D() {}

const BoundingBall& Model3D::getOuterBB() const {
	return m_outerBall;
}

static std::string debugMaterialString(aiMaterial* mat,const char* pKey) {
	aiString str;
	int i;
	aiColor4D col;
	float f;

	if (pKey == "AI_MATKEY_NAME") {
		aiGetMaterialString(mat, pKey, 0, 0, &str);
		return str.C_Str();
	}
	else if (pKey == "AI_MATKEY_COLOR_DIFFUSE" || pKey == "AI_MATKEY_COLOR_SPECULAR" ||
		     pKey == "AI_MATKEY_COLOR_AMBIENT" || pKey == "AI_MATKEY_COLOR_EMISSIVE" ||
		     pKey == "AI_MATKEY_TRANSPARENT") {
		aiGetMaterialColor(mat, pKey, 0, 0, &col);
		return "(" + 
			std::string(std::to_string(col.a)) + "," + 
			std::string(std::to_string(col.g)) + "," + 
			std::string(std::to_string(col.b)) + "," +
			std::string(std::to_string(col.a)) +
		")";
	}
	else if (pKey == "AI_MATKEY_WIREFRAME" || pKey == "AI_MATKEY_TWOSIDED" || 
		     pKey == "AI_MATKEY_SHADING_MODEL" || pKey == "AI_MATKEY_BLEND_FUNC") {
		aiGetMaterialInteger(mat, pKey, 0, 0, &i);
		return std::to_string(i);
	}
	else if (pKey == "AI_MATKEY_OPACITY" || pKey == "AI_MATKEY_SHININESS" ||
		     pKey == "AI_MATKEY_SHININESS_STRENGTH" || pKey == "AI_MATKEY_REFRACTI") {
		aiGetMaterialFloat(mat, pKey, 0, 0, &f);
		return std::to_string(f);
	}
	else return "";
}

bool Model3D::loadFileFromMemory(uint8_t * const buffer, const size_t bufferLength, 
								 const std::string filename)
{
	try {
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFileFromMemory(buffer, bufferLength, aiProcess_Triangulate | aiProcess_FlipUVs);

		if (scene == nullptr)
			throw std::string("Error opening Assimp Scene");

		if (scene->mNumMeshes != 1)
			throw std::string("Invalid number of meshes. EXACTLY 1 please.");

		// Debug Out Materials
		for (int i = 0; i < scene->mNumMaterials; i++) {
			printf_s("Mesh %s: \n" 
				"NAME: %s\n"
				"COLOR_DIFFUSE: %s\n"
				"COLOR_SPECULAR: %s\n"
				"COLOR_AMBIENT: %s\n"
				"COLOR_EMISSIVE: %s\n"
				"COLOR_TRANSPARENT: %s\n"
				"WIREFRAME: %s\n"
				"TWOSIDED: %s\n"
				"SHADING_MODEL: %s\n"
				"BLEND_FUNC: %s\n"
				"OPACITY: %s\n"
				"SHININESS: %s\n"
				"SHININESS_STRENGTH: %s\n"
				"REFRACTI: %s\n"
				"\n",
				filename.c_str(),
				debugMaterialString(scene->mMaterials[i],"AI_MATKEY_NAME").c_str(),
				debugMaterialString(scene->mMaterials[i],"AI_MATKEY_COLOR_DIFFUSE").c_str(),
				debugMaterialString(scene->mMaterials[i],"AI_MATKEY_COLOR_SPECULAR").c_str(),
				debugMaterialString(scene->mMaterials[i],"AI_MATKEY_COLOR_AMBIENT").c_str(),
				debugMaterialString(scene->mMaterials[i],"AI_MATKEY_COLOR_EMISSIVE").c_str(),
				debugMaterialString(scene->mMaterials[i],"AI_MATKEY_COLOR_TRANSPARENT").c_str(),
				debugMaterialString(scene->mMaterials[i],"AI_MATKEY_WIREFRAME").c_str(),
				debugMaterialString(scene->mMaterials[i],"AI_MATKEY_TWOSIDED").c_str(),
				debugMaterialString(scene->mMaterials[i],"AI_MATKEY_SHADING_MODEL").c_str(),
				debugMaterialString(scene->mMaterials[i],"AI_MATKEY_BLEND_FUNC").c_str(),
				debugMaterialString(scene->mMaterials[i],"AI_MATKEY_OPACITY").c_str(),
				debugMaterialString(scene->mMaterials[i],"AI_MATKEY_SHININESS").c_str(),
				debugMaterialString(scene->mMaterials[i],"AI_MATKEY_SHININESS_STRENGTH").c_str(),
				debugMaterialString(scene->mMaterials[i],"AI_MATKEY_REFRACTI").c_str()
			);
		}

		// Load Vertices
		processAiMesh(scene->mMeshes[0],scene,m_mesh);
		importer.FreeScene();

		// Generate BoundingBalls
		float max_rad = 0.0f;
		for (const auto& iter : m_mesh.m_vertices) {
			glm::vec3 vec3(
				iter.m_position.x,
				iter.m_position.y,
				iter.m_position.z
			);
			
			if (glm::length(vec3) > max_rad)
				max_rad = glm::length(vec3);
		}
		m_outerBall = std::move(BoundingBall(glm::vec3(), max_rad));

		// Load Textures (if availiable)
		const std::string rel_fn = filename.substr(0,filename.find_last_of('.'));

		if (TextureManager.exists(rel_fn + "_diffuse.png")) {
			m_mesh.m_texDiffuse = &TextureManager.get(rel_fn + "_diffuse.png");
		}
		if (TextureManager.exists(rel_fn + "_glossy.png")) {
			m_mesh.m_texGlossy = &TextureManager.get(rel_fn + "_glossy.png");
		}
		if (TextureManager.exists(rel_fn + "_normal.png")) {
			m_mesh.m_texNormal = &TextureManager.get(rel_fn + "_normal.png");
		}
		if (TextureManager.exists(rel_fn + "_emit.png")) {
			m_mesh.m_texEmit = &TextureManager.get(rel_fn + "_emit.png");
		}

		return true;
	}
	catch (std::string e) {
		std::cout << "Exception: " << e << std::endl;
		return false;
	}
}

void Model3D::uploadToGl() {
	// Setup OpenGL Buffers
	glBindVertexArray(0);

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
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, m_normal));

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, m_uv));

	glBindVertexArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	// Clean Mesh Data from RAM
	m_mesh.m_numIndices = m_mesh.m_indices.size();
	m_mesh.m_indices.clear();
	m_mesh.m_numVertices = m_mesh.m_vertices.size();
	m_mesh.m_vertices.clear();

	util::checkGlCalls(__FUNCTION__);
}

void Model3D::cleanFromGl() {
	glDeleteVertexArrays(1, &m_mesh.gl_vao);
	glDeleteBuffers(1, &m_mesh.gl_vbo);
	glDeleteBuffers(1, &m_mesh.gl_ebo);
}

void bindTextures(
	const Texture* diffuse,
	const Texture* glossy,
	const Texture* normal,
	const Texture* emit,
	unsigned int cubemap) 
{
	if (diffuse != nullptr) {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, diffuse->getGlID());
	}

	if (glossy != nullptr) {
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, glossy->getGlID());
	}

	if (normal != nullptr) {
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, normal->getGlID());
	}

	if (emit != nullptr) {
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, emit->getGlID());
	}

	if (cubemap != 0) {
		glActiveTexture(GL_TEXTURE4);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap);
	}

	glActiveTexture(GL_TEXTURE0);

	util::checkGlCalls(__FUNCTION__);
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

	util::checkGlCalls(__FUNCTION__);
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

void Model3D::drawInstanceQueue(InstanceBuffer& instances,const Camera& cam,Cubemap &cubemap) const {
	static const Program& program = ProgramManager.get(ProgramEntry::Model3D);

	glm::mat4x4 view = cam.getViewMatrix();
	glm::mat4x4 perspective = cam.getProjectionMatrix();

	program.bind();
	program.setUniform("matProjection", perspective);
	program.setUniform("matView", view);

	instances.bind(0);

	bindTextures(
		m_mesh.m_texDiffuse,
		m_mesh.m_texGlossy,
		m_mesh.m_texNormal,
		m_mesh.m_texEmit,
		cubemap.getGlID()
	);

	glBindVertexArray(m_mesh.gl_vao);

	glDrawElementsInstanced(
		GL_TRIANGLES,
		m_mesh.m_numIndices,
		GL_UNSIGNED_INT,
		0,
		instances.getInnerCount()
	);
	glBindVertexArray(0);

	unbindTextures();

	instances.unbind();

	program.unbind();

	util::checkGlCalls(__FUNCTION__);
}