#pragma once
#include "system.hpp"
#include "vectors.hpp"
#include <GLFW/glfw3.h>

class Engine;
struct Triangle {
	Vector3 vertices[3];
	Vector3 color;
};

struct Rectangle {
	Vector3 vertices[4];
	Vector3 color;	
};

struct Transform {
    Vector3 position;
    double rotation; 
};

template<typename T>
struct Buffer {
	T * data;
	unsigned int size;
	T * current;

	Buffer(unsigned int initial_size) {
		size = initial_size;
		data = (T *)malloc(size * sizeof(T));
		current = data;
	}

	void Empty() {
		current = data;
	}
	
	void Append(T element) {
		*current = element;
		++current; 
	}

	void Reserve(unsigned int elements) {
		if (size < elements) {
			size = elements;
			data = (T *)realloc(data, size * sizeof(T));
		}
	}

	~Buffer() {
		free(data);
	}
};

class Renderer : public System {
public:
	Renderer(Engine &engine, Vector2Int window_size, const char *window_name, unsigned int buffer_size);
	Renderer(Engine &engine, Vector2Int window_size, const char *window_name);
	~Renderer();
	void Update(float dt);
private:
	GLFWwindow *_window;
	Vector2Int _window_size;
	Buffer<float> _vertex_buffer;
	Buffer<unsigned int> _index_buffer;
};
