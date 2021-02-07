#include "buffer_object.h"

namespace schess::gl
{
    bufferObject::~bufferObject() { GLCALL(glDeleteBuffers(1, &m_id)); }

    void bufferObject::m_bind()   const { GLCALL(glBindBuffer(m_target, m_id)); }
    void bufferObject::m_unBind() const { GLCALL(glBindBuffer(m_target, 0));    }

    void bufferObject::m_update() const
    { 
        m_bind();  
        GLCALL(glBufferData(m_target, m_size, m_data, m_usage));
    }

    void bufferObject::m_setAndEnableVertex
    (GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, std::size_t startPoint)
    {
        GLCALL(glVertexAttribPointer(
        index, size, type, normalized, stride, reinterpret_cast<const GLvoid*>(startPoint)));
        GLCALL(glEnableVertexAttribArray(index));
    }
    
}   