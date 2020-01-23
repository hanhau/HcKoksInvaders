#include "include/ModelManager.hpp"
#include <iostream>
#include <fstream>
#include <thread>
#include <string>
#include <vector>

std::map<std::string, ModelPreloadData> ModelManager::m_preloadData;
std::thread								ModelManager::m_preloadThread;
std::map<std::string, Model3D>			ModelManager::m_models;

static const std::vector<std::string> LOAD_PATHS({
	"res/models/vengabus.obj",
	"res/models/vengabus_hq.obj",
	"res/models/ship1.obj",
	"res/models/turret_base.obj",
	"res/models/turret_head.obj",
	"res/models/money.obj",
	"res/models/finger.obj",
	"res/models/medibox.obj"
});

void ModelManager::preloadToMemory() {
	m_preloadData.clear();

	m_preloadThread = std::move(std::thread([]
		(const std::vector<std::string>& loadPaths,
		 std::map<std::string, ModelPreloadData> &preloadData) 
		{
			for (const auto& path : loadPaths) {
				std::ifstream file(path, std::ios::in | std::ios::binary | std::ios::ate);

				size_t fileLength = file.tellg();
				file.seekg(0);

				preloadData[path] = ModelPreloadData();
				preloadData[path].m_dataLength = fileLength;
				preloadData[path].m_path = path;
				preloadData[path].m_data = std::unique_ptr<uint8_t>(new uint8_t[fileLength]);

				file.read((char*)preloadData[path].m_data.get(), fileLength);
			}
		},
		std::ref(LOAD_PATHS),std::ref(m_preloadData)
	));
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

		if (!m_models.at(path).loadFileFromMemory(
			m_preloadData[path].m_data.get(),
			m_preloadData[path].m_dataLength,
			path,
			texMgr
		))
			throw "Unable to load " + path + " @ " + std::string(__FUNCSIG__);
	}
	catch (std::string e)
	{
		std::cout << e << std::endl;
	}
}

void ModelManager::init(const TextureManager& texMgr,const ProgramManager& progMgr) {
	const Program* p = &progMgr.get(ProgramManager::ProgramEntry::Model3D);
	
	add("res/models/vengabus.obj",texMgr, *p);
	add("res/models/vengabus_hq.obj",texMgr, *p);
	add("res/models/ship1.obj",texMgr, *p);
	add("res/models/turret_base.obj",texMgr, *p);
	add("res/models/turret_head.obj",texMgr, *p);
	add("res/models/money.obj",texMgr, *p);
	add("res/models/finger.obj",texMgr, *p);
	add("res/models/medibox.obj", texMgr, *p);

	m_preloadData.clear();
}

bool ModelManager::exists(const std::string path) {
	return m_models.find(path) != m_models.end();
}

const Model3D& ModelManager::getModel(const std::string path) {
	if (!exists(path))
		throw;

	return m_models.at(path);
}
