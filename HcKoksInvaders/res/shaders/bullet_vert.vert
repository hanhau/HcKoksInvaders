#version 430

layout (location = 0) in vec3 aPos;

out vec4 vCol;

uniform mat4 view;
uniform mat4 proj;

struct BulletPosData{
	mat4 translate;
	vec4 color;
};

layout (std430,binding = 0) buffer modelMatrices
{
	BulletPosData instance[];
};


void main(){
	//gl_Position = proj * view * inst[gl_InstanceID].translate * vec4(aPos,1.0f);
	//gl_Position = iMat[gl_InstanceID] * vec4(aPos,1.0f);
	gl_Position = proj * view * instance[gl_InstanceID].translate * vec4(aPos,1.0f);
	vCol = instance[gl_InstanceID].color;
}