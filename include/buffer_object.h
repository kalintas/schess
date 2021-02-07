#pragma once

#include "utility.h"

namespace schess::gl
{

    class bufferObject
    {
    public:
        ~bufferObject();

        template<typename T>
        void m_create(const GLenum target, const T& buffer, const GLenum usage)
        {
            m_target = target;
            m_size = static_cast<GLsizeiptr>(sizeof(buffer));
            m_data = static_cast<const GLvoid*>(buffer);
            m_usage = usage;

            GLCALL(glGenBuffers(1, &m_id));
            m_update();
            m_unBind();
        }

        void m_update() const;

        void m_bind()   const;
        void m_unBind() const;

        void m_setAndEnableVertex
        (GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, std::size_t startPoint);

    private:

        GLenum m_target;
        const GLvoid* m_data;
        GLsizeiptr m_size;
        GLenum m_usage;

        GLuint m_id;
    };

}