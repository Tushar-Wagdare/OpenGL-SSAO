#version 460 core 

in vec3 oDiffuseLight; 
out vec4 FragColor; 

void main(void) 
{ 
	FragColor = vec4(oDiffuseLight, 1.0f);
}
