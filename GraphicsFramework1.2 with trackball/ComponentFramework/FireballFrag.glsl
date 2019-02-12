#version 330

in vec3 eyeDir;
in vec3 vertNormal;
in vec3 lightDir;
in vec3 noiseVec;

out vec4 fragColor;

uniform samplerCube cubeTexture;
uniform float elapsedTime;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;

void main(){

vec4 color1 = vec4(1.0, 0.0, 0.0, 1.0);
vec4 color2 = vec4(1.0, 0.70, 0.0, 1.0);

fragColor = mix(color1, color2, noiseVec.x);
}
