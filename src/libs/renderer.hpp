#pragma once
#include "system.hpp"
#include "vector3.hpp"
#include <GLFW/glfw3.h>

class Engine;
struct Triangle {
	Vector3 vertices[3];
};

class Renderer : public System {
public:
	Renderer(Engine &engine, int width, int height, const char *window_name, unsigned int buffer_size);
	~Renderer();
	void Update(float dt);
private:
	GLFWwindow *_window;
	Vector3 _window_size;
	float *_positions_buffer;
	unsigned int _buffer_size;
};
