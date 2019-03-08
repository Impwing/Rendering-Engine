#include "ObjectManager.h"

ObjectManager::ObjectManager() {
	this->fileloader = new Fileloader();
	this->objectloader = new ObjectLoader();

}

ObjectManager::~ObjectManager() {
	delete this->fileloader;
	delete this->objectloader;
}

void ObjectManager::start() {

}

std::vector<Object> ObjectManager::getObjects() {
	return this->objects;
}

std::vector<Object>& ObjectManager::handleObjects() {
	return this->objects;
}


void ObjectManager::readFromFile(std::string filename, std::string objName, ObjectTypes objectType, Shader* shader) {
	
	Object obj;
	shader->use();

	if (objectType == ObjectTypes::HeightMapBased) {
		obj = fileloader->loadMap(OBJECTSPATH + filename);
	}
	else if(objectType == ObjectTypes::Standard){
		obj = fileloader->readFile(OBJECTSPATH + filename);
		obj.setPosition(glm::vec3(10, 2, 10));
	}
	else {
		obj = fileloader->readFile(OBJECTSPATH + filename);
	}
	obj.type = objectType;
	//this->objectloader->loadObject(obj, shader);
	obj.init();

	if (objectType == ObjectTypes::LightSource) {
		obj.setPosition(glm::vec3(0, 10, 0));
		
		obj.pointLight = new PointLight();
		obj.pointLight->constant = 1.0f;
		obj.pointLight->linear = 0.7f;
		obj.pointLight->quadratic = 1.8f;
		this->lightcount++;
	}
	if (objName == "L1") {
		obj.setPosition(glm::vec3(45, 3, 10));
	}
	obj.name = objName;
	this->objects.push_back(obj);
}

void ObjectManager::destroy() {
	this->objectloader->clean();
}

float ObjectManager::getElevation(glm::vec3 position)
{
	return fileloader->getElevation(position);
}

ObjectLoader  ObjectManager::getObjectloader() {
	return *this->objectloader;
}

int ObjectManager::getLightCount() const
{
	return this->lightcount;
}

void ObjectManager::destroyLight(int at) {
	this->objects.at(at).destroyLight();
}

