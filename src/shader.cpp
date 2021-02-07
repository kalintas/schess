#include "shader.h"


namespace schess::gl
{
    shader::~shader() { glDeleteProgram(m_id); }


    void shader::m_bind() const
    {   
        GLCALL(glUseProgram(m_id));
    }
    void shader::m_unBind() const
    {
        GLCALL(glUseProgram(0));
    }

    bool shader::s_compileShader(const GLenum type, const std::string& sShader, GLuint& shaderId)
    {
        shaderId = glCreateShader(type);

        const GLchar* shaderSrc = sShader.c_str();

        GLCALL(glShaderSource(shaderId, 1, &shaderSrc, nullptr));
        GLCALL(glCompileShader(shaderId));

        //error checking and getting the error log
        GLint success;
        GLCALL(glGetShaderiv(shaderId, GL_COMPILE_STATUS, &success));

        if(success == GL_TRUE) return true;

        GLint errorLength;
        GLCALL(glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &errorLength));

        std::string sError;
        sError.resize(static_cast<std::size_t>(errorLength));

        GLCALL(glGetShaderInfoLog(shaderId, errorLength, &errorLength, &sError[0]));
        std::cerr << "(s_compileShader) : Cannot compile shader\n OpenGL Error = " << sError << "\n";

        return false;
    }

    bool shader::m_createShader(const std::string_view& sFilePath, const GLenum type) const
    {       
        std::string sShader;
        GLuint shaderId;

        if(!sGetTextFile(sFilePath, sShader) || !s_compileShader(type, sShader, shaderId))
        { return false; }

        GLCALL(glAttachShader(m_id, shaderId));
        GLCALL(glDeleteShader(shaderId));

        return true;
    } 

    bool shader::m_createShaders(const std::string_view& sVertexFPath, const std::string_view& sFragmentFPath)
    {   

        GLCALL(m_id = glCreateProgram());
        
        const bool result = 
        m_createShader(sVertexFPath  , GL_VERTEX_SHADER  ) && 
        m_createShader(sFragmentFPath, GL_FRAGMENT_SHADER);

        GLCALL(glLinkProgram(m_id));
        GLCALL(glValidateProgram(m_id));

        return result;
    }

}