#version 460 core 
	
in vec3 oTransformedNormals; 
in vec3 oLightDirection; 
in vec3 oViewerVector; 

uniform vec3 uLightAmbient; 
uniform vec3 uLightDiffuse; 
uniform vec3 uLightSpecular; 

uniform vec3 uMaterialAmbient; 
uniform vec3 uMaterialDiffuse; 
uniform vec3 uMaterialSpecular; 
uniform float uMaterialShininess;  
out vec4 FragColor; 

void main(void) 
{ 
	vec3 phongADSLight; 
	
	vec3 normalizedTransformedNormals = normalize(oTransformedNormals); 
	vec3 normalizedLightDirection = normalize(oLightDirection); 
	vec3 normalizedViewerVector = normalize(oViewerVector); 
	vec3 ambientLight = uLightAmbient * uMaterialAmbient; 
	vec3 diffuseLight = uLightDiffuse * uMaterialDiffuse * max(dot(normalizedLightDirection,normalizedTransformedNormals), 0.0f); 
	vec3 reflectionVector = reflect(-normalizedLightDirection, normalizedTransformedNormals); 
	vec3 lightSpecular = uLightSpecular * uMaterialSpecular * pow(max(dot(reflectionVector, normalizedViewerVector), 0.0f), uMaterialShininess); 
	phongADSLight = ambientLight + diffuseLight + lightSpecular; 
	//FragColor = vec4(1.0f,0.0f,0.0f,1.0f); 
	FragColor = vec4(phongADSLight,1.0f); 
}
