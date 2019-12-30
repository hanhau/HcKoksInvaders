#include "include/Text.hpp"
#include <gl/glew.h>
#include <array>
#include <iostream>
#include "include/Util.hpp"

#include <ft2build.h>
#include FT_FREETYPE_H

struct Char {
	glm::ivec2 size;
	glm::ivec2 bearing;
	int advance;
};

struct CharVertex {
	glm::vec2 pos;
	glm::vec3 uv;
};

class Text::impl {
public:
	std::array<Char, 128> chars = {};
	
	GLuint gl_tex2DArray = 0;
	GLuint gl_vao;
	GLuint gl_vbo;

	int pixelHeight;
	std::string text = "";
	glm::ivec2 pos = glm::ivec2();

	std::vector<CharVertex> createVertices(glm::vec2 pxsize) {
		float x = 0.0f;
		float y = 0.0f;

		std::vector<CharVertex> verts;

		for (const auto& c : text) {
			Char& cRef = chars[c];

			float xpos = x + (cRef.size.x * pxsize.x);
			float ypos = y + ((pixelHeight - cRef.size.y - cRef.bearing.y) * pxsize.y);

			float w = cRef.size.x * pxsize.x;
			float h = cRef.size.y * pxsize.y;

			float w_uv = cRef.size.x / (float)pixelHeight;
			float h_uv = cRef.size.y / (float)pixelHeight;

			// tl
			verts.push_back(CharVertex{
				glm::vec2(xpos,ypos),
				glm::vec3(glm::vec2(0,h_uv),c)
			});
			// tr
			verts.push_back(CharVertex{
				glm::vec2(xpos + w,ypos),
				glm::vec3(glm::vec2(w_uv,h_uv),c)
			});
			// br
			verts.push_back(CharVertex{
				glm::vec2(xpos + w,ypos + h),
				glm::vec3(glm::vec2(w_uv, 0),c)
			});
			// bl
			verts.push_back(CharVertex{
				glm::vec2(xpos,ypos + h),
				glm::vec3(glm::vec2(0, 0),c)
			});

			x += (cRef.advance / 64) * pxsize.x;
		}

		return std::move(verts);
	}
};

constexpr static unsigned int _maxChars = 64 * 4;

Text::Text(const std::string text,int pixelheight, glm::ivec2 posInPixel) 
{
	m_impl = new Text::impl();
	m_impl->pos = posInPixel;
	m_impl->text = text;
	m_impl->pixelHeight = pixelheight;

	util::checkGlCalls("texstart");

	FT_Library ft{};
	FT_Face face{};

	if (FT_Init_FreeType(&ft))
		std::cout << "Error initializing FT @ " << std::string(__FUNCSIG__) << "\n";

	if (FT_New_Face(ft, "res/fonts/CabinSketch-Bold.ttf", 0, &face))
		std::cout << "Error opening Face @ " << std::string(__FUNCSIG__) << "\n";

	if ((face->face_flags & FT_FACE_FLAG_SCALABLE) != FT_FACE_FLAG_SCALABLE) {
		std::cout << "Error: font is not scalable.";
	}
	FT_Set_Pixel_Sizes(face, 0, pixelheight);

	unsigned char* clearBuffer = new unsigned char[pixelheight * pixelheight]{ 0 };
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	glGenTextures(1, &m_impl->gl_tex2DArray);
	glBindTexture(GL_TEXTURE_2D_ARRAY, m_impl->gl_tex2DArray);

	glTexStorage3D(GL_TEXTURE_2D_ARRAY, 1, GL_ALPHA, pixelheight, pixelheight, 128);
	/*glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);*/

	for (unsigned char c = 0; c <= 127; c++) {
		FT_Load_Char(face, c, FT_LOAD_RENDER);

		m_impl->chars[c].size = glm::ivec2(
			face->glyph->bitmap.width,
			face->glyph->bitmap.rows
		);
		m_impl->chars[c].bearing = glm::ivec2(
			face->glyph->bitmap_left,
			face->glyph->bitmap_top
		);
		m_impl->chars[c].advance = face->glyph->advance.x;

		// clear layer
		glTexSubImage3D(
			GL_TEXTURE_2D_ARRAY,
			0,0,0,c,
			pixelheight, pixelheight,
			1, GL_ALPHA, GL_UNSIGNED_BYTE,
			clearBuffer
		);

		// insert layer
		glTexSubImage3D(
			GL_TEXTURE_2D_ARRAY, // target
			0, // level
			0, // xoffset
			0, // yoffset
			c, // zoffset
			m_impl->chars[c].size.x, // width
			m_impl->chars[c].size.y, // height
			1, // depth
			GL_ALPHA, // format
			GL_UNSIGNED_BYTE, // type
			face->glyph->bitmap.buffer // data
		);
	}

	FT_Done_Face(face);
	FT_Done_FreeType(ft);

	glBindTexture(GL_TEXTURE_2D_ARRAY, 0);

	glGenVertexArrays(1, &m_impl->gl_vao);
	glGenBuffers(1, &m_impl->gl_vbo);

	glBindVertexArray(m_impl->gl_vao);

	glBindBuffer(GL_ARRAY_BUFFER, m_impl->gl_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(CharVertex) * _maxChars, (void*)0, GL_DYNAMIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(CharVertex), (void*)0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(CharVertex), (void*)offsetof(CharVertex,pos));

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	util::checkGlCalls(__FUNCSIG__);
}

void Text::setText(const std::string text) {
	m_impl->text = text;
}
void Text::setPos(const glm::ivec2 posInPixel) {
	m_impl->pos = posInPixel;
}

void Text::draw(const sf::Window& win, const Program& program) {
	glDepthMask(GL_FALSE);

	const glm::vec2 ps = glm::vec2(
		2.0f / (float)win.getSize().x,
		2.0f / (float)win.getSize().y
	);

	const float tx = .0f + (m_impl->pos.x * ps.x);
	const float ty = .0f + (m_impl->pos.y * ps.y);

	auto verts = m_impl->createVertices(ps);

	program.bind();
	program.setUniform("translate", glm::translate(glm::identity<glm::mat4>(), glm::vec3(tx,ty,0.0)));

	glActiveTexture(0);
	glBindTexture(GL_TEXTURE_2D_ARRAY, m_impl->gl_tex2DArray);

	glBindVertexArray(m_impl->gl_vao);

	glBindBuffer(GL_ARRAY_BUFFER, m_impl->gl_vbo);
	glBufferSubData(GL_ARRAY_BUFFER,0,sizeof(CharVertex) * verts.size(), verts.data());

	glDrawArrays(GL_QUADS, 0, verts.size());

	glBindVertexArray(0);
	program.unbind();

	glDepthMask(GL_TRUE);
}