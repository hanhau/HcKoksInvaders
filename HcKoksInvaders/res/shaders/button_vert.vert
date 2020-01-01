#version 330

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aCol;

uniform mat4 translate;

out vec4 col;

void main(){
	gl_Position = translate * vec4(aPos,1.0);
	col = aCol;
}