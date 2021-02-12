#include "renderer.h"

namespace schess::gl
{
    texture2DRenderer::texture2DRenderer(
        const std::string_view& sFilePath, 
        const shader& _shader, 
        const texture2D::paramArr& arr, const GLenum usage)
    {
        m_create(sFilePath, _shader, arr, usage);
    }

    texture2DRenderer::texture2DRenderer()  {}
    texture2DRenderer::~texture2DRenderer() {}

    const std::string_view texture2DRenderer::s_sTextureName = "u_texture";
    
    const GLuint texture2DRenderer::s_Indices[6] = { 0, 1, 2,  1, 2, 3 }; // 2D square

    void texture2DRenderer::m_create(
        const std::string_view& sFilePath, 
        const shader& _shader, 
        const texture2D::paramArr& arr, const GLenum usage 
    )
    {
        m_vao.m_create();
        m_vao.m_bind();
    
        m_vbo.m_create(GL_ARRAY_BUFFER        , m_vertices, usage);
        m_ebo.m_create(GL_ELEMENT_ARRAY_BUFFER, s_Indices , usage);

        m_vbo.m_bind();
        m_ebo.m_bind();

        m_vbo.m_setAndEnableVertex(0, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 4, 0);
        m_vbo.m_setAndEnableVertex(1, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 4, 2 * sizeof(GLfloat));

        m_vao.m_unBind();

        ASSERT(m_texture.m_createTexture(sFilePath, s_sTextureName, _shader, arr), "cannot load image");
    }

    void texture2DRenderer::m_setBuffer(const rect<float>& screenRect, const rect<float>& textureRect)
    {
        const vec2<float> screenPos2 { screenRect.x  + screenRect.z , screenRect.y  - screenRect.w  };
        const vec2<float> texturePos2{ textureRect.x + textureRect.z, textureRect.y - textureRect.w };

        m_vertices[0] = { screenRect.x, screenRect.y };
        m_vertices[2] = { screenPos2.x, screenRect.y };
        m_vertices[4] = { screenRect.x, screenPos2.y };
        m_vertices[6] = { screenPos2.x, screenPos2.y };
        
        m_vertices[1] = { textureRect.x, textureRect.y };
        m_vertices[3] = { texturePos2.x, textureRect.y };
        m_vertices[5] = { textureRect.x, texturePos2.y };
        m_vertices[7] = { texturePos2.x, texturePos2.y };

        m_vbo.m_update();
    }


    void texture2DRenderer::m_draw() const
    { 
        m_vao.m_bind();
        m_texture.m_bind();

        GLCALL(glDrawElements(GL_TRIANGLES, 6u, GL_UNSIGNED_INT, nullptr));
    }


}