#version 420

in vec3 vertexPos;
in vec3 vertexColor;
in vec3 vertexNormal;

out vec4 FragColor;

uniform vec3 lightCoords; 
uniform vec3 ambientColor;
uniform vec3 diffuseColor;
uniform vec3 bgColor;
uniform float time;

void main()
{
    float distFromPlayer = length(vertexPos);
    float timedOffset = (sin(time) + 0.4 *sin(time * 3.14159) + 0.2 * sin(time * 1.57079)) * 0.005;
    if (distFromPlayer > (1.1 + timedOffset))
    {
        if (distFromPlayer < (1.102 + timedOffset)) {
            FragColor = vec4(bgColor*5.0, 1.0);
        }
        else {
            //FragColor = vec4(bgColor, 1.0);
            FragColor = vec4(vertexColor/4.0 + bgColor/2.0  , 1.0);
        }
    }
    else 
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
}