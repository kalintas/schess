#version 330 core

layout (location = 0) in vec2 a_Pos;
layout (location = 1) in vec2 a_texturePos;

out vec2 v_texturePos;

void main()
{   
    gl_Position = vec4(a_Pos, 1.0f, 1.0f);
    v_texturePos = a_texturePos;

}