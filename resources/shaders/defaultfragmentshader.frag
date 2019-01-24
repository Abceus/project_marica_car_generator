#version 330 core
in vec2 TexCoord;
flat in uint TexIndex;

uniform sampler2D texture;
uniform uint nowTexture;

void main(void)
{
    vec4 color = texture2D(texture, TexCoord);
    if( TexIndex != nowTexture )
    {
        discard;
    }
    gl_FragColor = color;
}
