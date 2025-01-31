#version 460 core 
layout (location = 0) in vec4 aPos;
layout (location = 1) in vec4 aCol;
		
out vec4 oColor; 
uniform mat4 uMVPMatrix; 

void main(void) 
{ 
	gl_Position = uMVPMatrix * aPos; 
	oColor = aCol; 
}
