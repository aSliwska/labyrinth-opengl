#version 420

in vec3 vertexColor;
in vec3 vertexNormal;

out vec4 FragColor;

uniform vec3 lightCoords; 
uniform vec3 ambientColor;
uniform vec3 diffuseColor;

void main()
{
    // ambient
    float ambientStrength = 1.0;
    vec3 ambient = ambientStrength * ambientColor;
  	
    // diffuse 
    vec3 lightDir = normalize(lightCoords);
    float diff = max(dot(vertexNormal, lightDir), 0.0);
    vec3 diffuse = diff * diffuseColor;
    
    vec3 result = (ambient + diffuse) * vertexColor;
    FragColor = vec4(result, 1.0);
}