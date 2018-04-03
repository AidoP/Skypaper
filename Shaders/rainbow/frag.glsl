#version 330 core

in vec4 gl_FragCoord;
out vec4 color;

uniform float time;

void main(){
  color = vec4((sin(2 * 3.141 * time / 1000000) + 1) / 2, (cos(2 * 3.141 * time / 1000000) + 1) / 2, 0, 1) * (gl_FragCoord / vec4(1920, 1080, 1, 1));
}