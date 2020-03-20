
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


void Renderer::Draw(const VertexArray &va, const IndexBuffer &ib, const Shader &shader) const
{
    shader.Bind();
    va.Bind();
    ib.Bind();

    glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr);   // Square
}

void Renderer::Clear() const
{
    glClear(GL_COLOR_BUFFER_BIT);
}