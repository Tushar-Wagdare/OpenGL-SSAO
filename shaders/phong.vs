#version 460 core 
		
layout (location = 0) in vec4 aPos;
layout (location = 1) in vec4 aCol;
layout (location = 2) in vec4 aTex;
layout (location = 3) in vec4 aNorm;

uniform mat4 uModelMatrix; 
uniform mat4 uViewMatrix; 
uniform mat4 uProjectionMatrix; 
uniform vec4 uLightPosition;  

out vec3 oTransformedNormals; 
out vec3 oLightDirection; 
out vec3 oViewerVector; 


void main(void) 
{ 
	
	vec4 iCordinates = uViewMatrix * uModelMatrix * aPos; 
	oTransformedNormals = vec3(mat3(uViewMatrix * uModelMatrix) * vec3(aNorm)); 
	oLightDirection = vec3(uLightPosition - iCordinates); 
	oViewerVector = -iCordinates.xyz; 
		
	gl_Position = uProjectionMatrix * uViewMatrix * uModelMatrix * aPos; 
}
