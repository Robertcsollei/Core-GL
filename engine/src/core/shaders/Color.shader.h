#pragma once

#include <terrakit/platform/OpenGLContext.h>

namespace Shaders {

namespace Color {

static const std::string vertex =
  std::string(terrakit::platform::OpenGLContext::GetShaderVersion()) + R"(

layout(std140) uniform Camera {
    mat4 u_View;
    mat4 u_Proj;
    vec4 u_Eye; 
};


layout(location=0) in vec3 a_Position;
layout(location=1) in vec4 a_Color;

out vec4 v_Color;

void main() {
    vec4 worldPos = vec4(a_Position, 1.0);
    vec4 eyePos   = u_View * worldPos;

    gl_Position = u_Proj * eyePos;
    v_Color = a_Color;
}

)";

static const std::string fragment =
  std::string(terrakit::platform::OpenGLContext::GetShaderVersion()) + R"(


out vec4 color;

uniform vec3 u_Color; 

void main() {
    color = u_Color;
}
)";

}
}