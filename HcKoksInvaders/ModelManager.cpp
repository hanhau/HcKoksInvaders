#include "include/ModelManager.hpp"
#include <iostream>

void ModelManager::add(const std::string path, const TextureManager& texMgr) {
	try {
		if (exists(path))
			throw "Model already exists @ ModelManager::load(path)";

		Model3D temp_model;
		if (!temp_model.loadFromFile(path,texMgr))
			throw "Unable to load " + path + " @ ModelManager::add(path)";

		m_models.emplace(path,std::move(temp_model));
	}
	catch (std::string e)
	{
		std::cout << e << std::endl;
	}
}

ModelManager::ModelManager(const TextureManager& texMgr) {
	add("res/models/vengabus.obj",texMgr);
	add("res/models/vengabus_hq.obj",texMgr);
	add("res/models/ship1.obj",texMgr);
	add("res/models/sphere.obj",texMgr);
	add("res/models/main_menu_title.obj",texMgr);
	add("res/models/turret_base.obj",texMgr);
	add("res/models/turret_head.obj",texMgr);
	add("res/models/money.obj",texMgr);
	add("res/models/finger.obj",texMgr);
}

bool ModelManager::exists(const std::string path) {
	return m_models.find(path) != m_models.end();
}

Model3D& const ModelManager::getModel(const std::string path) {
	if (!exists(path))
		throw;

	return m_models[path];
}
