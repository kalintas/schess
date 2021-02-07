#pragma once

#include "buffer_object.h"
#include "shader.h"
#include "texture.h"
#include "uniform.h"
#include "vertex_array_object.h"


namespace schess::gl
{

    // simple 2d texture renderer
    class texture2DRenderer
    {
    public:

        texture2DRenderer(
            const std::string_view& sFilePath, 
            const shader& __shader, 
            const texture2D::paramArr& arr, const GLenum usage = GL_DYNAMIC_DRAW);

        texture2DRenderer();
        ~texture2DRenderer();

        void m_create(
            const std::string_view& sFilePath, 
            const shader& __shader, 
            const texture2D::paramArr& arr, const GLenum usage = GL_DYNAMIC_DRAW);

        void m_setBuffer(const rect<float>& screenRect, const rect<float>& textureRect);

        void m_draw() const;

    private:
        // posx posy textureX textureY
        vec2<float> m_vertices[8];

        verArrObj m_vao;
        bufferObject m_vbo;
        bufferObject m_ebo;

        texture2D m_texture;

        static const GLuint s_Indices[6];
        static const std::string_view s_sTextureName;
    };


}