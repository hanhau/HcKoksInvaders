#version 330

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aCol;

uniform mat4 translate;
uniform mat4 position;
uniform mat4 scale;

out vec4 col;

void main(){
	gl_Position = position * translate * scale * vec4(aPos,1.0);
	col = aCol;
}