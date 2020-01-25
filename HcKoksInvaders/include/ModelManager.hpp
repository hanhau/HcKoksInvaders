#pragma once
#include <map>
#include <vector>
#include <thread>
#include <string>
#include "Model.hpp"
#include "PreloadData.hpp"
#include "ResourceManagerBase.hpp"

class _ModelManager : public ResourceManagerBase {
private:
	std::map<std::string, Model3D> m_models;
public:
	_ModelManager();

	void preloadToMemory();
	void waitForMemoryPreload();

	void init();

	bool exists(const std::string path);
	const Model3D& getModel(const std::string path);
};

extern _ModelManager ModelManager;