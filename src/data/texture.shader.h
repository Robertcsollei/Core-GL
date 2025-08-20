#pragma once



namespace Shaders {

	namespace Texture {

		static const char* vertex = R"(
#version 330 core

layout (location = 0) in vec4 position;
layout (location = 1) in vec4 color;
layout (location = 2) in vec2 textCoord;

out vec4 v_Color;
out vec2 v_TextCoord;

uniform mat4 u_MVP;

void main()
{
	gl_Position = u_MVP * position;
	v_Color = color;
    v_TextCoord = textCoord;
}
)";

		static const char* fragment = R"(
#version 330 core

layout (location = 0) out vec4 color;

in vec4 v_Color;
in vec2 v_TextCoord;

uniform sampler2D u_Texture;

void main()
{
	vec4 textColor = texture(u_Texture, v_TextCoord);
    color = textColor * v_Color;
}
)";

	}
}