#pragma once
#include <map>
#include "ModelManager.hpp"
#include "Model.hpp"

class ModelManager {
	friend class ModelInstance;
private:
	std::map<std::string, Model3D> m_models;
	void add(const std::string path, const TextureManager& texMgr, const Program& prog);
public:
	ModelManager(const TextureManager& texMgr, const ProgramManager& progMgr);

	bool exists(const std::string path);
	const Model3D& getModel(const std::string path);
};