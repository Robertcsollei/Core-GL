#pragma once



namespace Shaders {

namespace Point {

    static const char* vertex = R"(
#version 330 core


out vec4 v_Color;

uniform mat4 u_MVP;
uniform vec3 u_Pivot;
uniform float u_PointSize;
uniform vec4 u_Color;

void main()
{
gl_Position = u_MVP * vec4(u_Pivot, 1.0);
gl_PointSize = u_PointSize;
v_Color = u_Color;
}
)";

    static const char* fragment = R"(
#version 330 core

in vec4 v_Color;

out vec4 color;

void main() {

    vec2 d = gl_PointCoord - vec2(0.5);
    if (dot(d, d) > 0.25) discard; // make it circular
    color = v_Color;
}
)";

}
}