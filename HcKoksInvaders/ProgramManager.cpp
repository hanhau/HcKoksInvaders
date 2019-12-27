#include "include/ProgramManager.hpp"
#include <iostream>

void ProgramManager::add(const ProgramEntry se, Shader const& vert, Shader const& frag) {
	try {
		if (exists(se))
			throw "Program does already exists @ ProgramManager::add(se,vert,frag)";

		m_programs.emplace(
			std::piecewise_construct,
			std::forward_as_tuple(se),
			std::forward_as_tuple(vert,frag)
		);
	}
	catch (std::string e)
	{
		std::cout << e << std::endl;
	}
}

ProgramManager::ProgramManager()
{
	// Model3D
	Shader model3d_vert, model3d_frag;
	model3d_vert.loadFromFile("res/shaders/model_vert.vert", Shader::ShaderType::Vertex);
	model3d_frag.loadFromFile("res/shaders/model_frag.frag", Shader::ShaderType::Fragment);

	add(ProgramEntry::Model3D,model3d_vert,model3d_frag);
	
	// MainMenuBackground
	Shader mm_vert, mm_frag;
	mm_vert.loadFromFile("res/shaders/background_star_shader.ve rt",Shader::ShaderType::Vertex);
	mm_frag.loadFromFile("res/shaders/main_menu_frag.frag",Shader::ShaderType::Fragment);

	add(ProgramEntry::MainMenuBackground, mm_vert, mm_frag);

	// AmmunitionIcon
	Shader ami_vert, ami_frag;
	ami_vert.loadFromFile("res/shaders/AmmunitionIcon.vert",Shader::ShaderType::Vertex);
	ami_frag.loadFromFile("res/shaders/AmmunitionIcon.frag",Shader::ShaderType::Fragment);

	add(ProgramEntry::AmmunitionIcon, ami_vert, ami_frag);

	// Bullet
	Shader bullet_vert, bullet_frag;
	bullet_vert.loadFromFile("res/shaders/bullet_vert.vert", Shader::ShaderType::Vertex);
	bullet_frag.loadFromFile("res/shaders/bullet_frag.frag", Shader::ShaderType::Fragment);

	add(ProgramEntry::Bullet, bullet_vert, bullet_frag);

	// Text
	Shader text_vert, text_frag;
	text_vert.loadFromFile("res/shaders/Text.vert", Shader::ShaderType::Vertex);
	text_frag.loadFromFile("res/shaders/Text.frag", Shader::ShaderType::Fragment);

	add(ProgramEntry::Text, text_vert, text_frag);
}

ProgramManager::~ProgramManager() {
	m_programs.clear();
}

bool ProgramManager::exists(const ProgramEntry se) const {
	return m_programs.find(se) != m_programs.end();
}

const Program& ProgramManager::get(const ProgramEntry se) const
{
	return m_programs.at(se);
}