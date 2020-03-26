#include <assimp/scene.h>
#include <assimp/Importer.hpp>

bool importModel() {
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile("", 0);

	aiTexture tex;
	return false;
}