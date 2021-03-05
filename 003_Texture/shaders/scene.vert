#version 440 core
#pragma debug(on)
#pragma optimize(off)

layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 clr;
layout (location = 2) in vec2 aTexCoord;

uniform mat4 mvp;

out vec3 color;
out vec2 texCoord;

void main(void) 
{
	gl_Position = mvp * vec4(pos.x, pos.y, pos.z, 1.0);

	color = clr;
	texCoord = aTexCoord;
}
