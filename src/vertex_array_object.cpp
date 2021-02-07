#include "vertex_array_object.h"



namespace schess::gl
{
    verArrObj::~verArrObj() { GLCALL(glDeleteVertexArrays(1, &m_id)); }

    void verArrObj::m_create() { GLCALL(glGenVertexArrays(1, &m_id)); }

    void verArrObj::m_bind()   const { GLCALL(glBindVertexArray(m_id)); }
    void verArrObj::m_unBind() const { GLCALL(glBindVertexArray(0));    }

}