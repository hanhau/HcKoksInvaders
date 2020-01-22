#version 330 core
#extension GL_EXT_texture_array : enable

in vec3 vUV;
out vec4 res;

uniform sampler2DArray tex;
uniform vec3 texColor = vec3(1.0f);

void main(){
	vec3 v = vUV;
	res = vec4(texColor,texture2DArray(tex,v).r);
}