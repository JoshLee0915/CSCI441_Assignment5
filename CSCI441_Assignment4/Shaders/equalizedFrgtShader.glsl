#version 430 core
out vec4 color;
in vec2 textCoord;

uniform sampler2D txtr; 
uniform float maxPx;
uniform float minPx;

void main(void)
{
  // get the color
  vec4 clr = texture(txtr, textCoord);

  clr.rgb = vec3((clr.r-minPx)/(maxPx-minPx));
  color = clr;
}