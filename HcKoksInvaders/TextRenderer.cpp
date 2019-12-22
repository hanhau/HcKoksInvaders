#include "include/TextRenderer.hpp"

TextRenderer::TextRenderer(const ProgramManager& progMgr, 
						   const sf::Window& win) :
	m_progRef(progMgr.get(ProgramManager::ProgramEntry::Model3D))
{
	
}

void TextRenderer::draw(const Text& text) const {

}