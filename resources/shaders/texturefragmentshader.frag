#version 330 core
in vec2 TexCoord;

uniform sampler2D texture;
uniform vec4 color;

void main(void)
{
    gl_FragColor = texture2D(texture, TexCoord) * color;
}
