#pragma once

#include <terrakit/platform/OpenGLContext.h>

namespace Shaders {
namespace Atmosphere {

static const std::string vertex =
  std::string(terrakit::platform::OpenGLContext::GetShaderVersion()) + R"(

layout(std140) uniform Camera {
    mat4 u_View;
    mat4 u_Proj;
    vec4 u_Eye; 
};

uniform mat4 u_Model;

layout(location=0) in vec3 a_Position;
// TODO: remove later. Mesh doesn't support pos only yet
layout(location=1) in vec4 a_Color; 

out vec3 v_WorldNormal;
out vec3 v_EyeDir;

void main() {
    vec4 worldPos = u_Model * vec4(a_Position, 1.0);
    gl_Position = u_Proj * u_View * worldPos;

    v_WorldNormal = normalize(mat3(u_Model) * a_Position);
    v_EyeDir = u_Eye.xyz - worldPos.xyz;
}
)";

static const std::string fragment =
  std::string(terrakit::platform::OpenGLContext::GetShaderVersion()) + R"(

in vec3 v_WorldNormal;
in vec3 v_EyeDir;

out vec4 fragColor;

uniform vec3 u_Color; 

void main() {
    vec3 N = normalize(v_WorldNormal);
    vec3 V = normalize(v_EyeDir);

    float viewDot = dot(N, V);
    float fresnel = pow(1.0 - viewDot, 2.0);

    vec3 atmosphereColor = u_Color * fresnel;
    fragColor = vec4(atmosphereColor, fresnel * 0.01);
}
)";

}
}
