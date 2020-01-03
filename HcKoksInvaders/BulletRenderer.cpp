#include "include/BulletRenderer.hpp"
#include <GL/glew.h>
#include <glm/ext/matrix_transform.hpp>

struct BulletPosData {
	glm::mat4 translate;
	glm::vec4 color;
};

std::vector<glm::vec3> getBulletVertices() {
	constexpr float x_half = 2.5f * 1.f/320.f;
	constexpr float y_half = 4.0f * 1.f/320.f;

	return std::vector<glm::vec3>{
		glm::vec3(-x_half, y_half,0.0f),
		glm::vec3( x_half, y_half,0.0f),
		glm::vec3( x_half,-y_half,0.0f),
		glm::vec3(-x_half,-y_half,0.0f)
	};
}

constexpr unsigned int MAX_BULLETS = 1024;

BulletRenderer::BulletRenderer(const ProgramManager& progMgr) :
	m_progMgr(progMgr)
{
	const std::vector<glm::vec3> vertices(getBulletVertices());
	
	glGenVertexArrays(1, &gl_vao);

	glBindVertexArray(gl_vao);

	glCreateBuffers(1, &gl_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, gl_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * vertices.size(),vertices.data(),GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);

	glBindVertexArray(0);

	glCreateBuffers(1, &gl_ssbo);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, gl_ssbo);
	glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(BulletPosData) * MAX_BULLETS, (void*)0, GL_DYNAMIC_DRAW);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, gl_ssbo);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}

void BulletRenderer::drawInstances(std::list<Bullet>& bullets, const Camera& cam) {
	static const Program& prog = m_progMgr.get(ProgramManager::ProgramEntry::Bullet);
	static std::vector<BulletPosData> bulletPos(MAX_BULLETS);

	const unsigned int bulletCount = bullets.size();

	assert(bulletCount <= MAX_BULLETS);

	unsigned int i = 0;
	for (auto& iter : bullets) {
		bulletPos[i].translate = std::move(glm::translate(
			glm::identity<glm::mat4>(),
			iter.m_pos
		));

		if(iter.m_owner == Bullet::Owner::Enemy)
			bulletPos[i].color = std::move(glm::vec4(cosf(i), sinf(i), sinf(i * cosf(i)), 1.0));
		if(iter.m_owner == Bullet::Owner::Player)
			bulletPos[i].color = std::move(glm::vec4(0.0f,1.0f,0.5f, 1.0));

		i++;
	}

	glDepthMask(GL_FALSE);
	
	// Setup Program
	prog.bind();
	prog.setUniform("view", cam.getViewMatrix());
	prog.setUniform("proj", cam.getProjectionMatrix());

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, gl_ssbo);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, gl_ssbo);
	glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, sizeof(BulletPosData) * bulletCount, bulletPos.data());

	glBindVertexArray(gl_vao);

	glDrawArraysInstanced(GL_QUADS, 0, 4, bulletCount);

	glBindVertexArray(0);

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

	prog.unbind();
	glDepthMask(GL_TRUE);
}