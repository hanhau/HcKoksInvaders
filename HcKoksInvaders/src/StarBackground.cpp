#include "../include/StarBackground.hpp"
#include <GL/glew.h>

StarBackground::StarBackground() {
	static const sf::Vector3f verts[] = {
		sf::Vector3f(-1.0, 1.0, .0),
		sf::Vector3f( 1.0, 1.0, .0),
		sf::Vector3f(-1.0,-1.0, .0),

		sf::Vector3f( 1.0, 1.0, .0),
		sf::Vector3f( 1.0,-1.0, .0),
		sf::Vector3f(-1.0,-1.0, .0)
	};
	
	glGenVertexArrays(1, &gl_vao);
	glGenBuffers(1, &gl_vbo);

	glBindVertexArray(gl_vao);

	glBindBuffer(GL_ARRAY_BUFFER, gl_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * 6, &verts[0], GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,sizeof(float)*3, (void*)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);
}

StarBackground::~StarBackground() {
	glDeleteBuffers(1, &gl_vbo);
	glDeleteVertexArrays(1, &gl_vao);
}

void StarBackground::draw(const sf::Window& window,const Program& program,float pastTime) const {
	glDepthMask(GL_FALSE);
	
	program.bind();
	program.setUniform("iResolution",sf::Vector2f(
		window.getSize().x,
		window.getSize().y
	));
	program.setUniform("progress",pastTime*10);
	program.setUniform("baseColor", sf::Vector3f(
		abs(cosf(pastTime*0.3f)),
		abs(cosf(pastTime*0.3f+2.f)),
		abs(cosf(pastTime*0.3f+4.f))
	));

	glBindBuffer(GL_ARRAY_BUFFER, gl_vbo);
	glBindVertexArray(gl_vao);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);

	program.unbind();

	glDepthMask(GL_TRUE);
}