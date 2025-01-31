#version 460 core 
		
layout (location = 0) in vec4 aPos;
layout (location = 1) in vec4 aCol;
layout (location = 2) in vec4 aTex;
layout (location = 3) in vec4 aNorm;

uniform mat4 uModelViewMatrix; 
uniform mat4 uProjectionMatrix; 
uniform vec3 uld; 
uniform vec3 ukd; 
uniform vec4 uLightPosition;  

out vec3 oDiffuseLight;

void main(void) 
{ 
	
	vec4 iPosition = uModelViewMatrix * aPos; 
	mat3 normalMatrix = mat3(transpose(inverse(uModelViewMatrix))); 
	vec3 n = normalize(normalMatrix * vec3(aNorm)); 
	vec3 s = normalize(vec3(uLightPosition - iPosition)); 
	oDiffuseLight = uld * ukd * dot(s,n); 

	gl_Position = uProjectionMatrix * uModelViewMatrix * aPos; 
}