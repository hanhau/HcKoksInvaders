#version 330

#define M_PI 3.14159265358979323846

uniform float progress = 1 ;
uniform float screenWidth = 640;
uniform float screenHeight = 960;
uniform vec3 baseColor = vec3(1.0,1.0,1.0);

float rand(vec2 c){
	return fract(sin(dot(c.xy ,vec2(12.9898,78.233))) * 43758.5453);
}

float noise(vec2 p, float freq ){
	float unit = screenWidth/freq;
	vec2 ij = floor(p/unit);
	vec2 xy = mod(p,unit)/unit;
	//xy = 3.*xy*xy-2.*xy*xy*xy;
	xy = .5*(1.-cos(M_PI*xy));
	float a = rand((ij+vec2(0.,0.)));
	float b = rand((ij+vec2(1.,0.)));
	float c = rand((ij+vec2(0.,1.)));
	float d = rand((ij+vec2(1.,1.)));
	float x1 = mix(a, b, xy.x);
	float x2 = mix(c, d, xy.x);
	return mix(x1, x2, xy.y);
}

float pNoise(vec2 p, int res){
	float persistance = .75;
	float n = 0.;
	float normK = 0.;
	float f = 32.;
	float amp = 1.;
	int iCount = 0;
	for (int i = 0; i<50; i++){
		n+=amp*noise(p, f);
		f*=2.;
		normK+=amp;
		amp*=persistance;
		if (iCount == res) break;
		iCount++;
	}
	float nf = n/normK;
	return nf*nf*nf*nf;
}

void main( void )
{
	float n = pNoise(vec2(gl_FragCoord.x,gl_FragCoord.y+progress),1024);
	float n_far = pNoise(vec2(gl_FragCoord.x+640.f,gl_FragCoord.y+progress*0.67),256);
	
	gl_FragColor = mix(vec4(baseColor*n,1.0),vec4(vec3(1.0)*n_far,1.0),0.3);
}