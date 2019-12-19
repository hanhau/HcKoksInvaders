#pragma once
#include <string>

class Shader
{
public:
	enum class ShaderType;
private:
	unsigned int glID;
	ShaderType m_shaderType;
public:
	Shader();
	~Shader();

	enum class ShaderType {
		Vertex,
		Fragment,
		Unitialized
	};

	bool loadFromFile(const std::string path,ShaderType shaderType);
	const ShaderType& getType() const;
	unsigned int getID() const;
};

