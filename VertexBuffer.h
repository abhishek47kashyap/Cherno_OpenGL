
#ifndef CHERNO_OPENGL_VERTEXBUFFER_H
#define CHERNO_OPENGL_VERTEXBUFFER_H

class VertexBuffer
{
private:
    unsigned int m_RendererID;
public:
    VertexBuffer(const void* data, unsigned int size);
    ~VertexBuffer();

    void Bind() const;
    void Unbind() const;
};

#endif //CHERNO_OPENGL_VERTEXBUFFER_H
