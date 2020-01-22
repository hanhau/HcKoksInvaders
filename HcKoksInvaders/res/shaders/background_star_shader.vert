#version 330 core

layout (location = 0) in vec3 bsPos;

void main(){
	gl_Position = vec4(bsPos,1.0);
}