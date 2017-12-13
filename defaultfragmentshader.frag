#version 330 core
uniform sampler2D qt_Texture0;
//varying vec4 qt_TexCoord0;
in vec2 TexCoord;

uniform sampler2D texture;

void main(void)
{
    //gl_FragColor = texture2D(qt_Texture0, qt_TexCoord0.st);
    //gl_FragColor = vec4(1.0, 0.5, 0.2, 1.0);
    gl_FragColor = texture2D(texture, TexCoord);
}
