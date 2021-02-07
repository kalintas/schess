#pragma once

#include <GL/glew.h>

#include <string>
#include <cstdio>
#include <iostream>
#include <cmath>
#include <optional>
#include <cstdlib>

#ifdef _DEBUG
    #define GLCALL(x)\
        {\
            schess::gl::ClearErrors();\
            x;\
            if(schess::gl::TellError(#x, __FILE__, __LINE__)) std::abort();\
        }

    #define ASSERT(x, msg)\
        {\
            if(!(x))\
            {\
                std::cerr << "[ASSERT FAILED](in: " << __FILE__\
                << ", at: " << __LINE__ << "){" << #x << ", (info: " << msg ")}\n";\
                std::abort();\
            }\
        }
    #define DEBUG_PRINT_VAR(x)\
        std::cout << #x << " = (" << (x) << ")\n";
#else
    #define GLCALL(x) x
    #define ASSERT(x, msg) x
    #define DEBUG_PRINT_VAR(x) 
#endif

namespace schess
{   

    std::optional<std::size_t> GetFileSize(std::FILE* file);

    bool sGetTextFile(const std::string_view& sFilePath, std::string& res);

    namespace gl
    {
        inline void ClearErrors() { while(glGetError() != GL_NO_ERROR) {} }
    
        bool TellError(const std::string_view& sFuncName, const std::string_view& sFileName, const int Line);
    }

    template<typename T>
    class vec4;

    template<typename T>
    class vec2
    {
    public:

        T x = 0;
        T y = 0;

        vec2(const T& vx, const T& vy) : x(vx), y(vy)            {}
        explicit vec2(const T& v)      : vec2<T>::vec2(v , v) {}
        vec2() {}

        inline vec2<T> operator+ (const vec2<T>& v) const { return { x + v.x, y + v.y }; }    
        inline vec2<T> operator- (const vec2<T>& v) const { return { x - v.x, y - v.y }; }
        inline vec2<T> operator* (const vec2<T>& v) const { return { x * v.x, y * v.y }; }    
        inline vec2<T> operator/ (const vec2<T>& v) const { return { x / v.x, y / v.y }; }
        
        inline vec2<T>& operator+= (const vec2<T>& v) { return *this = *this + v; }
        inline vec2<T>& operator-= (const vec2<T>& v) { return *this = *this - v; }
        inline vec2<T>& operator*= (const vec2<T>& v) { return *this = *this * v; }
        inline vec2<T>& operator/= (const vec2<T>& v) { return *this = *this / v; }

        inline vec2<T> operator* (const T& v) const { return { x * v, y * v }; }    
        inline vec2<T> operator/ (const T& v) const { return { x / v, y / v }; }
        
        inline vec2<T>& operator*= (const T& v) { return *this = *this * v; }
        inline vec2<T>& operator/= (const T& v) { return *this = *this / v; }

        inline const T& operator[] (const std::size_t i) const 
        { 
            ASSERT(i < 2, "vec2<T>::operator[] out of range"); 
            return *(reinterpret_cast<const T*>(this) + i); 
        }

        inline T& operator[] (const std::size_t i) { return const_cast<T&>(const_cast<const vec2<T>&>(*this)[i]); }

        template<typename newT>
        explicit inline operator vec2<newT>() const 
        { 
            return { static_cast<newT>(x), static_cast<newT>(y) }; 
        }

        friend inline std::ostream& operator<< (std::ostream& o, const vec2<T>& v)
        {
            o << "[vec2<T>](x = " << v.x << ", y = " << v.y <<  ")";
            return o;
        }

        inline vec2<T> m_abs() const { return { std::abs(x), std::abs(y) }; }

        inline bool m_isInRange(const vec4<T>& v) const { return x >= v.x && y >= v.y && x < v.z && y < v.w; }
        inline bool m_isInRange(const vec2<T>& v0, const vec2<T>& v1) const { return m_isInRange({ v0, v1 }); }

    };

    template<typename T>
    class vec4
    {
    public:

        T x = 0;
        T y = 0;
        T z = 0;
        T w = 0;

        vec4(const T& vx, const T& vy, const T& vz, const T& vw) : x(vx) , y(vy), z(vz), w(vw)       {}
        vec4(const T& vx, const T& vy, const T& vz)              : vec4<T>::vec4(vx  , vy  , vz  , 0   ) {}
        vec4(const T& vx, const T& vy)                           : vec4<T>::vec4(vx  , vy  , 0   , 0   ) {}
        explicit vec4(const T& v)                                : vec4<T>::vec4(v   , v   , v   , v   ) {}
        explicit vec4(const vec2<T>& v)                          : vec4<T>::vec4(v.x , v.y , 0   , 0   ) {}
        vec4(const vec2<T>& v0, const vec2<T>& v1)               : vec4<T>::vec4(v0.x, v0.y, v1.x, v1.y) {}

        vec4() {}

        inline vec4<T> operator+ (const vec4<T>& v) const { return { x + v.x, y + v.y, z + v.z, w + v.w }; }    
        inline vec4<T> operator- (const vec4<T>& v) const { return { x - v.x, y - v.y, z - v.z, w - v.w }; }
        inline vec4<T> operator* (const vec4<T>& v) const { return { x * v.x, y * v.y, z * v.z, w * v.w }; }    
        inline vec4<T> operator/ (const vec4<T>& v) const { return { x / v.x, y / v.y, z / v.z, w / v.w }; }

        inline vec4<T>& operator+= (const vec4<T>& v) { return *this = *this + v; }
        inline vec4<T>& operator-= (const vec4<T>& v) { return *this = *this - v; }
        inline vec4<T>& operator*= (const vec4<T>& v) { return *this = *this * v; }
        inline vec4<T>& operator/= (const vec4<T>& v) { return *this = *this / v; }

        inline vec4<T> operator* (const T& v) const { return { x * v, y * v, z * v, w * v }; }    
        inline vec4<T> operator/ (const T& v) const { return { x / v, y / v, z / v, w / v }; }
        
        inline vec4<T>& operator*= (const T& v) { return *this = *this * v; }
        inline vec4<T>& operator/= (const T& v) { return *this = *this / v; }

        inline const T& operator[] (const std::size_t i) const 
        { 
            ASSERT(i < 4, "vec4<T>::operator[] out of range"); 
            return *(reinterpret_cast<const T*>(this) + i); 
        }

        inline T& operator[] (const std::size_t i) { return const_cast<T&>(const_cast<const vec4<T>&>(*this)[i]); }

        template<typename newT>
        explicit inline operator vec4<newT>() const 
        { 
            return { static_cast<newT>(x), static_cast<newT>(y), static_cast<newT>(z), static_cast<newT>(w) }; 
        }

        friend inline std::ostream& operator<< (std::ostream& o, const vec4<T>& v)
        {
            o << "[vec4<T>](x = " << v.x << ", y = " << v.y << ", z = " << v.z << ", w = " << v.w << ")";
            return o;
        } 

        inline vec4<T> m_abs() const { return { std::abs(x), std::abs(y), std::abs(z), std::abs(w) }; }

    };

    template<typename T>
    using size2 = vec2<T>;

    template<typename T>
    using rect = vec4<T>;

}

/*
union   
{
    struct{ T x, y, z, w; };
    struct{ T r, g, b, a; };

    struct{ vec2<T> xy; vec2<T> zw; };
};
*/