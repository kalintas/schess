#version 330

uniform sampler2D u_texture;

in vec2 v_texturePos;

uniform bool u_bInverseTextureColors;

void main()
{
    vec4 textureColor = texture(u_texture, v_texturePos);

    if(u_bInverseTextureColors && textureColor.w < 0.1f) // background color
    {
        gl_FragColor = vec4(1.0f, 0.0f, 0.0f, 0.5f);        
    }
    else gl_FragColor = textureColor;

}