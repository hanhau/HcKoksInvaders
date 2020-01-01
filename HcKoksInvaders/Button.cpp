#include "include/Button.hpp"
#include <GL/glew.h>
#include <Util.hpp>
#include <vector>

class ButtonVertex {
public:
	glm::vec3 pos;
	glm::vec4 col;
};

Button::Button(const sf::Window& win,const std::string title, const glm::ivec2 posPixel, int fontHeight) :
	m_title(title), m_position(posPixel), 
	Text(title,fontHeight,glm::ivec2(posPixel.x+5,posPixel.y+5))
{
	glm::vec2 size = this->getSizeNDC(win);
	m_pixelSize = glm::vec2(
		2.0f / (float)win.getSize().x,
		2.0f / (float)win.getSize().y
	);

	const float borx = 5.0f * m_pixelSize.x;
	const float bory = 5.0f * m_pixelSize.y;

	const glm::vec4 gray = glm::vec4(0.5, 0.5, 0.5, 1.0);
	const glm::vec4 outline = glm::vec4(247.f / 255.f, 108.f / 255.f, 205.f / 255.f, 0.0f);

	std::vector<ButtonVertex> v = {
		// top left triangle
		ButtonVertex{glm::vec3(borx,bory,0),gray}, // 0
		ButtonVertex{glm::vec3(0,bory,0),outline}, // 1
		ButtonVertex{glm::vec3(borx,0,0),outline}, // 2

		// top right triangle
		ButtonVertex{glm::vec3(borx + size.x,bory,0),gray}, // 3
		ButtonVertex{glm::vec3(borx + size.x,0,0),outline}, // 4
		ButtonVertex{glm::vec3(borx + size.x + borx,bory,0),outline}, // 5

		// bottom right triangle
		ButtonVertex{glm::vec3(borx + size.x,bory + size.y,0),gray}, // 6
		ButtonVertex{glm::vec3(borx + size.x + borx,bory + size.y,0),outline}, // 7
		ButtonVertex{glm::vec3(borx + size.x,bory + size.y + bory,0),outline}, // 8

		// bottom left triangle
		ButtonVertex{glm::vec3(borx,bory + size.y,0),gray}, // 9
		ButtonVertex{glm::vec3(borx,bory + size.y + bory,0),outline}, // 10
		ButtonVertex{glm::vec3(0,bory + size.y,0),outline} // 11
	};

	for (auto& iter : v)
		iter.pos.y *= -1.0f;

	std::vector<ButtonVertex> vertices = {
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

void Button::draw(const sf::Window& window,const ProgramManager& progMgr)
{
	static sf::Clock clock;

	const Program& textProg = progMgr.get(ProgramManager::ProgramEntry::Text);
	const Program& buttonProg = progMgr.get(ProgramManager::ProgramEntry::Button);

	buttonProg.bind();

	const float tx = -1.0f + (m_position.x * m_pixelSize.x);
	const float ty = 1.0f - (m_position.y * m_pixelSize.y);

	buttonProg.setUniform("translate", glm::translate(glm::identity<glm::mat4>(),glm::vec3(tx,ty,0.0f)));
	buttonProg.setUniform("time", clock.getElapsedTime().asSeconds());

	glDepthMask(GL_FALSE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glBindVertexArray(gl_vao);	
	glDrawArrays(GL_TRIANGLES, 0, 16*3);
	glBindVertexArray(0);

	glDisable(GL_BLEND);
	glDepthMask(GL_TRUE);

	buttonProg.unbind();

	Text::draw(window,textProg);
}

bool Button::containsPoint(const glm::ivec2 pointInPixel) const
{
	return true;

	/*return (point.x > m_position.x && point.x < m_position.x + m_size.x) &&
		   (point.y > m_position.y && point.y < m_position.y + m_size.y);*/
}
