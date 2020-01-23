#include "include/ModelManager.hpp"
#include <iostream>
#include <fstream>
#include <thread>

const std::vector<const std::string> LOAD_PATHS = {
	"res/models/vengabus.obj",
	"res/models/vengabus_hq.obj",
	"res/models/ship1.obj",
	"res/models/turret_base.obj",
	"res/models/turret_head.obj",
	"res/models/money.obj",
	"res/models/finger.obj",
	"res/models/medibox.obj"
};

void ModelManager::preloadToMemory() {
	m_preloadData.clear();

	m_preloadThread = std::thread([]
		(const std::vector<const std::string&> loadPaths,
		 std::map<std::string, ModelPreloadData> &preloadData) 
		{
			for (const auto& path : loadPaths) {
				std::ifstream file(path, std::ios::in | std::ios::binary | std::ios::ate);

				size_t fileLength = file.tellg();
				files[path] = std::make_unique<uint8_t*>(new uint8_t[fileLength]);
				file.seekg(0);

				file.read((char*)files[path].get(), fileLength);
			}
		},
		LOAD_PATHS,m_preloadData
	);
}

void ModelManager::waitForMemoryPreload() {
	m_preloadThread.join();
}

void ModelManager::add(const std::string path, const TextureManager& texMgr,const Program& prog) {
	try {
		if (exists(path))
			throw "Model already exists @ " + std::string(__FUNCSIG__);

		m_models.emplace(
			std::piecewise_construct,
			std::forward_as_tuple(path), 
			std::forward_as_tuple(prog)
		);

		if (!m_models.at(path).loadFileFromMemory(path,texMgr))
			throw "Unable to load " + path + " @ ModelManager::add(path)";
	}
	catch (std::string e)
	{
		std::cout << e << std::endl;
	}
}

ModelManager::ModelManager(const TextureManager& texMgr,const ProgramManager& progMgr) {
	const Program* p = &progMgr.get(ProgramManager::ProgramEntry::Model3D);
	
	add("res/models/vengabus.obj",texMgr, *p);
	add("res/models/vengabus_hq.obj",texMgr, *p);
	add("res/models/ship1.obj",texMgr, *p);
	add("res/models/turret_base.obj",texMgr, *p);
	add("res/models/turret_head.obj",texMgr, *p);
	add("res/models/money.obj",texMgr, *p);
	add("res/models/finger.obj",texMgr, *p);
	add("res/models/medibox.obj", texMgr, *p);
}

void ModelManager::init() {

}

bool ModelManager::exists(const std::string path) const {
	return m_models.find(path) != m_models.end();
}

const Model3D& ModelManager::getModel(const std::string path) const {
	if (!exists(path))
		throw;

	return m_models.at(path);
}
