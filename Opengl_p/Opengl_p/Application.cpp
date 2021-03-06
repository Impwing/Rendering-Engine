#include "Application.h"

//Creates a default window. 
Application::Application() {

	this->window = new WND();
	this->window->start();
}

//Creates a window with a specific size
Application::Application(int WNDW, int WNDH) {

	this->window = new WND(WNDW, WNDH);
	this->window->start();
	this->objectManager = new ObjectManager();
	this->deltaTime = new Deltatime();
	this->shaderManager = new ShaderManager();
	this->particleManager = new ParticleManager();

}

Application::~Application() {
	//Critical error when deleting this->window??
	delete this->window;
	delete this->camera;
	delete this->objectManager;
	delete this->deltaTime;
	delete this->shaderManager;
	delete this->particleManager;
}

//Setup the matrixes
void Application::start() {

	this->camera = new Camera(glm::vec3(0, 0, 3), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0), this->window->getWindow());

	this->prjMatrix = glm::perspective(glm::radians(65.0f), (float)this->window->getResolution().first / (float)this->window->getResolution().second, 0.1f, 50.0f);
	


	//Set View Matrix
	this->shaderManager->getSpecific("GeometryPass")->setMat4("viewMatrix", this->camera->getViewMatrix());
	//Set Projection Matrix
	this->shaderManager->getSpecific("GeometryPass")->setMat4("prjMatrix", this->prjMatrix);

	this->currentKey = ValidKeys::DUMMY;
	//Start the delta timer
	this->deltaTime->start();
	this->deltaTime->end();
}

void Application::setupShaders() {
	this->shaderManager->insertShader(
		SHADERPATH + "GeometryPassVS.glsl",
		SHADERPATH + "GeometryPassGS.glsl",
		SHADERPATH + "GeometryPassFS.glsl", "GeometryPass");

	Shader* lightPass = this->shaderManager->insertShader(
		SHADERPATH + "LightPassVS.glsl",
		SHADERPATH + "LightPassFS.glsl", "LightPass");

	lightPass->use();
	lightPass->setInt("positionBuffer", 0);
	lightPass->setInt("normalBuffer", 1);
	lightPass->setInt("colourBuffer", 2);
	lightPass->setInt("depthMap", 3);

	Shader* shadowshader = this->shaderManager->insertShader(
		SHADERPATH + "ShadowPassVS.glsl", 
		SHADERPATH + "ShadowPassFS.glsl", "ShadowPass");

	Shader* particleshader = this->shaderManager->insertShader(
		SHADERPATH + "ParticleVS.glsl",
		SHADERPATH + "ParticleFS.glsl", "ParticlePass");
	
}

void Application::loadObjects() {
	this->objectManager->readFromFile("ExampleOBJ.obj", "RCube", ObjectTypes::Standard, this->shaderManager->getSpecific("GeometryPass"));
	this->objectManager->readFromFile("sphere.obj", "Sphere", ObjectTypes::Standard, this->shaderManager->getSpecific("GeometryPass"));

	this->objectManager->readFromFile("ExampleOBJ.obj", "Cube", ObjectTypes::Standard, this->shaderManager->getSpecific("GeometryPass"));
	this->objectManager->readFromFile("HeightMap3.png", "Terrain", ObjectTypes::HeightMapBased, this->shaderManager->getSpecific("GeometryPass"));
	this->objectManager->readFromFile("ExampleOBJ.obj", "L1", ObjectTypes::LightSource, this->shaderManager->getSpecific("LightPass"));
	this->objectManager->readFromFile("ExampleOBJ.obj", "L2", ObjectTypes::LightSource, this->shaderManager->getSpecific("LightPass"));
}

