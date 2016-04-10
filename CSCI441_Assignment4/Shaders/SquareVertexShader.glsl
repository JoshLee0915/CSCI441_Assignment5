#version 430 core

uniform mat4 rot;
uniform mat4 scale;
uniform mat4 viewMatrix;
uniform mat4 projection;

out vec2 textCoord;

void main(void)
{
	// sides of the cube
	const vec4 vertices[24] = vec4[24](
	vec4(1.0, 1.0, -1.0, 1.0),
	vec4(-1.0, 1.0, -1.0, 1.0),
	vec4(-1.0, 1.0, 1.0, 1.0),
	vec4(1.0, 1.0, 1.0, 1.0),

	vec4(1.0, -1.0, 1.0, 1.0),
	vec4(-1.0, -1.0, 1.0, 1.0),
	vec4(-1.0, -1.0, -1.0, 1.0),
	vec4(1.0, -1.0, -1.0, 1.0),
 
	vec4(1.0, 1.0, 1.0, 1.0),
	vec4(-1.0, 1.0, 1.0, 1.0),
	vec4(-1.0, -1.0, 1.0, 1.0),
	vec4(1.0, -1.0, 1.0, 1.0),
   
	vec4(1.0, -1.0, -1.0, 1.0),
	vec4(-1.0, -1.0, -1.0, 1.0),
	vec4(-1.0, 1.0, -1.0, 1.0),
	vec4(1.0, 1.0, -1.0, 1.0),
     
	vec4(-1.0, 1.0, 1.0, 1.0),
	vec4(-1.0, 1.0, -1.0, 1.0),
	vec4(-1.0, -1.0, -1.0, 1.0),
	vec4(-1.0, -1.0, 1.0, 1.0),
   
	vec4(1.0, 1.0, -1.0, 1.0),
	vec4(1.0, 1.0, 1.0, 1.0),
	vec4(1.0, -1.0, 1.0, 1.0),
	vec4(1.0, -1.0, -1.0, 1.0));
	
	const vec2 textureCoords[4] = vec2[4](
	vec2(1.0, 1.0),
	vec2(-1.0, 1.0),
	vec2(-1.0, -1.0),
	vec2(1.0, -1.0));
	
	mat4 modelMatrix = rot*scale;
	
	gl_Position = projection*viewMatrix*modelMatrix*vertices[gl_VertexID];
	textCoord = textureCoords[gl_VertexID%4];
}