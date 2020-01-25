#include "include/Cubemap.hpp"
#include <GL/glew.h>
#include <Util.hpp>
#include "include/TextureManager.hpp"

Cubemap::Cubemap() {
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	glGenTextures(1, &gl_texID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, gl_texID);

	const Texture& front = TextureManager.get("res/images/cubemap/front.png");
	const Texture& back  = TextureManager.get("res/images/cubemap/back.png");
	const Texture& left  = TextureManager.get("res/images/cubemap/left.png");
	const Texture& right = TextureManager.get("res/images/cubemap/right.png");
	const Texture& top   = TextureManager.get("res/images/cubemap/top.png");
	const Texture& bot   = TextureManager.get("res/images/cubemap/bot.png");

	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z,
		0, front.getFormat(),
		front.getSize().x, front.getSize().y,
		0, front.getFormat(),
		GL_UNSIGNED_BYTE, front.getBuffer());

	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
		0, back.getFormat(),
		back.getSize().x, back.getSize().y,
		0, back.getFormat(),
		GL_UNSIGNED_BYTE, back.getBuffer());

	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
		0, left.getFormat(),
		left.getSize().x, left.getSize().y,
		0, left.getFormat(),
		GL_UNSIGNED_BYTE, left.getBuffer());

	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X,
		0, right.getFormat(),
		right.getSize().x, right.getSize().y,
		0, right.getFormat(),
		GL_UNSIGNED_BYTE, right.getBuffer());

	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
		0, bot.getFormat(),
		bot.getSize().x, bot.getSize().y,
		0, bot.getFormat(),
		GL_UNSIGNED_BYTE, bot.getBuffer());

	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
		0, top.getFormat(),
		top.getSize().x, top.getSize().y,
		0, top.getFormat(),
		GL_UNSIGNED_BYTE, top.getBuffer());

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	util::checkGlCalls(__FUNCTION__);
}

unsigned int Cubemap::getGlID() {
	return gl_texID;
}