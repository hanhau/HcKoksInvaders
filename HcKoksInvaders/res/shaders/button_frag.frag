#version 330

in vec4 col;
uniform float time;

void main(){
	float x = (gl_FragCoord.y+gl_FragCoord.x+time*100)*0.03;
	vec4 rainbow = vec4(cos(tan(x)),abs(cos(x+2.094)),abs(cos(x-2.094)),1.0);

	gl_FragColor = mix(rainbow,col,col.a);
}