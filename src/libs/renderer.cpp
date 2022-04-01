#include <GL/glew.h>
#include "renderer.hpp"
#include <GLFW/glfw3.h>
#include <cassert>
#include "vector3.hpp"
#include "system.hpp"
#include "engine.hpp"
#include "config.hpp"
#include <iostream>
#include "profiler.hpp"

#define GLCall(x) ClearError();\
    x;\
    assert(GetErrors(#x, __FILE__, __LINE__))

static void ClearError(){
    while (glGetError() != GL_NO_ERROR);
}

static std::string_view GetErrorMessage(unsigned int code){
    switch(code){
        case GL_INVALID_ENUM:      return "Invalid enum";
        case GL_INVALID_VALUE:     return "Invalid value";
        case GL_INVALID_OPERATION: return "Invalid operation";
        case GL_STACK_OVERFLOW:    return "Stack overflow";
        case GL_STACK_UNDERFLOW:   return "Stack underflow";
        case GL_OUT_OF_MEMORY:     return "Out of memory";
        default:                   return "Unknown error";
    }
}

static bool GetErrors(const char* func, const char* file, unsigned int line){
    bool flag = true;
    while(GLenum err = glGetError()){
        std::cout << "[OpenGL ERROR] " << GetErrorMessage(err) << " in " << func << ":" << line << " " << file << std::endl;
        flag = false;
    }
    return flag;
}

static unsigned int CompileShader(unsigned int type, const std::string &source)
{
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE)
    {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* error_message = (char*)alloca(length * sizeof(char));
        glGetShaderInfoLog(id, length, &length, error_message);
        std::cout << "Failed to compile shader" << std::endl;
        std::cout << error_message << std::endl;
        std::cout << source << std::endl;
        glDeleteShader(id);
        return 0;
    }

    return id;
}

static unsigned int CreateShader(const std::string &vertexShader, const std::string &fragmentShader)
{
    
    unsigned int program = glCreateProgram();
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}

static unsigned int LoadShader(const std::string &filepath)
{
    std::ifstream stream(filepath);

    std::stringstream vertexShader;
    std::stringstream fragmentShader;

    unsigned int mode = -1; // 0 - vertexShader; 1 - fragmentShader

    std::string line;
    while (getline(stream, line))
    {
        if (line.find("#shader") != std::string::npos)
        {
            if (line.find("vertex") != std::string::npos)
                mode = 0;
            else if (line.find("fragment") != std::string::npos)
                mode = 1; 
        }
        else if (mode == 0)
            vertexShader << line << '\n';
        else if (mode == 1)
            fragmentShader << line << '\n';

    }

    return CreateShader(vertexShader.str(), fragmentShader.str());
}

Renderer::Renderer(Engine &engine, int width, int height, const char *window_name, unsigned int buffer_size=6) 
		: System(engine, engine.ConstructSignature<Triangle>()) {

	_window = nullptr;
	assert(glfwInit() && "GLFW was not able to initialize");

	_window = glfwCreateWindow(width, height, window_name, NULL, NULL);
	if (!_window) {
		glfwTerminate();
		assert(false && "GLFW was not able to create a window");
	}
	
	_window_size = {width, height, 0};

	glfwMakeContextCurrent(_window);
	glfwSwapInterval(1);	

	assert(glewInit() == GLEW_OK && "GLEW was not able to initialize");

	unsigned int shader = LoadShader(SOURCE_DIR + "/shaders/RedTriangle.shader");
	glUseProgram(shader);

	unsigned int position = glGetUniformLocation(shader, "u_Color");
	assert(position != -1 && "u_Color uniform was not found");

	glUniform4f(position, 1.0f, 0.2f, 0.4f, 1.0f);

	float *_positions_buffer = (float *)malloc(buffer_size * sizeof(float));
	unsigned int _buffer_size = buffer_size;

	unsigned int buffer;
	GLCall(glGenBuffers(1, &buffer));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, buffer));
}

Renderer::~Renderer() {
	glfwTerminate();
	free(_positions_buffer);
}

void Renderer::Update(float dt) {
	PROFILE_FUNCTION();
	auto triangles = _engine.GetComponentList<Triangle>(_targets[0]);

	if (_buffer_size < triangles.size()*3*2) {
		_positions_buffer = (float *)realloc(_positions_buffer, triangles.size()*3*2 * sizeof(float)); 
	}
	
	float *current = _positions_buffer;
	
	for (auto *triangle : triangles) {
		for (auto &vertex : triangle->vertices) {
			*current = vertex.x / _window_size.x;
			current++;
			*current = vertex.y / _window_size.y;
			current++;
		}
	}
	
	GLCall(glBufferData(GL_ARRAY_BUFFER, triangles.size()*3*2 * sizeof(float), _positions_buffer, GL_DYNAMIC_DRAW));

	GLCall(glEnableVertexAttribArray(0));
	GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0));

	glClear(GL_COLOR_BUFFER_BIT);

	GLCall(glDrawArrays(GL_TRIANGLES, 0, triangles.size()*3));

	glfwSwapBuffers(_window);

	glfwPollEvents();
}
