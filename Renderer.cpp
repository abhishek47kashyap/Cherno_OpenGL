
#include "Renderer.h"


void GLClearError()
{
    // keep flushing out pending errors (if there are any) so as to start with a clean slate
    while (glGetError() != GL_NO_ERROR);
}

bool GLLogCall()
{
    while (GLenum error = glGetError())
    {
        std::cout << "[OpenGL error] (" << error << ")" << std::endl;
        return false;
    }
    return true;
}
