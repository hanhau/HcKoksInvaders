#include "include/StarBackground.hpp"
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
	//glDeleteBuffers(1, &gl_vbo);
	//glDeleteVertexArrays(1, &gl_vao);
}

void StarBackground::draw(const Program& program,float pastTime) const {
	glDepthMask(GL_FALSE);
	
	program.bind();
	program.setUniform("progress",pastTime*10);
	program.setUniform("baseColor", sf::Vector3f(
		abs(cos(pastTime*0.3)),
		abs(cos(pastTime*0.3+2)),
		abs(cos(pastTime*0.3+4))
	));

	glBindBuffer(GL_ARRAY_BUFFER, gl_vbo);
	glBindVertexArray(gl_vao);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);

	glUseProgram(0);

	glDepthMask(GL_TRUE);
}