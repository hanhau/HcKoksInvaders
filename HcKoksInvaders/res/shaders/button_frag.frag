#version 430 core

in vec4 col;
out vec4 res;

uniform float time;

void main(){
	float x = (gl_FragCoord.y+gl_FragCoord.x+time*100)*0.03;
	vec4 rainbow = vec4(abs(cos(x)),abs(cos(x+2.094)),abs(cos(x-2.094)),1.0);

	res = mix(rainbow,col,col.a);
	//res = vec4(1);
}