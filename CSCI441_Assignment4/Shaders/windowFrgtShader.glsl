#version 430 core
out vec4 color;
in vec2 textCoord;

uniform sampler2D txtr; 
void main(void)
{
  color = texture(txtr, textCoord);
}