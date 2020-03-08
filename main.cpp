#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"

struct ShaderProgramSource
{
    std::string VertexSource;
    std::string FragmentSource;
};

static ShaderProgramSource ParseShader(const std::string& filepath)
{
    std::ifstream shaderFile(filepath);   // opens the file

    if (!shaderFile)
        std::cerr << "Shader file " << filepath << " could not be opened" << std::endl;
    if (shaderFile.peek() == std::ifstream::traits_type::eof())
        std::cerr << "Shader file is empty" << std::endl;

    enum class ShaderType
    {
        NONE = -1, VERTEX = 0, FRAGMENT = 1
    };

    std::string line;
    std::stringstream ss[2];
    ShaderType shaderType = ShaderType::NONE;
    while (getline(shaderFile, line))
    {
        if (line.find("#shader") != std::string::npos)   // if shader is found (npos is invalid string position)
        {
            if (line.find("vertex") != std::string::npos)
                shaderType = ShaderType::VERTEX;
            else if (line.find("fragment") != std::string::npos)
                shaderType = ShaderType::FRAGMENT;
        }
        else
            ss[(int)shaderType] << line << "\n";
    }

    return {ss[0].str(), ss[1].str()};
}

static unsigned int CompileShader(unsigned int type, const std::string& source)
{
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    // Error handling
    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE)
    {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)alloca(length * sizeof(char));
        glGetShaderInfoLog(id, length, &length, message);
        std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") <<  " shader!" << std::endl;
        std::cout << message << std::endl;
        glDeleteShader(id);
        return 0;
    }

    return id;
}

static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
    unsigned int program = glCreateProgram();
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    glAttachShader(program, vs);   // attach vertex shader
    glAttachShader(program, fs);   // attach fragment shader
    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}


int main() {
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    // these 3 lines are needed for the renders to show up on my system
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "OpenGL render", nullptr, nullptr);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    glfwSwapInterval(1); // modifying refresh rate

    glewExperimental = GL_TRUE;   // without this, glGenVertexArrays() causes memory issues

    if (glewInit() != GLEW_OK)
        std::cout << "Error linking glewInit()!" << std::endl;
    else
        std::cout << "GLEW linked alright" << std::endl;

    std::cout << "GL version: " << glGetString(GL_VERSION) << std::endl;

    if (glGenVertexArrays == nullptr)
        std::cerr << "Bad luck, glGenVertexArrays() returned NULL" << std::endl;


    GLuint vertexArrayID;
    glGenVertexArrays(1, &vertexArrayID);
    glBindVertexArray(vertexArrayID);

    // creating buffer for triangles vertices
    float vertexCoordinates[] = {-0.5f, -0.5f,    // corner 0
                                  0.5f, -0.5f,    // corner 1
                                  0.5f,  0.5f,    // corner 2
                                 -0.5f,  0.5f     // corner 3
                                };

    unsigned int indices[] = {0, 1, 2,
                              2, 3, 0};   // building a square out of vertexCoordinates

    // Vertex Array Object (VAO) == bind Vertex buffer and setting up layout
    unsigned int vertexArrayObj;
    glGenVertexArrays(1, &vertexArrayObj);
    glBindVertexArray(vertexArrayObj);

    // for a triangle: GL_ARRAY_BUFFER
    VertexBuffer vb(vertexCoordinates, 4 * 2 * sizeof(float));

    // Specifying layout
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float)*2, nullptr);  // links the buffer with the VAO

    // for composing a square out of 2 triangles: GL_ELEMENT_ARRAY_BUFFER
    IndexBuffer ib(indices, 6);

    std::string shaderFilepath = "resources/shaders/Basic.shader";  // working directory must be where main.cpp is
    ShaderProgramSource shaderSource = ParseShader(shaderFilepath);

    unsigned int shader = CreateShader(shaderSource.VertexSource, shaderSource.FragmentSource);
    glUseProgram(shader);

    int location = glGetUniformLocation(shader, "u_Color");
    if (location == -1)
        std::cerr << "Uniform could not be found (or may be there's an unused uniform)" << std::endl;
    glUniform4f(location, 0.2f, 0.3f, 0.0f, 1.0f);

    // color animation
    float blueChannel = 0.0f, increment = 0.05;

    // Unbinding
    glBindVertexArray(0);
    glUseProgram(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        // binding every iteration
        glUseProgram(shader);
        glUniform4f(location, 0.2f, 0.3f, blueChannel, 1.0f);

        glBindVertexArray(vertexArrayObj);
        ib.Bind();

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);   // Square

        // changing blue channel intensity
        if (blueChannel > 1.0f || blueChannel < 0.0f)
            increment = -increment;
        blueChannel += increment;

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glDeleteProgram(shader);

    glfwTerminate();
    return 0;
}