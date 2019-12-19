#version 430

#define PI 3.14159f

uniform float progress = 1.0f;
uniform vec3 baseColor = vec3(1.0f,1.0f,1.0f);
vec2 iResolution = vec2(640.0f,960.0f);

float vDrop(vec2 uv,float t)
{
    uv.x = uv.x*128.0f;						// H-Count
    float dx = fract(uv.x);
    uv.x = floor(uv.x);
    uv.y *= 0.05f;							// stretch
    float o=sin(uv.x*215.4f);				// offset
    float s=cos(uv.x*33.1f)*.3f +.7f;			// speed
    float trail = mix(95.0f,35.0f,s);			// trail length
    float yv = fract(uv.y + t*s + o) * trail;
    yv = 1.0/yv;
    yv = smoothstep(0.0f,1.0f,yv*yv);
    yv = sin(yv*PI)*(s*5.0f);
    float d2 = sin(dx*PI);
    return yv*(d2*d2);
}

void mainImage( out vec4 fragColor, in vec2 fragCoord )
{
    vec2 p = (fragCoord.xy - 0.5f * iResolution.xy) / iResolution.y;
    float d = length(p)+0.1f;
	p = vec2(atan(p.x, p.y) / PI, 2.5f / d);
    
    float t =  progress*0.01;
    vec3 col = baseColor * vDrop(p,t);
    col += vec3(0.55f,0.75f,1.225f) * vDrop(p,t+0.33f);
    col += vec3(0.45f,1.15f,0.425f) * vDrop(p,t+0.66f);
	fragColor = vec4(col*(d*d), 1.0f);
}

void main(){
	mainImage(gl_FragColor,gl_FragCoord.xy);
	progress;
	baseColor;
}