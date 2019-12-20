#pragma once
#include <map>
#include "ModelManager.hpp"
#include "Model.hpp"

class ModelManager {
	friend class ModelInstance;
private:
	std::map<std::string, Model3D> m_models;
	void add(const std::string path, const TextureManager& texMgr);
public:
	ModelManager(const TextureManager& texMgr);

	bool exists(const std::string path);
	Model3D& const getModel(const std::string path);
};