#version 330 core
in vec2 TexCoord;
in float TexIndex;

uniform sampler2D texture;
uniform int nowTexture;

void main(void)
{
    vec4 color = texture2D(texture, TexCoord);
    if( int(TexIndex) != nowTexture )
    {
        discard;
    }
    gl_FragColor = color;
}
