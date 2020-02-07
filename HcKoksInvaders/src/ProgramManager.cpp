#include "../include/ProgramManager.hpp"
#include <iostream>
#include <fstream>

_ProgramManager ProgramManager;

_ProgramManager::_ProgramManager() :
	ResourceManagerBase({
		"res/shaders/model_vert.vert",
		"res/shaders/model_frag.frag",

		"res/shaders/background_star_shader.vert",
		"res/shaders/main_menu_frag.frag",

		"res/shaders/background_star_shader.vert",
		"res/shaders/background_star_shader.frag",

		"res/shaders/AmmunitionIcon.vert",
		"res/shaders/AmmunitionIcon.frag",

		"res/shaders/bullet_vert.vert",
		"res/shaders/bullet_frag.frag",

		"res/shaders/Text.vert",
		"res/shaders/Text.frag",

		"res/shaders/button_vert.vert",
		"res/shaders/button_frag.frag"
	})
{}

void _preloadFunc(const std::vector<std::string>& loadPaths,
				  std::map<std::string, PreloadData>& pDataMap,
				  std::map<const ProgramEntry, Program> * const programs) 
{
	pDataMap.clear();

	for (const auto& path : loadPaths) {
		std::ifstream file(path,std::ios::binary | std::ios::in | std::ios::ate);

		size_t fileLength = file.tellg();
		file.seekg(0);

		pDataMap[path] = PreloadData();
		pDataMap[path].m_dataLength = fileLength;
		pDataMap[path].m_path = path;
		pDataMap[path].m_data = std::unique_ptr<uint8_t>(new uint8_t[fileLength]);

		file.read((char*)pDataMap[path].m_data.get(), fileLength);
	}
}

void _ProgramManager::preloadToMemory() {
	m_rmPreloadThread = std::thread(
		_preloadFunc,
		std::ref(m_rmPreloadPaths), std::ref(m_rmPreloadData), &m_programs
	);
}

void _ProgramManager::waitForMemoryPreload() {
	if(m_rmPreloadThread.joinable())
		m_rmPreloadThread.join();
}

void _ProgramManager::add(const ProgramEntry se, Shader const& vert, Shader const& frag) {
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

void _ProgramManager::init()
{
	auto& pData = m_rmPreloadData;

	// Model3D
	Shader model3d_vert, model3d_frag;
	model3d_vert.loadFromPreloadData(pData["res/shaders/model_vert.vert"], Shader::ShaderType::Vertex);
	model3d_frag.loadFromPreloadData(pData["res/shaders/model_frag.frag"], Shader::ShaderType::Fragment);
	add(ProgramEntry::Model3D,model3d_vert,model3d_frag);
	
	// MainMenuBackground
	Shader mm_vert, mm_frag;
	mm_vert.loadFromPreloadData(pData["res/shaders/background_star_shader.vert"],Shader::ShaderType::Vertex);
	mm_frag.loadFromPreloadData(pData["res/shaders/main_menu_frag.frag"],Shader::ShaderType::Fragment);
	add(ProgramEntry::MainMenuBackground, mm_vert, mm_frag);

	// Ingame Background
	Shader igBkg_vert, igBkg_frag;
	igBkg_vert.loadFromPreloadData(pData["res/shaders/background_star_shader.vert"],Shader::ShaderType::Vertex);
	igBkg_frag.loadFromPreloadData(pData["res/shaders/background_star_shader.frag"], Shader::ShaderType::Fragment);
	add(ProgramEntry::IngameBackground, igBkg_vert, igBkg_frag);

	// AmmunitionIcon
	Shader ami_vert, ami_frag;
	ami_vert.loadFromPreloadData(pData["res/shaders/AmmunitionIcon.vert"],Shader::ShaderType::Vertex);
	ami_frag.loadFromPreloadData(pData["res/shaders/AmmunitionIcon.frag"],Shader::ShaderType::Fragment);
	add(ProgramEntry::AmmunitionIcon, ami_vert, ami_frag);

	// Bullet
	Shader bullet_vert, bullet_frag;
	bullet_vert.loadFromPreloadData(pData["res/shaders/bullet_vert.vert"], Shader::ShaderType::Vertex);
	bullet_frag.loadFromPreloadData(pData["res/shaders/bullet_frag.frag"], Shader::ShaderType::Fragment);
	add(ProgramEntry::Bullet, bullet_vert, bullet_frag);

	// Text
	Shader text_vert, text_frag;
	text_vert.loadFromPreloadData(pData["res/shaders/Text.vert"], Shader::ShaderType::Vertex);
	text_frag.loadFromPreloadData(pData["res/shaders/Text.frag"], Shader::ShaderType::Fragment);
	add(ProgramEntry::Text, text_vert, text_frag);

	// Button
	Shader button_vert, button_frag;
	button_vert.loadFromPreloadData(pData["res/shaders/button_vert.vert"],Shader::ShaderType::Vertex);
	button_frag.loadFromPreloadData(pData["res/shaders/button_frag.frag"],Shader::ShaderType::Fragment);
	add(ProgramEntry::Button, button_vert, button_frag);

	m_rmPreloadData.clear();
}

bool _ProgramManager::exists(const ProgramEntry se) {
	return m_programs.find(se) != m_programs.end();
}

const Program& _ProgramManager::get(const ProgramEntry se) {
	return m_programs.at(se);
}