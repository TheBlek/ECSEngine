#include "render.hpp"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include <cassert>
#include "config.hpp"

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


int main(void)
{

    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    if(glewInit() != GLEW_OK)
        return -1;

    std::cout << glGetString(GL_VERSION) << std::endl;

    unsigned int shader = LoadShader(SOURCE_DIR + "/shaders/RedTriangle.shader");
    GLCall(glUseProgram(shader));

    float positions[] = {
        -0.5f, -0.5f,
         0.5f,  0.5f,
         0.5f, -0.5f
       // -0.5f,  0.5f
    };

    unsigned int indices[] = {
        0, 1, 2,
        0, 1, 3
    };
	/*
	unsigned int vertex_array_object;
	GLCall(glGenVertexArrays(1, &vertex_array_object));	
	GLCall(glBindVertexArray(vertex_array_object));
	*/
    unsigned int buffer;
    GLCall(glGenBuffers(1, &buffer));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, buffer));
    GLCall(glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), positions, GL_STATIC_DRAW));

    GLCall(glEnableVertexAttribArray(0));
    GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0));
	/*
    unsigned int index_buffer;
    GLCall(glGenBuffers(1, &index_buffer));
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer));
    GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indices, GL_STATIC_DRAW));
	*/
    GLCall(int location = glGetUniformLocation(shader, "u_Color"));
    assert(location != -1);

    float r = 0.0f;
    float increment = 0.01f;
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        GLCall(glUniform4f(location, r, 0.3f, 0.8f, 1.0f));
        //GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));
		GLCall(glDrawArrays(GL_TRIANGLES, 0, 3));
        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();

        if (r > 1.0f || r < 0.0f)
            increment = -increment;
        r += increment;
    }

    glfwTerminate();
    return 0;
}
