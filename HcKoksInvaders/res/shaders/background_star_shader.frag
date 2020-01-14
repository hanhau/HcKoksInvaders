#version 330

#define PI 3.14159265358979

#define iterations 14
#define formuparam 0.125

#define volsteps 14
#define stepsize 0.25

#define zoom   0.080
#define tile   0.850
#define speed  0.010 

#define brightness 0.0015
#define darkmatter 0.500
#define distfading 0.730
#define saturation 0.550


uniform float progress = 1.0f;
uniform vec3 baseColor = vec3(1.0f,1.0f,1.0f);
uniform vec2 iResolution = vec2(640.0f,960.0f);

out vec4 res;

void main(){
	res = vec4(1.0);
	discard;

	//get coords and direction
	vec2 uv=gl_FragCoord.xy/iResolution.xy-.5;
	uv.y*=iResolution.y/iResolution.x;
	vec3 dir=vec3(uv*zoom,1.);
	float time=progress*speed*0.1+.25;

	//rotation
	vec3 from = vec3(1.+time*2.,.5+time,-1.5);
	
	//volumetric rendering
	float s=0.1,fade=1.;
	vec3 v=vec3(0.);
	for (int r=0; r<volsteps; r++) {
		lowp vec3 p=from+s*dir*.5;
		p = abs(vec3(tile)-mod(p,vec3(tile*2.))); // tiling fold
		float pa,a=pa=0.;
		for (int i=0; i<iterations; i++) { 
			p=abs(p)/dot(p,p)-formuparam; // the magic formula
			a+=abs(length(p)-pa); // absolute sum of average change
			pa=length(p);
		}
		float dm=max(0.,darkmatter-a*a*.001); //dark matter
		a*=a*a; // add contrast
		if (r>6) fade*=1.-dm; // dark matter, don't render near
		//v+=vec3(dm,dm*.5,0.);
		v+=fade;
		v+=vec3(s,s*s,s*s*s*s)*a*brightness*fade; // coloring based on distance
		fade*=distfading; // distance fading
		s+=stepsize;
	}
	v=mix(vec3(length(v)),v,saturation); //color adjust
	res = vec4(v*.01,1.);	
}