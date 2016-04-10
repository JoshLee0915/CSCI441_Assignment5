#version 430 core

out vec2 textCoord;

void main(void)
{
	// sides of the window
	const vec4 vertices[4] = vec4[4](
	vec4(1.0, 1.0, 0.0, 1.0),
	vec4(-1.0, 1.0, 0.0, 1.0),
	vec4(-1.0, -1.0, 0.0, 1.0),
	vec4(1.0, -1.0, 0.0, 1.0));
	
	gl_Position = vertices[gl_VertexID];
	textCoord = vertices[gl_VertexID].xy;
}