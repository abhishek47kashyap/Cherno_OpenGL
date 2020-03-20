#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Texture.h"

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
    float vertexCoordinates[] = {-0.5f, -0.5f,    // corner 0 (bottom left)
                                                0.0f,  0.0f,    // texture coordinates for corner 0
                                  0.5f, -0.5f,    // corner 1 (bottom right)
                                                1.0f,  0.0f,    // texture coordinates for corner 1
                                  0.5f,  0.5f,    // corner 2 (top right)
                                                1.0f,  1.0f,    // texture coordinates for corner 2
                                 -0.5f,  0.5f,    // corner 3 (top left)
                                                0.0f,  1.0f     // texture coordinates for corner 3
                                };

    unsigned int indices[] = {0, 1, 2,
                              2, 3, 0};   // building a square out of vertexCoordinates

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    VertexArray va;
    VertexBuffer vb(vertexCoordinates, 4 * 4 * sizeof(float));

    VertexBufferLayout layout;
    layout.Push<float>(2);
    layout.Push<float>(2);
    va.AddBuffer(vb, layout);

    // for composing a square out of 2 triangles: GL_ELEMENT_ARRAY_BUFFER
    IndexBuffer ib(indices, 6);

    std::string shaderFilepath = "resources/shaders/Basic.shader";  // working directory must be where main.cpp is
    Shader shader(shaderFilepath);
    shader.Bind();
    shader.SetUniform4f("u_Color", 0.2f, 0.3f, 0.0f, 1.0f);

    // color animation
    float blueChannel = 0.0f, increment = 0.05;

    // Textures
    Texture texture("resources/textures/Avengers_logo.png");
    texture.Bind();  // default slot is 0
    shader.SetUniform1i("u_Texture", 0);   // 0 is slot number, which should be the same as texture.Bind()

    // Unbinding
    va.Unbind();
    vb.Unbind();   // same as glBindBuffer(GL_ARRAY_BUFFER, 0);
    ib.Unbind();   // same as glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    shader.Unbind();

    // Create instance of Renderer
    Renderer renderer;

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        renderer.Clear();

        // binding every iteration
        shader.Bind();
        shader.SetUniform4f("u_Color", 0.2f, 0.3f, blueChannel, 1.0f);

        // render to screen
        renderer.Draw(va, ib, shader);

        // changing blue channel intensity
        if (blueChannel > 1.0f || blueChannel < 0.0f)
            increment = -increment;
        blueChannel += increment;

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}