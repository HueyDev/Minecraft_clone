#version 330 core
out vec4 FragColor;

in vec2 texCord;
in vec3 normal;
flat in float id;

uniform sampler3D tex;

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
    
    //FragColor = texture(tex, vec3(texCord.xy, id));
    //
    //FragColor = (vec4(ambient, 1.0) + vec4(diffuse, 1.0)) * FragColor;
    FragColor = vec4(0.0, 1.0, 0.0, 1.0);
    
}
