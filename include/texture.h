#pragma once

#include "utility.h"
#include "uniform.h"
#include "shader.h"

namespace schess::gl
{

    class texture2D
    {
    public:

        typedef GLint paramArr[4];

        texture2D(
            const std::string_view& sFilePath, 
            const std::string_view& sName, 
            const gl::shader& _shader, const paramArr& arr);
        
        texture2D() {};
        ~texture2D();

        bool m_createTexture(
            const std::string_view& sFilePath, 
            const std::string_view& sName,
            const gl::shader& _shader, const paramArr& arr);

        void m_bind(const GLenum texture = 0)   const;
        void m_unBind() const;

    private:

        int m_w = 0;
        int m_h = 0;

        GLuint m_id = 0;

        uniform1i m_u{ glUniform1i };
    };

}