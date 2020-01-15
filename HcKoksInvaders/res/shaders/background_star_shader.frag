#version 430 core

#define PI 3.14159265358979

#define iterations 6
#define formuparam 0.525

#define volsteps 5
#define stepsize 1.25

#define zoom   0.450
#define tile   0.850
#define speed  0.005 

#define brightness 0.01
#define darkmatter 0.500
#define distfading 0.730
#define saturation 0.550

uniform lowp float progress = 1.0f;
uniform lowp vec3 baseColor = vec3(1.0f,1.0f,1.0f);
uniform lowp vec2 iResolution = vec2(640.0f,960.0f);

out vec4 res;

void main(){
	baseColor;
	//discard;

	//get coords and direction
	lowp vec2 uv=gl_FragCoord.xy/iResolution.xy-.5;
	uv.y*=iResolution.y/iResolution.x;
	lowp vec3 dir=vec3(uv*zoom,1.);
	lowp float time=progress*speed+.25;

	//rotation
	lowp vec3 from = vec3(1.+time*2.,.5+time,-1.5);
	
	//volumetric rendering
	lowp float s=0.1,fade=1.;
	lowp vec3 v=vec3(0.);
	for (int r=0; r<volsteps; r++) {
		lowp vec3 p=from+s*dir*.5;
		p = abs(vec3(tile)-mod(p,vec3(tile*2.))); // tiling fold
		lowp float pa,a=pa=0.;
		
			p=abs(p)/dot(p,p)-formuparam; // the magic formula
			a+=abs(length(p)-pa); // absolute sum of average change
			pa=length(p);

			p=abs(p)/dot(p,p)-formuparam; // the magic formula
			a+=abs(length(p)-pa); // absolute sum of average change
			pa=length(p);

			p=abs(p)/dot(p,p)-formuparam; // the magic formula
			a+=abs(length(p)-pa); // absolute sum of average change
			pa=length(p);

			p=abs(p)/dot(p,p)-formuparam; // the magic formula
			a+=abs(length(p)-pa); // absolute sum of average change
			pa=length(p);
		
		lowp float dm=max(0.,darkmatter-a*a*.001); //dark matter
		
		v+=fade;
		v+=vec3(s,s*s,s*s*s*s)*a*brightness*fade; // coloring based on distance
		fade*=distfading; // distance fading
		s+=stepsize;
	}
	
	res = vec4(v*.01,1.);	
}