#include "include/Button.hpp"
#include <GL/glew.h>
#include <Util.hpp>

struct ButtonVertex {
	sf::Vector3f pos;
	float col[4];

	inline ButtonVertex(sf::Vector3f pos, const float* col) {
		this->pos = pos;
		memcpy(this->col, col, 4 * sizeof(float));
	}
};

std::vector<ButtonVertex> createButtonMesh(sf::Vector2f size);

Button::Button(std::string title, sf::Vector2f pos, sf::Vector2f size) :
	m_title(title), m_position(pos), m_size(size) 
{
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

	glBindVertexArray(0);

	util::checkGlCalls(__FUNCSIG__);
}

void Button::draw(sf::RenderWindow& rw,sf::Shader * shader) const
{
	rw.setActive();
	sf::Shader::bind(shader);

	auto mat = glm::translate(glm::identity<glm::mat4x4>(), glm::vec3(-1.0, -1.0, 0.0));
	util::setUniformMat(shader, "translate", mat);

	auto pos = glm::translate(glm::identity<glm::mat4x4>(), glm::vec3(m_position.x*2.0, m_position.y*2.0, 0.0));
	util::setUniformMat(shader, "position", pos);

	auto scale = glm::scale(glm::identity<glm::mat4x4>(), glm::vec3(2.0, 2.0, 1.0));
	util::setUniformMat(shader, "scale", scale);

	glDepthFunc(GL_ALWAYS);
	glBindVertexArray(gl_vao);
	glDrawArrays(GL_TRIANGLES, 0, 14*3);
	glBindVertexArray(0);
	glDepthFunc(GL_GREATER);

	sf::Shader::bind(nullptr);
}

bool Button::containsPoint(const sf::Vector2f& p) const
{
	return (p.x > m_position.x && p.x < m_position.x + m_size.x) &&
		   (p.y > m_position.y && p.y < m_position.y + m_size.y);
}

std::vector<ButtonVertex> createButtonMesh(sf::Vector2f size) 
{
	// border-radius
	float bor = size.y * 0.10f;
	
	static const std::array<float, 4> gray = { 0.5,0.5,0.5,1.0 };
	static const std::array<float, 4> outline = {247.f/255.f,108.f/255.f,205.f/255.f,0.0f};

	std::vector<ButtonVertex> v = {
		// top left triangle
		ButtonVertex(sf::Vector3f(bor,bor,0.0),gray.data()), // 0
		ButtonVertex(sf::Vector3f(0.0,bor,0.0),outline.data()), // 1
		ButtonVertex(sf::Vector3f(bor,0.0,0.0),outline.data()), // 2

		// top right triangle
		ButtonVertex(sf::Vector3f(size.x-bor,bor,0.0),gray.data()), // 3
		ButtonVertex(sf::Vector3f(size.x-bor,0.0,0.0),outline.data()), // 4
		ButtonVertex(sf::Vector3f(size.x,bor,0.0),outline.data()), // 5

		// bottom right triangle
		ButtonVertex(sf::Vector3f(size.x-bor,size.y-bor,0.0),gray.data()), // 6
		ButtonVertex(sf::Vector3f(size.x,size.y-bor,0.0),outline.data()), // 7
		ButtonVertex(sf::Vector3f(size.x-bor,size.y,0.0),outline.data()), // 8

		// bottom left triangle
		ButtonVertex(sf::Vector3f(bor,size.y-bor,0.0),gray.data()), // 9
		ButtonVertex(sf::Vector3f(bor,size.y,0.0),outline.data()), // 10
		ButtonVertex(sf::Vector3f(0.0,size.y-bor,0.0),outline.data()) // 11
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