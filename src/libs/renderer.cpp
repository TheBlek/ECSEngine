#include <GL/glew.h>
#include "renderer.hpp"
#include <GLFW/glfw3.h>
#include <cassert>
#include "vectors.hpp"
#include "system.hpp"
#include "engine.hpp"
#include "config.hpp"
#include <iostream>
#include "profiler.hpp"
#include "logger.hpp"

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
        Logger::LogAdvanced("[OpenGL ERROR] %s in %s:%d %s\n", GetErrorMessage(err), func, line, file);
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
        assert(false);
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
Renderer::Renderer(Engine &engine, Vector2Int window_size, const char *window_name)
    : Renderer(engine, window_size, window_name, 6) { }

Renderer::Renderer(
        Engine &engine,
        Vector2Int window_size,
        const char *window_name,
        unsigned int buffer_size=6)
		:   System(engine, engine.ConstructSignature<Triangle>(), engine.ConstructSignature<Rectangle>()),
            _vertex_buffer(buffer_size), 
            _index_buffer(buffer_size) {

	PROFILE_FUNCTION();

	_window = nullptr;
	assert(glfwInit() && "GLFW was not able to initialize");

	_window = glfwCreateWindow(window_size.x, window_size.y, window_name, NULL, NULL);
	if (!_window) {
		glfwTerminate();
		assert(false && "GLFW was not able to create a window");
	}
	
	_window_size = window_size;

	glfwMakeContextCurrent(_window);
	glfwSwapInterval(0);	

	assert(glewInit() == GLEW_OK && "GLEW was not able to initialize");

	unsigned int shader = LoadShader(SOURCE_DIR + "/shaders/RedTriangle.shader");
	glUseProgram(shader);

	unsigned int buffer;
	GLCall(glGenBuffers(1, &buffer));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, buffer));

	GLCall(glEnableVertexAttribArray(0));
	GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), 0));
	GLCall(glEnableVertexAttribArray(1));
	GLCall(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float),(const void *)( 2 * sizeof(float) )));

	unsigned int ibo;
	GLCall(glGenBuffers(1, &ibo));
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo));
}

Renderer::~Renderer() {
	glfwTerminate();
}

void Renderer::Update(float dt) {
	PROFILE_FUNCTION();
    auto triangles = _engine.GetComponentArray<Triangle>();
    auto rectangles = _engine.GetComponentArray<Rectangle>();
    auto transforms = _engine.GetComponentArray<Transform>();

    _vertex_buffer.Reserve((triangles.size()*3 + rectangles.size()*4) * 5);
    _index_buffer.Reserve(triangles.size()*3 + rectangles.size()*6);

    _vertex_buffer.Empty();
    _index_buffer.Empty();
    auto current_index = 0u;
    
    auto bufferVertex = [&](Vector3 vertex, Vector3 color) {
        _vertex_buffer.Append(vertex.x / _window_size.x);
        _vertex_buffer.Append(vertex.y / _window_size.y);
        _vertex_buffer.Append(color.x / 255);
        _vertex_buffer.Append(color.y / 255);
        _vertex_buffer.Append(color.z / 255);
        return current_index++;	
    };
    
    for (auto entity : _targets[0]) {
        auto &triangle = triangles.GetData(entity);
        Vector3 position = transforms.HasData(entity) ? transforms.GetData(entity).position : (Vector3){0,0,0};
        for (auto &vertex : triangle.vertices) {
            _index_buffer.Append(bufferVertex(vertex + position, triangle.color));
        }
    }

    for (auto entity : _targets[1]) {
        auto &rectangle = rectangles.GetData(entity);
        Vector3 position = transforms.HasData(entity) ? transforms.GetData(entity).position : (Vector3){0,0,0};
        auto i0 = bufferVertex(rectangle.vertices[0] + position, rectangle.color);
        auto i1 = bufferVertex(rectangle.vertices[1] + position, rectangle.color);
        auto i2 = bufferVertex(rectangle.vertices[2] + position, rectangle.color);
        auto i3 = bufferVertex(rectangle.vertices[3] + position, rectangle.color);

        _index_buffer.Append(i0);
        _index_buffer.Append(i1);
        _index_buffer.Append(i2);

        _index_buffer.Append(i0);
        _index_buffer.Append(i2);
        _index_buffer.Append(i3);
    }
    GLCall(glBufferData(GL_ARRAY_BUFFER, _vertex_buffer.size * sizeof(float), _vertex_buffer.data, GL_DYNAMIC_DRAW));
    GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, _index_buffer.size * sizeof(unsigned int), _index_buffer.data, GL_DYNAMIC_DRAW)); 

    glClear(GL_COLOR_BUFFER_BIT);

    GLCall(glDrawElements(GL_TRIANGLES, triangles.size()*3 + rectangles.size()*6, GL_UNSIGNED_INT, nullptr));

    glfwSwapBuffers(_window);
    GLCall(glFlush());

    glfwPollEvents();
    
}
