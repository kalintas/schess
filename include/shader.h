#pragma once

#include "utility.h"

namespace schess::gl
{       

    class shader
    {
    public:

        ~shader();
    
        // compile and set shaders
        bool m_createShaders(const std::string_view& sVertexFPath, const std::string_view& sFragmentFPath);

        void m_bind()   const;
        void m_unBind() const;

        inline GLint m_getUniformLocation(const std::string_view& u_name) const 
        { return glGetUniformLocation(m_id, u_name.data()); }

    private:

        static bool s_compileShader(const GLenum type, const std::string& sShader, GLuint& shaderId);  
        
        bool m_createShader(const std::string_view& sFilePath, const GLenum type) const;

        GLuint m_id;
    };

}