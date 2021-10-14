#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCord;
layout (location = 2) in float imageId;

uniform mat4 transform;
uniform mat4 offset;

out vec2 texCord;
flat out float id;


void main()
{
    gl_Position = transform * offset * vec4(aPos, 1.0);
    texCord = aTexCord;
    id = imageId;
}