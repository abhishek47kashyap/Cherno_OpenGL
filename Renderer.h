
#include <iostream>
#include <GL/glew.h>
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"

#ifndef CHERNO_OPENGL_RENDERER_H
#define CHERNO_OPENGL_RENDERER_H


void GLClearError();

bool GLLogCall();


class Renderer
{
public:
    void Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;
    void Clear() const;
};


#endif //CHERNO_OPENGL_RENDERER_H
