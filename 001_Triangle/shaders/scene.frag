#version 440 core
#pragma debug(on)
#pragma optimize(off)

in vec3 color;

out vec4 fragColor;

void main()
{
	fragColor = vec4(color.r, color.g, color.b, 1.0);
}
