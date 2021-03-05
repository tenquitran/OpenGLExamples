#version 440 core
#pragma debug(on)
#pragma optimize(off)

in vec3 color;
in vec2 texCoord;

uniform sampler2D ourTexture;

out vec4 fragColor;

void main()
{
	fragColor = texture(ourTexture, texCoord);

	//fragColor = vec4(color.r, color.g, color.b, 1.0);
}
