#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCord;

uniform mat4 transform;
uniform mat4 offset;

out vec2 texCord;


void main()
{
    gl_Position = transform * offset * vec4(aPos, 1.0);
    texCord = aTexCord;
}