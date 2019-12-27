#include "include/TextRenderer.hpp"
#include <SFML/Graphics/Text.hpp>
#include <GL/glew.h>
#include <iostream>

// static members
sf::RenderTexture* TextRenderer::sf_renderTex;
unsigned int TextRenderer::gl_vao;
unsigned int TextRenderer::gl_vbo;
Program* TextRenderer::m_program;
Shader* TextRenderer::m_vertShader;
Shader* TextRenderer::m_fragShader;
sf::Window* TextRenderer::m_win = nullptr;

void TextRenderer::init(const sf::Window& win) {
	m_win = (sf::Window*)&win;

	// Init Overlay
	sf_renderTex = new sf::RenderTexture();
	sf_renderTex->create(win.getSize().x, win.getSize().y, sf::ContextSettings());
	win.setActive(true);

	// Make Program
	const std::string vert = ""
		"\n#version 430 core"
		"\nlayout (location = 0) in vec2 aPos;"
		"\nlayout (location = 1) in vec2 aUV;"
		"\nout vec2 vUV;"
		"\nvoid main(){"
		"\nvUV = aUV;"
		"\ngl_Position = vec4(aPos,0.0,1.0);"
		"}";

	const std::string frag = ""
		"\n#version 430 core"
		"\nin vec2 vUV;"
		"\nlayout (binding = 0) uniform sampler2D tex;"
		"\nvoid main(){"
		"\ngl_FragColor = texture(tex,vUV);"
		"}";

	m_vertShader = new Shader();
	m_fragShader = new Shader();

	m_vertShader->loadFromString(vert, Shader::ShaderType::Vertex);
	m_fragShader->loadFromString(frag, Shader::ShaderType::Fragment);
	m_program = new Program(*m_vertShader, *m_fragShader);

	// Init Buffer
	const glm::vec2 verts[] = {
		// position, uv,
		glm::vec2(-1.0f, 1.0f), glm::vec2(0.f,1.f),
		glm::vec2( 1.0f, 1.0f), glm::vec2(1.f,1.f),
		glm::vec2( 1.0f,-1.0f), glm::vec2(1.f,0.f),
		glm::vec2(-1.0f,-1.0f), glm::vec2(0.f,0.f)
	};

	// Create Buffers
	glCreateVertexArrays(1, &gl_vao);
	glBindVertexArray(gl_vao);

	glCreateBuffers(1, &gl_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, gl_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * 2 * 4, &verts[0], GL_DYNAMIC_DRAW);

	// pos
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2) * 2, (void*)0);

	// uv
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2) * 2, (void*)sizeof(glm::vec2));

	glBindVertexArray(0);
}

void TextRenderer::drawToOverlay(sf::Text& text) {
	sf_renderTex->draw(text);
}
void TextRenderer::clearOverlay() {
	sf_renderTex->clear(sf::Color(0, 0, 0, 0));
}
void TextRenderer::drawOverlay() {
	glDepthMask(GL_FALSE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	sf_renderTex->display();
	m_win->setActive(true);

	m_program->bind();

	glActiveTexture(0);
	glBindTexture(GL_TEXTURE_2D, sf_renderTex->getTexture().getNativeHandle());
	
	glBindVertexArray(gl_vao);
	glDrawArrays(GL_QUADS, 0, 4);
	glBindVertexArray(0);
	
	glBindTexture(GL_TEXTURE_2D, 0);
	glUseProgram(0);

	glDisable(GL_BLEND);
	glDepthMask(GL_TRUE);
}