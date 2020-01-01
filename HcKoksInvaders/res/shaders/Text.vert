#version 430 

layout (location = 0) in vec2 aPos;
layout (location = 1) in vec3 aUV;

out vec3 vUV;
uniform mat4 translate;

void main(){
	gl_Position = translate * vec4(aPos.x,aPos.y,-1.0f,1.0f);
	vUV = aUV;
}