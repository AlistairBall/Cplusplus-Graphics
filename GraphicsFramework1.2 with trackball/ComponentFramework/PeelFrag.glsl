#version 330
#define MAX_DEPTH 1.0


layout(location = 0) out vec4 vFragColor0;
layout(location = 1) out vec4 vFragColor1;
layout(location = 2) out vec4 vFragColor2;  

uniform sampler2DRect  depthBlenderTex;
uniform sampler2DRect  frontBlenderTex;

in vec4 vOutColor;
in vec3 position;
in vec3 eyeDir;
in  vec3 vertNormal;
uniform samplerCube cubeTexture; 
uniform samplerCube noiseTexture; 
uniform mat4 viewMatrix; 
uniform mat4 modelMatrix; 
out vec4 fragColor;

uniform vec4 vColor;
uniform float isObject;

void main(void)
{
float fragDepth = gl_FragCoord.z;
vec2 depthBlender = texture(depthBlenderTex, gl_FragCoord.xy).xy;
vec4 forwardTemp = texture(frontBlenderTex, gl_FragCoord.xy);



// Because over blending makes color increase or decrease,
// we cannot pass-through by default.
// Each pass, only one fragment writes a color greater than 0
vFragColor2 = vec4(0.0);

float nearestDepth = -depthBlender.x;
float farthestDepth = depthBlender.y;
float alphaMultiplier = 1.0 - forwardTemp.w;

if (fragDepth < nearestDepth || fragDepth > farthestDepth)
{
    // Skip this depth in the peeling algorithm
    vFragColor0.xy = vec2(-MAX_DEPTH);
    return;
}

if (fragDepth > nearestDepth && fragDepth < farthestDepth) 
{
    // This fragment needs to be peeled again
    vFragColor0.xy = vec2(-fragDepth, fragDepth);
    return;
}    

// If we made it here, this fragment is on the peeled layer from last pass
// therefore, we need to shade it, and make sure it is not peeled any farther
vFragColor0.xy = vec2(-MAX_DEPTH);

vec4 Color;
if(isObject == 0.0)
 Color = vColor;


if (fragDepth == nearestDepth)
{
  vFragColor1.xyz += Color.rgb * Color.a * alphaMultiplier;
      vFragColor1.w = 1.0 - alphaMultiplier * (1.0 - Color.a);
} 

else
{
    vFragColor2 += Color;
}

				  
    
    vec3 refraction = refract(eyeDir, vertNormal, 1.0/1.6);
    refraction = vec3 (inverse (viewMatrix * modelMatrix) * vec4 (refraction, 0.0));
    fragColor =  texture(cubeTexture,  refraction);

}