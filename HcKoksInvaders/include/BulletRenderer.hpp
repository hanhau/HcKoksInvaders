#pragma once
#include "Bullet.hpp"
#include "ProgramManager.hpp"
#include "Camera.hpp"
#include <list>
#include <memory>

class BulletRenderer
{
	unsigned int gl_vao;
	unsigned int gl_vbo;
	unsigned int gl_ssbo;

	const ProgramManager& m_progMgr;
public:
	BulletRenderer(const ProgramManager& progMgr);

	void drawInstances(std::list<Bullet>& bullets, const Camera& cam);
};