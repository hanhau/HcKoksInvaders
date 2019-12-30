#include "include/Button.hpp"
#include <GL/glew.h>
#include <Util.hpp>

struct ButtonVertex {
	glm::vec3 pos;
	glm::vec4 col;
	glm::vec2 uv;

	inline ButtonVertex(const glm::vec3 pos, const glm::vec4 col) : 
		pos(pos), col(col)
	{}
	inline ButtonVertex(const glm::vec3 pos, const glm::vec4, const glm::vec2 uv) :
		pos(pos), col(col), uv(uv)
	{}
};

std::vector<ButtonVertex> createButtonMesh(glm::vec2 size);

Button::Button(std::string title, glm::vec2 pos, int fontHeight) :
	m_title(title), m_position(pos)
{
	glm::vec2 size = glm::vec2(0.0f,0.0f);

	std::vector<ButtonVertex> vertices = createButtonMesh(size);

	glGenVertexArrays(1, &gl_vao);
	glGenBuffers(1, &gl_vbo);

	glBindVertexArray(gl_vao);

	glBindBuffer(GL_ARRAY_BUFFER, gl_vbo);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(ButtonVertex), vertices.data(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(ButtonVertex), (void*)0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(ButtonVertex), (void*)offsetof(ButtonVertex, col));

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(ButtonVertex), (void*)offsetof(ButtonVertex, uv));

	glBindVertexArray(0);

	util::checkGlCalls(__FUNCSIG__);
}

void Button::draw(const Program& program) const
{
	

	/*auto mat = glm::translate(glm::identity<glm::mat4x4>(), glm::vec3(-1.0, -1.0, 0.0));
	util::setUniformMat(shader, "translate", mat);

	auto pos = glm::translate(glm::identity<glm::mat4x4>(), glm::vec3(m_position.x*2.0, m_position.y*2.0, 0.0));
	util::setUniformMat(shader, "position", pos);

	auto scale = glm::scale(glm::identity<glm::mat4x4>(), glm::vec3(2.0, 2.0, 1.0));
	util::setUniformMat(shader, "scale", scale);*/

	glDepthFunc(GL_ALWAYS);
	glBindVertexArray(gl_vao);
	glDrawArrays(GL_TRIANGLES, 0, 14*3);
	glBindVertexArray(0);
	glDepthFunc(GL_GREATER);

	sf::Shader::bind(nullptr);
}

bool Button::containsPoint(const glm::vec2 point) const
{
	return true;

	/*return (point.x > m_position.x && point.x < m_position.x + m_size.x) &&
		   (point.y > m_position.y && point.y < m_position.y + m_size.y);*/
}

std::vector<ButtonVertex> createButtonMesh(glm::vec2 size) 
{
	// border-radius
	float bor = size.y * 0.10f;
	
	static const glm::vec4 gray = glm::vec4(0.5,0.5,0.5,1.0);
	static const glm::vec4 outline = glm::vec4(247.f/255.f,108.f/255.f,205.f/255.f,0.0f);

	std::vector<ButtonVertex> v = {
		// top left triangle
		ButtonVertex(glm::vec3(bor,bor,0.0),gray,glm::vec2(0,0)), // 0
		ButtonVertex(glm::vec3(0.0,bor,0.0),outline), // 1
		ButtonVertex(glm::vec3(bor,0.0,0.0),outline), // 2

		// top right triangle
		ButtonVertex(glm::vec3(size.x-bor,bor,0.0),gray,glm::vec2(0,0)), // 3
		ButtonVertex(glm::vec3(size.x-bor,0.0,0.0),outline), // 4
		ButtonVertex(glm::vec3(size.x,bor,0.0),outline), // 5

		// bottom right triangle
		ButtonVertex(glm::vec3(size.x-bor,size.y-bor,0.0),gray,glm::vec2(0,0)), // 6
		ButtonVertex(glm::vec3(size.x,size.y-bor,0.0),outline), // 7
		ButtonVertex(glm::vec3(size.x-bor,size.y,0.0),outline), // 8

		// bottom left triangle
		ButtonVertex(glm::vec3(bor,size.y-bor,0.0),gray,glm::vec2(0,0)), // 9
		ButtonVertex(glm::vec3(bor,size.y,0.0),outline), // 10
		ButtonVertex(glm::vec3(0.0,size.y-bor,0.0),outline) // 11
	};

	return {
		// top left corner
		v[0],v[1],v[2],

		// top quad
		v[0],v[2],v[3],
		v[2],v[3],v[4],
		
		// top right corner
		v[3],v[4],v[5],

		// right quad
		v[3],v[5],v[6],
		v[5],v[6],v[7],
		
		// bottom right corner
		v[6],v[7],v[8],
		
		// bottom quad
		v[6],v[8],v[9],
		v[8],v[9],v[10],
		
		// bottom left corner
		v[9],v[10],v[11],
		
		// left quad
		v[0],v[1],v[11],
		v[0],v[9],v[11],

		// middle quad
		v[0],v[3],v[9],
		v[3],v[6],v[9]
	};
}