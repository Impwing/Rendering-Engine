#ifndef SHADER_h
#define SHADER_h

#include <glad/glad.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include "Containers.h"

class Shader {
public:
	//Input vertex path & fragment path
	Shader(std::string vPath, std::string fPath);
	//When we have a geometry shader
	Shader(std::string vPath, std::string gPath, std::string fPath);
	~Shader();

	void use();

	void setName(std::string name);
	std::string getName() const;
	void setMat4(std::string name, glm::mat4 mat);
	void setVec3(std::string name, glm::vec3 vec);
	void setVec4(std::string name, glm::vec4 vec);
	void setFloat(std::string name, float val);
	void setInt(std::string name, int val);
	unsigned int getShaderID() const;

	Shader& operator=(const Shader& otherShader);
private: 
	std::string name;
	unsigned int shaderProg;
};

#endif