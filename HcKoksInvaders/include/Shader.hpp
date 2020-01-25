#pragma once
#include <string>
#include "PreloadData.hpp"

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

	bool loadFromMemory(uint8_t* buffer,size_t bufferLength,ShaderType shaderType);
	bool loadFromString(const std::string str, ShaderType shaderType);
	bool loadFromPreloadData(const PreloadData& pData, ShaderType shaderType);

	const ShaderType& getType() const;
	unsigned int getID() const;
};

