#version 330

uniform sampler2D u_texture;

in vec2 v_texturePos;

uniform bool u_bInverseTextureColors;

void main()
{
    vec4 textureColor = texture(u_texture, v_texturePos);

    if(u_bInverseTextureColors) gl_FragColor = vec4(1.0f - textureColor.xyz, 1.0f);
    else gl_FragColor = textureColor;

}