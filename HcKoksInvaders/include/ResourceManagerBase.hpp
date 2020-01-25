#pragma once
#include "PreloadData.hpp"
#include <thread>
#include <vector>
#include <map>

class ResourceManagerBase {
protected:
	const std::vector<std::string> m_rmPreloadPaths;
	std::map<std::string, PreloadData> m_rmPreloadData;
	std::thread m_rmPreloadThread;
public:
	inline ResourceManagerBase(const std::vector<std::string> paths) :
		m_rmPreloadPaths(paths)
	{}

	virtual void preloadToMemory() {};
	virtual void waitForMemoryPreload() {};

	virtual ~ResourceManagerBase() {}
};
