#pragma once
#include <map>
#include "Program.hpp"
#include "ResourceManagerBase.hpp"

enum class ProgramEntry {
	Model3D,
	MainMenuBackground,
	Bullet,
	Button,
	AmmunitionIcon,
	Text,
	IngameBackground
};

class _ProgramManager : public ResourceManagerBase {
	std::map<const ProgramEntry, Program> m_programs;
	void add(const ProgramEntry se, Shader const& vert, Shader const& frag);

public:
	_ProgramManager();

	void preloadToMemory();
	void waitForMemoryPreload();

	void init();

	bool exists(const ProgramEntry se);
	const Program& get(const ProgramEntry se);
};

extern _ProgramManager ProgramManager;