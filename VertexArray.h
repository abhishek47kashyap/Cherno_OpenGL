
#ifndef CHERNO_OPENGL_VERTEXARRAY_H
#define CHERNO_OPENGL_VERTEXARRAY_H

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"

class VertexArray
{
private:
    unsigned int m_RendererID = 0;
public:
    VertexArray();
    ~VertexArray();

    void AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout);

    void Bind() const;
    void Unbind() const;
};


#endif //CHERNO_OPENGL_VERTEXARRAY_H
