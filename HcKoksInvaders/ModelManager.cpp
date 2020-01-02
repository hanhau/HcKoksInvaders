#include "include/ModelManager.hpp"
#include <iostream>

void ModelManager::add(const std::string path, const TextureManager& texMgr,const Program& prog) {
	try {
		if (exists(path))
			throw "Model already exists @ ModelManager::load(path)";

		m_models.emplace(
			std::piecewise_construct,
			std::forward_as_tuple(path), 
			std::forward_as_tuple(prog)
		);

		if (!m_models.at(path).loadFromFile(path,texMgr))
			throw "Unable to load " + path + " @ ModelManager::add(path)";
	}
	catch (std::string e)
	{
		std::cout << e << std::endl;
	}
}

ModelManager::ModelManager(const TextureManager& texMgr,const ProgramManager& progMgr) {
	const Program* p = &progMgr.get(ProgramManager::ProgramEntry::Model3D);
	
	add("res/models/vengabus.obj",texMgr, *p);
	add("res/models/vengabus_hq.obj",texMgr, *p);
	add("res/models/ship1.obj",texMgr, *p);
	add("res/models/turret_base.obj",texMgr, *p);
	add("res/models/turret_head.obj",texMgr, *p);
	add("res/models/money.obj",texMgr, *p);
	add("res/models/finger.obj",texMgr, *p);
}

bool ModelManager::exists(const std::string path) const {
	return m_models.find(path) != m_models.end();
}

const Model3D& ModelManager::getModel(const std::string path) const {
	if (!exists(path))
		throw;

	return m_models.at(path);
}
