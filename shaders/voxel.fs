#version 330 core
out vec4 FragColor;

in vec2 texCord;
flat in float id;

uniform sampler2D top;
uniform sampler2D bottom;
uniform sampler2D right;
uniform sampler2D left;
uniform sampler2D back;
uniform sampler2D front;
//uniform samplerCube tex;

void main()
{
    if(id == 0.0){
        FragColor = texture(top, texCord);
    }
    else if(id == 1.0){
        FragColor = texture(bottom, texCord);
    }
    else if(id == 2.0){
        FragColor = texture(right, texCord);
    }
    else if(id == 3.0){
        FragColor = texture(left, texCord);
    }
    else if(id == 4.0){
        FragColor = texture(back, texCord);
    }
    else if(id == 5.0){
        FragColor = texture(front, texCord);
    }
    else{
        FragColor = vec4(0.0, 0.0, 0.0, 0.0);
        discard;
    }
    
    
}