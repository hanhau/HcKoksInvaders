#include "../include/AmmunitionIcon.hpp"
#include "../include/Text.hpp"
#include <GL/glew.h>
#include <SFML/System/Clock.hpp>

struct ButtonVertex {
	sf::Vector2f pos;
	sf::Vector2f uv;
	float colAmount;
};

void makeVertices(std::vector<ButtonVertex>& vertices,
				  const float radius,
				  int& minDotsAmount,int& maxDotsAmount,
			      glm::vec2 pixelSize) 
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
					cos(glm::radians(i))*radius*pixelSize.x,
					sin(glm::radians(i))*radius*pixelSize.y
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
		const float pad_x = 4.f * pixelSize.x;
		const float pad_y = 4.f * pixelSize.y;
		const float rNdc_x = radius * pixelSize.x;
		const float rNdc_y = radius * pixelSize.y;

		vertices.push_back(ButtonVertex{
			sf::Vector2f(-rNdc_x+pad_x,rNdc_y -pad_y),
			sf::Vector2f(0.f,1.f),
			0.f
		});
		vertices.push_back(ButtonVertex{
			sf::Vector2f(rNdc_x -pad_x,rNdc_y -pad_y),
			sf::Vector2f(1.f,1.f),
			0.f
		});
		vertices.push_back(ButtonVertex{
			sf::Vector2f(-rNdc_x +pad_x,-rNdc_y +pad_y),
			sf::Vector2f(0.f,0.f),
			0.f
		});

		vertices.push_back(ButtonVertex{
			sf::Vector2f(rNdc_x -pad_x,rNdc_y -pad_y),
			sf::Vector2f(1.f,1.f),
			0.f
		});
		vertices.push_back(ButtonVertex{
			sf::Vector2f(rNdc_x -pad_x,-rNdc_y +pad_y),
			sf::Vector2f(1.f,0.f),
			0.f
		});
		vertices.push_back(ButtonVertex{
			sf::Vector2f(-rNdc_x +pad_x,-rNdc_y+pad_y),
			sf::Vector2f(0.f,0.f),
			0.f
		});
	}

	// Flip UVs and apply Aspect Ratio
	for (auto& iter : vertices) {
		iter.uv.x = 1.0f - iter.uv.x;
		iter.uv.y = 1.0f - iter.uv.y;
	}
}

AmmunitionIcon::AmmunitionIcon(const std::string pathIcon, 
							   glm::vec4 outlineColor, 
							   const int radius, glm::ivec2 pos,
							   const sf::Window& window) :
	m_col(outlineColor),
	m_radius(radius),
	m_position(pos),
	m_minDotsAmount(0),
	m_maxDotsAmount(0)
{	
	const glm::vec2 pixelSize = glm::vec2(
		2.f/(float)window.getSize().x,
		2.f/(float)window.getSize().y
	);

	if (!TextureManager.exists(pathIcon))
		throw "Texture not found: " + std::string(__FUNCTION__);
	gl_textureID = TextureManager.get(pathIcon).getGlID();

	std::vector<ButtonVertex> vertices;
	makeVertices(vertices,
				 radius,
				 m_minDotsAmount,
				 m_maxDotsAmount,
				 pixelSize);

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

void AmmunitionIcon::draw(const sf::Window& win, 
						  float percentageFull, 
						  const std::string slotTextStr,
						  bool active,
						  float reloadProgress)
{
	static Text slotText = Text("", 18, {0,0});
	static sf::Clock clock;

	const Program& program = ProgramManager.get(ProgramEntry::AmmunitionIcon);
	const Program& programText = ProgramManager.get(ProgramEntry::Text);

	percentageFull = std::clamp(percentageFull, 0.f, 100.f);

	glDepthMask(GL_FALSE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	const glm::vec2 renderPos = glm::vec2(
		-1.f + (m_radius + m_position.x) * (2.f/(float)win.getSize().x),
		+1.f - (m_radius + m_position.y) * (2.f/(float)win.getSize().y)
	);

	program.bind();
	program.setUniform("fColor", sf::Vector3f(m_col.r, m_col.g, m_col.b));
	program.setUniform("uPos", sf::Vector2f(renderPos.x,renderPos.y));
	program.setUniform("fTexture0", 0);
	
	program.setUniform("fReloadProgress",reloadProgress);

	if (percentageFull == 0.0f)
		program.setUniform("fGrayImage",1);
	else
		program.setUniform("fGrayImage",0);

	//glActiveTexture(0);
	glBindTexture(GL_TEXTURE_2D, gl_textureID);

	glBindVertexArray(gl_vao);

	// if circle is visible
	if (percentageFull >= 1.0) {
		glDrawArrays(GL_TRIANGLE_FAN, 0, m_minDotsAmount + std::round((m_maxDotsAmount-m_minDotsAmount)*percentageFull/100.f));
	}

	glDrawArrays(GL_TRIANGLES, m_maxDotsAmount, 6);

	glBindVertexArray(0);

	glBindTexture(GL_TEXTURE_2D, 0);

	glDepthMask(GL_TRUE);
	glDisable(GL_BLEND);

	// Draw SlotText
	slotText.setText(slotTextStr);
	const glm::ivec2 textCenterPos = glm::ivec2(
		m_position.x + m_radius - slotText.getSizePixels(win).x/2,
		m_position.y + m_radius*2 + 25
	);
	slotText.setPos(textCenterPos);

	glm::vec3 col = glm::vec3(1.0f);
	if (active) {
		const float secs = clock.getElapsedTime().asSeconds();
		col = glm::vec3(
			abs(cosf(secs * 2.0f)),
			abs(cosf(secs * 2.0f + glm::radians(120.f))),
			abs(cosf(secs * 2.0f + glm::radians(240.f)))
		);
	}

	slotText.draw(win, programText, col);
}