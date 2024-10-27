#version 420

layout(location = 0) in vec3 vCoords;
layout(location = 1) in vec3 vColor;
layout(location = 2) in vec3 vNormal;

uniform mat4 model;
uniform mat4 rotation;
uniform mat4 viewProjection;

out vec3 vertexPos;
out vec3 vertexColor;
out vec3 vertexNormal;

void main ()
{
	vec3 vertexCoords = vec3(model * vec4(vCoords, 1.0));
	gl_Position = viewProjection * vec4(vertexCoords, 1.0);

	vertexPos = vec3(viewProjection * vec4(vertexCoords, 1.0));
	vertexColor = vColor;
	vertexNormal = normalize(vec3(rotation * vec4(vNormal, 1.0)));
}