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
  
  // check if the color is to be clamped to balck or white
  if((clr.r-minPx)/(maxPx-minPx) >= 0.5)
	clr.rgb = vec3(1.0);	// white
  else
	clr.rgb = vec3(0.0);	// black
	
  color = clr;
}