//Runs every tick while the window is open
void Application::update() {
	this->setupShaders();
	this->loadObjects();

	//shadows
	Shader* shadowPass = this->shaderManager->getSpecific("ShadowPass");
	shadowPass->use();

	//Automatically gets the screen resolution
	unsigned int depthWidth = this->window->getResolution().first;
	unsigned int depthHeight = this->window->getResolution().second;
	unsigned int depthFramebuffer;

	glGenFramebuffers(1, &depthFramebuffer);

	glGenTextures(1, &this->depthMap);
	glBindTexture(GL_TEXTURE_2D, this->depthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, depthWidth, depthHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

	float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

	glBindFramebuffer(GL_FRAMEBUFFER, depthFramebuffer);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, this->depthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);


	Shader* geometryPass = this->shaderManager->getSpecific("GeometryPass");
	geometryPass->use();
	geometryPass->setInt("colorTexture", 0);
	geometryPass->setInt("normalMap", 1);
	
	this->renderer.start(this->window->getResolution().first, this->window->getResolution().second);

	this->start();
	Shader* lightPass = this->shaderManager->getSpecific("LightPass");

	while (!glfwWindowShouldClose(this->window->getWindow())) {
		this->window->update();

		this->deltaTime->start();

		//Check input
		this->window->inputKey(this->currentKey);

		this->mousePick();
		//Camera function 
		this->cameraHandler(geometryPass);
		lightPass->use();
		lightPass->setVec3("cameraPos", camera->getCameraPosition());
		//Render the VAO with the loaded shader
		#pragma region Shadows
		//shadows 
		glm::mat4 lightprjMatrix = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 0.1f, 50.0f);

		glm::mat4 lightMatrixes = lightprjMatrix * camera->getshadowViewMatrix();
		shadowPass->use();
		shadowPass->setMat4("lightMatrixes", lightMatrixes);

		//For shadow mapping
		this->window->setViewport1(depthWidth, depthHeight);
		glBindFramebuffer(GL_FRAMEBUFFER, depthFramebuffer);
		glClear(GL_DEPTH_BUFFER_BIT);
		this->renderer.render(this->objectManager->getObjects(), shadowPass, depthFramebuffer);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		std::pair <int, int> temp;
		temp = window->getResolution();
		this->window->setViewport1(temp.first, temp.second);
		#pragma endregion


		this->particleManager->update();
		
		lightPass->use();
		this->renderer.clearBuffers();

		//lightPass->setMat4("lightMatrixes", camera->getshadowViewMatrix());

		lightPass->setMat4("lightMatrixes", lightMatrixes);

		this->render();
		Shader* particlePass = this->shaderManager->getSpecific("ParticlePass");
		this->renderer.particlesRender(this->particleManager, particlePass, camera->getViewMatrix(), this->prjMatrix);

	}
	//Quit the program
	this->end();
}

void Application::render() {
	this->renderer.clearBuffers();
	this->acceleration->frontBackRendering(objectManager->handleObjects(), camera->getCameraPosition());
	this->renderer.deferredRender(objectManager->getObjects(), this->shaderManager, this->depthMap);

	//Deltatime in ms
	this->deltaTime->end();
	this->deltaT = this->deltaTime->deltaTime();
}

//Have this be in an object class
void Application::cameraHandler(Shader* geometryPass) {

	this->camera->update();
	if (this->currentKey != ValidKeys::DUMMY) {
		glm::vec3 cameraPos = camera->getCameraPosition();
		float yValue = objectManager->getElevation(cameraPos);

		camera->handleKeys(this->currentKey, yValue, (float)this->deltaT);

	}

	this->currentKey = ValidKeys::DUMMY;
	geometryPass->use();
	geometryPass->setMat4("worldMatrix", this->worldMatrix);
	// This is so that we can "walk" with wasd keys
	geometryPass->setMat4("viewMatrix", this->camera->getViewMatrix());
}

void Application::end() {
	for (size_t i = 0; i < this->objectManager->getObjects().size(); i++) {
		if (this->objectManager->getObjects().at(i).type == ObjectTypes::LightSource) {
			this->objectManager->destroyLight(i);
		}
	}
	this->renderer.clear();
	this->window->close();
}

void Application::mousePick() {
	float x = this->camera->getMousePosition().first;
	float y = this->camera->getMousePosition().second;


	if (x != -1 && y != -1) {
		if (this->currentKey == ValidKeys::LEFT_MOUSE_BUTTON) {
			//Go from the viewport space to NDC space
			x = (2 * x) / this->window->getResolution().first - 1;
			y = 1 - (2 * y) / this->window->getResolution().second;


			//Clip-space
			glm::vec4 coords = glm::vec4(x, y, -1.0f, 1.0f);

			//Get view space coords
			//Inverse projection matrix
			glm::mat4 inversePRJ = glm::inverse(this->prjMatrix);

			glm::vec4 mouseRay = inversePRJ * coords;
			mouseRay.z = -1;
			mouseRay.w = 0;
			
			//inverse view matrix
			glm::mat4 inverseVM = glm::inverse(this->camera->getViewMatrix());
			mouseRay = inverseVM * mouseRay;

			glm::vec3 rayInWorldSpace = glm::normalize(glm::vec3(mouseRay.x, mouseRay.y, mouseRay.z));

			int hit = this->objectManager->checkMousePicking(this->camera->getCameraPosition(), rayInWorldSpace);

			std::cout << "x: " << rayInWorldSpace.x << "| y: " << rayInWorldSpace.y << 
				"| z: " << rayInWorldSpace.z << std::endl;
			if (hit > 0)
			{
				std::vector<Object> obj = this->objectManager->getObjects();

				this->particleManager->addParticles(obj.at(hit).getPosition(), glm::vec3(0.0f, 0.1f, 0.0f), 1);
				std::cout << "HIT" << std::endl;
			}
		}
	}


}