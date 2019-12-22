#include "include/TextRenderer.hpp"
#include <ft2build.h>
#include FT_FREETYPE_H

TextRenderer::TextRenderer(const ProgramManager& progMgr, 
						   const sf::Window& win) :
	m_progRef(progMgr.get(ProgramManager::ProgramEntry::Text))
{
	
}

void TextRenderer::draw(const Text& text) const {

}