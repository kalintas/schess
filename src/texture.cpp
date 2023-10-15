#include "texture.h"

#include "../stb/stb_image.h"

namespace schess::gl
{
  
    texture2D::texture2D(
        const std::string_view& sFilePath, 
        const std::string_view& sName, 
        const gl::shader& _shader, const paramArr& arr) : m_u(glUniform1i)
    { m_createTexture(sFilePath, sName, _shader, arr); }
        
    texture2D::~texture2D() { GLCALL(glDeleteTextures(1, &m_id)); }

    void texture2D::m_bind(const GLenum texture) const 
    {   
        GLCALL(glActiveTexture(GL_TEXTURE0 + texture));
        GLCALL(glBindTexture(GL_TEXTURE_2D, m_id)); 
    }

    void texture2D::m_unBind() const { GLCALL(glBindTexture(GL_TEXTURE_2D, 0)); }

    bool texture2D::m_createTexture(
        const std::string_view& sFilePath, 
        const std::string_view& sName, 
        const gl::shader& _shader, const paramArr& arr)
    {
        stbi_set_flip_vertically_on_load(true);
        uint8_t* data = stbi_load(sFilePath.data(), &m_w, &m_h, nullptr, 4);

        if(!data) return false;

        GLCALL(glGenTextures(1, &m_id));
        m_bind();

        GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, arr[0]));
        GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, arr[1]));
        GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S    , arr[2]));
        GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T    , arr[3]));

        GLCALL(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_w, m_h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data));

        m_unBind();

        stbi_image_free(data);

        m_u.m_create(_shader, sName);
        m_u.setVec(vec4<int>{ 0 });

        return true;
    }

}