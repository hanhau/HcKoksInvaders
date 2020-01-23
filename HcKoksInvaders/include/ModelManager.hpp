#pragma once
#include <map>
#include <vector>
#include <thread>
#include "Model.hpp"

struct ModelPreloadData {
	std::unique_ptr<uint8_t*> m_data;
	std::string m_path;
};

class ModelManager {
private:
	// Preload Mechanic
	static std::map<std::string, ModelPreloadData> m_preloadData;
	static std::thread m_preloadThread;

	static std::map<std::string, Model3D> m_models;
	static void add(const std::string path, const TextureManager& texMgr, const Program& prog);
public:
	static void preloadToMemory();
	static void waitForMemoryPreload();

	static void init(const TextureManager& texMgr, const ProgramManager& progMgr);

	static bool exists(const std::string path);
	static const Model3D& getModel(const std::string path);
};