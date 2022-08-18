#version 330 core
in vec2 TexCoord;

uniform sampler2D texture;

void main(void)
{
    vec4 color = texture2D(texture, TexCoord);
    gl_FragColor = color;
}
