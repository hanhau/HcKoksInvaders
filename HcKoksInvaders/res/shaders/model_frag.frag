#version 430

in vec2 vUV;
in vec3 vNormal;
in vec3 vPos;

out vec4 res;

layout (binding = 0) uniform sampler2D texDiffuse;
layout (binding = 1) uniform sampler2D texGlossy;

layout (binding = 4) uniform samplerCube texCubemap;

uniform mat4 matProgressCubemap;

void main(){
	vec3 I = vec4(matProgressCubemap * vec4(normalize(vPos),1.0)).xyz;
    vec3 R = reflect(I, normalize(vNormal));
	
	vec4 col = texture(texDiffuse,vUV); 
	vec4 col_ = texture(texCubemap, R).gbra;

	res = vec4(mix(col.rgb,col_.bgr,length(texture(texGlossy,vUV).rgb)),col.a);
}