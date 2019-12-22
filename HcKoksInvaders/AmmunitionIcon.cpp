#include "include/AmmunitionIcon.hpp"
#include <GL/glew.h>

struct ButtonVertex {
	sf::Vector2f pos;
	sf::Vector2f uv;
	float colAmount;
};

void makeVertices(std::vector<ButtonVertex>& vertices,
				  const float radius,
				  int& minDotsAmount,int& maxDotsAmount,
			      const float aspectRatio) 
{
	vertices.clear();

	// Create circle
	{
		// center dot
		vertices.push_back(ButtonVertex{
			sf::Vector2f(),
			sf::Vector2f(),
			1.0f
		});

		for (float i = 90.f; i <= 360.f+90.f; i+= 360.f/100.f) {
			vertices.push_back(ButtonVertex{
				sf::Vector2f(
					cos(glm::radians(i))*radius,
					sin(glm::radians(i))*radius
				),
				sf::Vector2f(0.f,0.f),
				1.0f
			});
		}

		minDotsAmount = 3;
		maxDotsAmount = 101;
	}

	// Create icon mesh
	{
		// uv zero = bottom left
		// 0.0 to 1.0
		const float pad = 0.025;

		vertices.push_back(ButtonVertex{
			sf::Vector2f(-radius+pad,radius-pad),
			sf::Vector2f(0.f,1.f),
			0.f
		});
		vertices.push_back(ButtonVertex{
			sf::Vector2f(radius-pad,radius-pad),
			sf::Vector2f(1.f,1.f),
			0.f
		});
		vertices.push_back(ButtonVertex{
			sf::Vector2f(-radius+pad,-radius+pad),
			sf::Vector2f(0.f,0.f),
			0.f
		});

		vertices.push_back(ButtonVertex{
			sf::Vector2f(radius-pad,radius-pad),
			sf::Vector2f(1.f,1.f),
			0.f
		});
		vertices.push_back(ButtonVertex{
			sf::Vector2f(radius-pad,-radius+pad),
			sf::Vector2f(1.f,0.f),
			0.f
		});
		vertices.push_back(ButtonVertex{
			sf::Vector2f(-radius+pad,-radius+pad),
			sf::Vector2f(0.f,0.f),
			0.f
		});
	}

	// Flip UVs and apply Aspect Ratio
	for (auto& iter : vertices) {
		iter.uv.x = 1.0 - iter.uv.x;
		iter.uv.y = 1.0 - iter.uv.y;
		iter.pos.y *= aspectRatio;
	}
}

AmmunitionIcon::AmmunitionIcon(const std::string pathIcon, 
							   sf::Color outlineColor, 
							   const float radius, sf::Vector2f pos,
							   TextureManager& texMgr, sf::Window& const window) :
	m_col(outlineColor),
	m_radius(radius),
	m_position(pos),
	m_minDotsAmount(0),
	m_maxDotsAmount(0)
{	
	if (!texMgr.exists(pathIcon))
		throw "Texture not found: " + std::string(__FUNCSIG__);
	gl_textureID = texMgr.get(pathIcon).getNativeHandle();

	std::vector<ButtonVertex> vertices;
	makeVertices(vertices,
				 radius,
				 m_minDotsAmount,
				 m_maxDotsAmount,
				 (float)window.getSize().x/(float)window.getSize().y);

	glGenVertexArrays(1, &gl_vao);
	glGenBuffers(1, &gl_vbo);

	glBindVertexArray(gl_vao);

	glBindBuffer(GL_ARRAY_BUFFER, gl_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(ButtonVertex) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

	// pos vec2
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(ButtonVertex), (void*)0);

	// uv vec2
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(ButtonVertex), (void*)offsetof(ButtonVertex, uv));

	// colAmount float
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(ButtonVertex), (void*)offsetof(ButtonVertex, colAmount));

	glBindVertexArray(0);
}

void AmmunitionIcon::draw(float percentageFull, const Program& program) {
	percentageFull = std::clamp(percentageFull, 0.f, 100.f);

	glDepthMask(GL_FALSE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	program.bind();
	program.setUniform("fColor", sf::Vector3f(m_col.r, m_col.g, m_col.b));
	program.setUniform("uPos", m_position);

	if (percentageFull == 0.0f)
		program.setUniform("fGrayImage",1);
	else
		program.setUniform("fGrayImage",0);

	//glActiveTexture(0);
	glBindTexture(GL_TEXTURE_2D, gl_textureID);

	glBindVertexArray(gl_vao);

	// if circle is visible
	if (percentageFull >= 1.0) {
		glDrawArrays(GL_TRIANGLE_FAN, 0, m_minDotsAmount + (m_maxDotsAmount-m_minDotsAmount)*percentageFull/100.f);
	}

	glDrawArrays(GL_TRIANGLES, m_maxDotsAmount, 6);

	glBindVertexArray(0);

	glBindTexture(GL_TEXTURE_2D, 0);

	glDepthMask(GL_TRUE);
	glDisable(GL_BLEND);
}