#version 440 core
#pragma debug(on)
#pragma optimize(off)

layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 clr;

out vec3 color;

void main(void) 
{
	gl_Position = vec4(pos.x, pos.y, pos.z, 1.0);

	color = clr;
}
