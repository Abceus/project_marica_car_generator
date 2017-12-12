attribute vec4 qt_Vertex;
attribute vec4 qt_MultiTexCoord0;
varying vec4 qt_TexCoord0;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;


void main(void)
{
    gl_Position = projection * view * model * qt_Vertex;
    qt_TexCoord0 = qt_MultiTexCoord0;
}
