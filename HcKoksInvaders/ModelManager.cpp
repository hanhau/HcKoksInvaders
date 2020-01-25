#include "include/ModelManager.hpp"
#include <iostream>
#include <fstream>
#include <thread>
#include <string>
#include <vector>

_ModelManager ModelManager;

_ModelManager::_ModelManager() : 
	ResourceManagerBase({
		"res/models/vengabus.obj",
		"res/models/vengabus_hq.obj",
		"res/models/ship1.obj",
		"res/models/turret_base.obj",
		"res/models/turret_head.obj",
		"res/models/money.obj",
		"res/models/finger.obj",
		"res/models/medibox.obj"
	})
{}

void _preloadFunc(const std::vector<std::string>& loadPaths,
				  std::map<std::string, PreloadData>& preloadData,
				  std::map<std::string, Model3D> * const models) 
{
	for (const auto& path : loadPaths) {
		std::ifstream file(path, std::ios::in | std::ios::binary | std::ios::ate);

		size_t fileLength = file.tellg();
		file.seekg(0);

		preloadData[path] = PreloadData();
		preloadData[path].m_dataLength = fileLength;
		preloadData[path].m_path = path;
		preloadData[path].m_data = std::unique_ptr<uint8_t>(new uint8_t[fileLength]);

		file.read((char*)preloadData[path].m_data.get(), fileLength);
	}

	for (const auto& path : loadPaths) {
		try {
			if (models->find(path) != models->end())
				throw "Model already exists @ " + std::string(__FUNCTION__);

			models->emplace(path, Model3D());

			if (!models->at(path).loadFileFromMemory(
				preloadData[path].m_data.get(),
				preloadData[path].m_dataLength,
				path
			))
				throw "Unable to load " + path + " @ " + std::string(__FUNCTION__);
		}
		catch (std::string e) {
			std::cout << e << "\n";
		}
	}
}

void _ModelManager::preloadToMemory() {
	m_rmPreloadData.clear();

	m_rmPreloadThread = std::thread(
		_preloadFunc,
		std::ref(m_rmPreloadPaths),std::ref(m_rmPreloadData),&m_models
	);
}

void _ModelManager::waitForMemoryPreload() {
	if(m_rmPreloadThread.joinable())
		m_rmPreloadThread.join();
}

void _ModelManager::init() {
	for (auto& iter : m_models) {
		iter.second.uploadToGl();
	}

	m_rmPreloadData.clear();
}

bool _ModelManager::exists(const std::string path) {
	return m_models.find(path) != m_models.end();
}

const Model3D& _ModelManager::getModel(const std::string path) {
	if (!exists(path))
		throw;

	return m_models.at(path);
}
