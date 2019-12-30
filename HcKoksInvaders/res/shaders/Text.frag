#version 430 

in vec3 vUV;
out vec4 res;

layout (binding = 0) uniform sampler2DArray tex;

void main(){
	res = vec4(texture(tex,vec3(vUV.xy,int(round(vUV.z)))).aaaa);
}