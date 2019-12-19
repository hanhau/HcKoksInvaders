#pragma once
#include <string>
#include <SFML/Graphics.hpp>
#include <glm/glm.hpp>
#include <vector>
#include <GL/glew.h>
#include "Cubemap.hpp"
#include "ModelPosition.hpp"
#include "Program.hpp"
#include <array>

class Vertex {
public:
	sf::Vector3f m_position;
	sf::Vector3f m_normal;
	sf::Vector2f m_uv;
};

struct Mesh {
	// only used for loading
	std::vector<Vertex> m_vertices;
	std::vector<unsigned int> m_indices;

	long m_numIndices;
	long m_numVertices;

	sf::Texture m_texDiffuse;
	sf::Texture m_texGlossy;
	sf::Texture m_texNormal;
	sf::Texture m_texEmit;

	GLuint gl_vao;  // VertexArrayObject
	GLuint gl_vbo;  // VertexBufferObject
	GLuint gl_ebo;  // ElementBufferObject
	GLuint gl_ssbo; // ShaderStorageBufferObject for Instances
};

class Model3D {
private:
	Mesh m_mesh;

	static const size_t m_maxInstances = 256;
	std::array<glm::mat4, m_maxInstances> m_instances;
public:
	Model3D();
	~Model3D();

	bool loadFromFile(const std::string path);

	// Instanced Rendering
	void drawInstanceQueue(std::vector<ModelPosition>& pos,Program& const program,Cubemap& cubemap);
};