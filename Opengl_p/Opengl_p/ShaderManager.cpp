#include "ShaderManager.h"

ShaderManager::ShaderManager() {
}

ShaderManager::~ShaderManager() {
	for (size_t i = 0; i < this->shaders.size(); i++) {
		delete shaders.at(i);
	}
}

Shader* ShaderManager::insertShader(std::string vPath, std::string fPath, std::string name) {
	Shader* newShader = new Shader(vPath, fPath);
	newShader->setName(name);
	shaders.push_back(newShader);
	return newShader;
}

Shader*  ShaderManager::insertShader(std::string vPath, std::string gPath, std::string fPath, std::string name) {
	Shader* newShader = new Shader(vPath, gPath, fPath);
	newShader->setName(name);
	shaders.push_back(newShader);
	return newShader;
}

std::vector<Shader*> ShaderManager::getShaders() const {
	return shaders;
}

Shader* ShaderManager::getSpecific(std::string name) const {
	for (size_t i = 0; i < shaders.size(); i++) {
		if (shaders.at(i)->getName() == name) {
			return shaders.at(i);
		}
	}

	return nullptr;
}
