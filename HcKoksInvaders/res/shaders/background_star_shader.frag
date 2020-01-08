#version 330

#define PI 3.14159265358979

uniform float progress = 1.0f;
uniform vec3 baseColor = vec3(1.0f,1.0f,1.0f);
uniform vec2 iResolution = vec2(640.0f,960.0f);

out vec4 res;

void main(){
	float f = 0.0;

	res = vec4(f,f,f,1.0);
}