#include "include/ModelManager.hpp"
#include <iostream>

void ModelManager::add(const std::string path) {
	try {
		if (exists(path))
			throw "Model already exists @ ModelManager::load(path)";

		Model3D temp_model;
		if (!temp_model.loadFromFile(path))
			throw "Unable to load " + path + " @ ModelManager::add(path)";

		m_models.emplace(path,std::move(temp_model));
	}
	catch (std::string e)
	{
		std::cout << e << std::endl;
	}
}

ModelManager::ModelManager() {
	add("res/models/vengabus.obj");
	add("res/models/vengabus_hq.obj");
	add("res/models/ship1.obj");
	add("res/models/sphere.obj");
	add("res/models/main_menu_title.obj");
	add("res/models/turret_base.obj");
	add("res/models/turret_head.obj");
	add("res/models/money.obj");
	add("res/models/finger.obj");
}

bool ModelManager::exists(const std::string path) {
	return m_models.find(path) != m_models.end();
}

Model3D& const ModelManager::getModel(const std::string path) {
	if (!exists(path))
		throw;

	return m_models[path];
}
