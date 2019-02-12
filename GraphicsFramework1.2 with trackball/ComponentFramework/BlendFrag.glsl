#version 330


uniform sampler3D tempTexture; 

 layout(location = 0) out vec4 vFragColor;

 void main(void)
 {
vFragColor = texture(tempTexture, gl_FragCoord.xy); 
if(vFragColor.a == 0) 
    discard;
 }