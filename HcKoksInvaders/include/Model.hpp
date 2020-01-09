#pragma once
#include <string>
#include <glm/glm.hpp>
#include <vector>
#include <GL/glew.h>
#include "Cubemap.hpp"
#include "ModelPosition.hpp"
#include "ProgramManager.hpp"
#include "Program.hpp"
#include <array>
#include "Camera.hpp"
#include "BoundingBall.hpp"
#include "InstanceBuffer.hpp"

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
	const Program& m_progRef;

	BoundingBall m_outerBall;
public:
	Model3D(const Program& m_progRef);
	~Model3D();

	// Bounding Balls
	const BoundingBall& getOuterBB() const;

	// Loading
	bool loadFromFile(const std::string path,const TextureManager& texMgr);

	// Instanced Rendering
	void drawInstanceQueue(InstanceBuffer& instances,const Camera& cam,Cubemap& cubemap) const;
};