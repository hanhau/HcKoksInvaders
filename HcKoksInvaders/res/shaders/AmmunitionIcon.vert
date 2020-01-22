#version 330 core

layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aUV;
layout (location = 2) in float aColAmount;

uniform vec2 uPos;

out vec2 fUV;
out float fColAmount;

void main(){
	fUV = aUV;
	fColAmount = aColAmount;

	gl_Position = vec4(
		uPos.x+aPos.x,
		uPos.y+aPos.y,
		0.0,1.0
	);
}