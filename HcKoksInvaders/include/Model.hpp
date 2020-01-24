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
#include "Texture.hpp"

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

	std::shared_ptr<Texture&> m_texDiffuse;
	std::shared_ptr<Texture&> m_texGlossy;
	std::shared_ptr<Texture&> m_texNormal;
	std::shared_ptr<Texture&> m_texEmit;

	GLuint gl_vao;  // VertexArrayObject
	GLuint gl_vbo;  // VertexBufferObject
	GLuint gl_ebo;  // ElementBufferObject
};

class Model3D {
private:
	Mesh m_mesh;
	const Program& m_progRef;

	BoundingBall m_outerBall;
public:
	Model3D(const Program& m_progRef);

	// Loading
	bool loadFileFromMemory(uint8_t* const buffer, const size_t bufferLength, 
						    const std::string fileName, 
							const TextureManager& texMgr);

	// Upload to OpenGL
	void uploadToGl();
	void cleanFromGl();

	// Bounding Balls
	const BoundingBall& getOuterBB() const;

	// Instanced Rendering
	void drawInstanceQueue(InstanceBuffer& instances,const Camera& cam,Cubemap& cubemap) const;
};