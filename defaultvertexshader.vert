#version 330 core
attribute vec4 qt_Vertex;
//attribute vec4 qt_MultiTexCoord0;
layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texCoord;
//varying vec4 qt_TexCoord0;
out vec2 TexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;


void main(void)
{
    gl_Position = projection * view * model * vec4(position, 1.0f);
   // qt_TexCoord0 = qt_MultiTexCoord0;
    TexCoord = texCoord;
}
