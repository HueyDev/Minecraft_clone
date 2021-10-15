#version 330 core
out vec4 FragColor;

in vec2 texCord;
in vec3 normal;
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
    vec3 lightPos = vec3(2.0, 2.0, -3.0);
    vec3 facePos = vec3(0.0, 0.0, 0.0);
    vec3 lightDir = normalize(lightPos - facePos);

    vec3 lightColor = vec3(1.0, 1.0, 1.0);

    float ambientValue = 0.5;
    float diff = max(dot(normal, lightDir), 0.0);
    
    vec3 ambient = lightColor * ambientValue;
    vec3 diffuse = lightColor * diff;
    

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

    FragColor = (vec4(ambient, 1.0) + vec4(diffuse, 1.0)) * FragColor;
    
    
}
