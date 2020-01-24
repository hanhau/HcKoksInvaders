#pragma once
#include <map>
#include <vector>
#include <thread>
#include <string>
#include "Model.hpp"
#include "PreloadData.hpp"

class ModelManager {
private:
	// Preload Mechanic
	static std::map<std::string, PreloadData> m_preloadData;
	static std::thread m_preloadThread;

	static std::map<std::string, Model3D> m_models;
	static void add(const std::string path, const TextureManager& texMgr, const Program& prog);

	// Disallow Instances
	ModelManager() {};
public:
	static void preloadToMemory();
	static void waitForMemoryPreload();

	static void init(const TextureManager& texMgr, const ProgramManager& progMgr);

	static bool exists(const std::string path);
	static const Model3D& getModel(const std::string path);
};