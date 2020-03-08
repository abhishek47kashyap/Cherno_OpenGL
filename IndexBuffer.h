
#ifndef CHERNO_OPENGL_VERTEXBUFFER_H
#define CHERNO_OPENGL_VERTEXBUFFER_H

class IndexBuffer
{
private:
    unsigned int m_RendererID;
    unsigned int m_Count;
public:
    IndexBuffer(const unsigned int* data, unsigned int count);
    ~IndexBuffer();

    void Bind() const;
    void Unbind() const;

    inline unsigned int GetCount() const { return m_Count;}
};

#endif //CHERNO_OPENGL_VERTEXBUFFER_H
