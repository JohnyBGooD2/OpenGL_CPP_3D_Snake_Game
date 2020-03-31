#version 330

layout (location = 0) in vec3 pos; // All layouts in mesh.cpp
layout (location = 1) in vec2 tex;

out vec4 vCol;
out vec2 TexCoord;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;
uniform vec4 colorForPickUp;

void main()
{
	gl_Position = projection * view * model * vec4(pos, 1.0);
	if (colorForPickUp[3] == 0.0)
	{
	    vCol = vec4(clamp(pos.y, 0.2f, 1.0f), 0.2f, 0.2f, 1.0f);      //vCol = vec4(clamp(pos, 0.0f, 1.0f), 1.0f);
	}
	else
	{
	    vCol = vec4(0.2f, clamp(pos.y, 0.2f, 1.0f), 0.2f, 1.0f);
	}
	
	
	TexCoord = tex;
}