#pragma once
#include <SFML/Graphics.hpp>
#include <map>
#include "Program.hpp"

class ProgramManager {
public:
	enum class ProgramEntry;
private:
	std::map<const ProgramEntry, Program> m_programs;
	void add(const ProgramEntry se, Shader const& vert, Shader const& frag);
public:
	enum class ProgramEntry {
		Model3D,
		MainMenuBackground,
		Bullet,
		Button,
		AmmunitionIcon
	};

	ProgramManager();
	~ProgramManager();

	bool exists(const ProgramEntry se);
	Program& get(const ProgramEntry se) const;
};