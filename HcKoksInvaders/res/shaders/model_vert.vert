#version 430

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aUV;

out vec3 vNormal;
out vec3 vPos;
out vec2 vUV;

uniform mat4 matView;
uniform mat4 matProjection;

layout (std430,binding = 0) buffer modelMatrices
{
mat4 model[];
};

void main() {	
	vNormal = mat3(transpose(inverse(model[gl_InstanceID]))) * aNormal;
	vUV = aUV;
	vPos = aPos;

	gl_Position = matProjection * matView * model[gl_InstanceID] * vec4(aPos,1.0);
}