#include "WND.h"

void setViewport(GLFWwindow* window, int width, int height);

WND::WND()
{
	this->WNDW = 800;
	this->WNDH = 600;
}

WND::WND(int width, int height)
{
	this->WNDW = width;
	this->WNDH = height;
}

WND::~WND()
{
	//delete this->window;
}

//Start handler for the window. 
int WND::start()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	this->window = glfwCreateWindow(this->WNDW, this->WNDH, "OPENGL Project", NULL, NULL);

	//If the window fails to instantiate 
	if (this->window == NULL) {
		std::cout << "Failed to create window" << std::endl;
		glfwTerminate();
		return -1;
	}

	//Makes the context of the specified window current for the calling thread
	glfwMakeContextCurrent(this->window);
	
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to load GLAD" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwSetFramebufferSizeCallback(this->window, setViewport);
	
	return 0;
}

void WND::update() {
	glfwSwapBuffers(this->window);
	//glfwSwapInterval(1);
	glfwPollEvents();
}

void WND::inputKey(ValidKeys & key) {
	if (glfwGetKey(this->window, GLFW_KEY_W) == GLFW_PRESS) {
		key = ValidKeys::W;
		return;
	}
	if (glfwGetKey(this->window, GLFW_KEY_S) == GLFW_PRESS) {
		key = ValidKeys::S;
		return;
	}
	if (glfwGetKey(this->window, GLFW_KEY_A) == GLFW_PRESS) {
		key = ValidKeys::A;
		return;
	}
	if (glfwGetKey(this->window, GLFW_KEY_D) == GLFW_PRESS) {
		key = ValidKeys::D;
		return;
	}
	if (glfwGetKey(this->window, GLFW_KEY_Q) == GLFW_PRESS) {
		key = ValidKeys::Q;
		return;
	}
	if (glfwGetKey(this->window, GLFW_KEY_H) == GLFW_PRESS) {
		key = ValidKeys::H;
		return;
	}
	if (glfwGetKey(this->window, GLFW_KEY_E) == GLFW_PRESS) {
		key = ValidKeys::E;
		return;
	}
	if (glfwGetKey(this->window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(this->window, true);
		key = ValidKeys::ESC;
		return;
	}
	if (glfwGetMouseButton(this->window, GLFW_MOUSE_BUTTON_LEFT)) {
		key = ValidKeys::LEFT_MOUSE_BUTTON;
		return;
	}
	if (glfwGetKey(this->window, GLFW_KEY_I) == GLFW_PRESS) {
		key = ValidKeys::I;
		return;
	}
	if (glfwGetKey(this->window, GLFW_KEY_J) == GLFW_PRESS) {
		key = ValidKeys::J;
		return;
	}
	if (glfwGetKey(this->window, GLFW_KEY_K) == GLFW_PRESS) {
		key = ValidKeys::K;
		return;
	}
	if (glfwGetKey(this->window, GLFW_KEY_L) == GLFW_PRESS) {
		key = ValidKeys::L;
		return;
	}
	//if we're not pressing anything then we just set the current key to inactive.
	key = ValidKeys::DUMMY;
}

GLFWwindow* WND::getWindow() const {
	return this->window;
}

void WND::setViewport1(int x, int y) {
	glViewport(0, 0, x, y);
}

std::pair<int, int> WND::getResolution() const {
	return std::make_pair(this->WNDW, this->WNDH);
}

void WND::close() {
	glfwTerminate();
}

void setViewport(GLFWwindow * window, int width, int height)
{
	glViewport(0, 0, width, height);
}

