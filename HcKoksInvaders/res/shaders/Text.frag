#version 430 

in vec3 vUV;
out vec4 res;

layout (binding = 0) uniform sampler2DArray tex;

void main(){
	vec3 v = vUV;
	res = vec4(vec3(1.0),texture2DArray(tex,v).r);
}