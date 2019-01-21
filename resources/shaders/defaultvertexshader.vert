#version 330 core
layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texCoord;
layout(location = 2) in float texIndex;
out vec2 TexCoord;
out float TexIndex;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;


void main(void)
{
    gl_Position = projection * view * model * vec4(position, 1.0f);
    TexCoord = texCoord;
    TexIndex = texIndex;
}
