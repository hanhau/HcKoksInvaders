#include "include/Cubemap.hpp"
#include <GL/glew.h>
#include <Util.hpp>

Cubemap::Cubemap(TextureManager& texMgr) {
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	glGenTextures(1, &gl_texID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, gl_texID);

	sf::Image front = texMgr.get("res/images/cubemap/front.png").copyToImage();
	sf::Image back  = texMgr.get("res/images/cubemap/back.png").copyToImage();
	sf::Image left  = texMgr.get("res/images/cubemap/left.png").copyToImage();
	sf::Image right = texMgr.get("res/images/cubemap/right.png").copyToImage();
	sf::Image top   = texMgr.get("res/images/cubemap/top.png").copyToImage();
	sf::Image bot   = texMgr.get("res/images/cubemap/bot.png").copyToImage();

	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z,
		0, GL_RGBA,
		front.getSize().x, front.getSize().y,
		0, GL_RGBA,
		GL_UNSIGNED_BYTE, front.getPixelsPtr());

	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
		0, GL_RGBA,
		back.getSize().x, back.getSize().y,
		0, GL_RGBA,
		GL_UNSIGNED_BYTE, back.getPixelsPtr());

	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
		0, GL_RGBA,
		left.getSize().x, left.getSize().y,
		0, GL_RGBA,
		GL_UNSIGNED_BYTE, left.getPixelsPtr());

	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X,
		0, GL_RGBA,
		right.getSize().x, right.getSize().y,
		0, GL_RGBA,
		GL_UNSIGNED_BYTE, right.getPixelsPtr());

	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
		0, GL_RGBA,
		bot.getSize().x, bot.getSize().y,
		0, GL_RGBA,
		GL_UNSIGNED_BYTE, bot.getPixelsPtr());

	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
		0, GL_RGBA,
		top.getSize().x, top.getSize().y,
		0, GL_RGBA,
		GL_UNSIGNED_BYTE, top.getPixelsPtr());

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	util::checkGlCalls(__FUNCSIG__);
}

unsigned int Cubemap::getGlID() {
	return gl_texID;
}