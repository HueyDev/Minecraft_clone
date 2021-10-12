#version 330 core
out vec4 FragColor;

in vec2 texCord;

uniform sampler2D tex;
//uniform samplerCube tex;

void main()
{
    FragColor = texture(tex, texCord);
}