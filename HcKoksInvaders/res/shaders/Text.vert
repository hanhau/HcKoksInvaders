#version 430 

layout (location = 0) in vec2 aPos;
layout (location = 1) in vec3 aUV;

out vec3 vUV;

uniform mat4 translate;

void main(){
	translate;
	gl_Position = vec4(aPos.x,aPos.y,0.0,1.0);
	vUV = aUV;
}