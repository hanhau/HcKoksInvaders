#pragma once
#include "PreloadData.hpp"
#include <thread>
#include <vector>
#include <map>

class ResourceMangerBase {
protected:
	static const std::vector<std::string> m_rmPreloadPaths;
	static std::map<std::string, PreloadData> m_rmPreloadData;
	static std::thread m_rmPreloadThread;
public:
	static void preloadToMemory();
	static void waitForMemoryPreload();

	virtual ~ResourceMangerBase() {}
};
