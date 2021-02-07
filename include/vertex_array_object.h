#pragma once

#include "utility.h"

namespace schess::gl
{

    class verArrObj
    {
    public:

        ~verArrObj();

        void m_create();

        void m_bind()   const;
        void m_unBind() const;

    private:

        GLuint m_id;
    };

}