#pragma once

#ifdef __EMSCRIPTEN__
#define SHADER_VERSION "#version 300 es\nprecision mediump float;"
#else
#define SHADER_VERSION "#version 330 core"
#endif

namespace Shaders {

namespace Object3d {

static const char* vertex =
SHADER_VERSION R"(
layout(std140) uniform Camera {
    mat4 u_View;
    mat4 u_Proj;
    vec4 u_Eye; 
};

uniform mat4 u_Model;

layout(location=0) in vec3 a_Position;
layout(location=1) in vec3 a_Normal;
layout(location=2) in vec2 a_TextCoord;

out vec3 v_WorldNormal;
out vec3 v_EyeDir;
out vec2 v_TextCoord;

void main() {
    mat4 M = u_Model;
    gl_Position   = u_Proj * u_View * M * vec4(a_Position, 1.0);

    vec3 worldPos    = vec3(M * vec4(a_Position, 1.0));
    v_WorldNormal    = normalize(mat3(transpose(inverse(M))) * a_Normal);
    v_EyeDir         = u_Eye.xyz - worldPos;
    v_TextCoord      = a_TextCoord;
}

)";

static const char* fragment =
SHADER_VERSION R"(

in vec3 v_WorldNormal;
in vec3 v_EyeDir;
in vec2 v_TextCoord;

out vec4 fragColor;

uniform sampler2D   u_Texture;
uniform vec3        u_LightDir;

void main() {
    vec3 N = normalize(v_WorldNormal);
    vec3 V = normalize(v_EyeDir);
    vec3 L = normalize(u_LightDir); 

    float NdotL     = max(dot(N, L), 0.0);
    vec3 base       = vec3(1.0, 0.9, 0.8);
    vec4 textColor  = texture(u_Texture, v_TextCoord);
    fragColor       = textColor; //* vec4(base * (0.1 + 0.9 * NdotL), 1.0);
}

)";

}
}