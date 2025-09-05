#pragma once

namespace Shaders {

namespace Point {

static const char* vertex = R"(
#version 330 core

layout(std140) uniform Camera {
    mat4 u_View;
    mat4 u_Proj;
    vec4 u_Eye; 
};


layout(location=0) in vec3 a_Position;
layout(location=1) in vec4 a_Color;
layout(location=2) in vec3 i_Translation;

out vec4 v_Color;

void main() {
    vec4 worldPos = vec4(a_Position + i_Translation, 1.0);
    vec4 eyePos   = u_View * worldPos;

    float dist = length(eyePos.xyz);

    const float nearDist = 20.0;
    const float farDist  = 3000.0;
    const float maxSize  = 8.0;
    const float minSize  = 3.0;

    float t = clamp((dist - nearDist) / (farDist - nearDist), 0.0, 1.0);
    gl_PointSize = mix(maxSize, minSize, t);

    gl_Position = u_Proj * eyePos;

    v_Color = a_Color;
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