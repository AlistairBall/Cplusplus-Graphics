#version 330
in  vec4 vVertex;
in  vec4 vNormal;

out vec3 eyeDir;
out vec3 vertNormal;
out vec3 lightDir;

 
uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;
uniform mat3 normalMatrix;
uniform vec3 lightPos;

void main() {
    vertNormal = normalMatrix * vNormal.xyz; /// Rotate the normal to the correct orientation 
    eyeDir = normalize(vec3(viewMatrix * modelMatrix * vVertex)); /// Create the eye vector 
    lightDir = normalize(lightPos - eyeDir.xyz); /// Create the light direction 
    gl_Position =  projectionMatrix * viewMatrix * modelMatrix * vVertex; 
}