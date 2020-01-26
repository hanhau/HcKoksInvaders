#version 430 core

in vec2 fUV;
in float fColAmount;
out vec4 res;

uniform sampler2D fTexture0;

uniform float fReloadProgress; // 0.0f - 1.0f

uniform vec3 fColor;
uniform int fGrayImage;

int imgGray = fGrayImage;

void main(){	
	if(fReloadProgress > fUV.y)
		imgGray = 0;
	else 
		imgGray = 1;

	vec4 tex = texture(fTexture0, fUV).rgba;

	float gray = dot(tex.rgb,vec3(0.299,0.587,0.114));
	vec4 tex_filtered = mix(tex,vec4(vec3(gray),tex.a),imgGray);

	vec4 col = mix(tex_filtered,vec4(fColor.xyz,1.0),fColAmount);
	
	res = col;
	fGrayImage;
}