#pragma once
#include "ModelManager.hpp"
#include <map>
#include "Model.hpp"

class ModelManager {
	friend class ModelInstance;
private:
	std::map<std::string, Model3D> m_models;
	void add(const std::string path);
public:
	ModelManager();

	bool exists(const std::string path);
	Model3D& const getModel(const std::string path);
};