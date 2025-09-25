#pragma once

#include <terrakit/platform/OpenGLContext.h>

namespace Shaders {

namespace Line {

static const std::string vertex =
  std::string(terrakit::platform::OpenGLContext::GetShaderVersion()) + R"(

layout(std140) uniform Camera {
    mat4 u_View;
    mat4 u_Proj;
    vec4 u_Eye; 
};

uniform float u_LineWidth; // in pixels
uniform vec2 u_ScreenSize;
uniform int u_Color;

vec4 decodeColor(int c) {
    float r = float(c & 0xFF) / 255.0;
    float g = float((c >> 8) & 0xFF) / 255.0;
    float b = float((c >> 16) & 0xFF) / 255.0;
    float a = float((c >> 24) & 0xFF) / 255.0;
    return vec4(r,g,b,a);
}

layout(location=0) in vec3 a_Position;     // centerline point
layout(location=1) in vec3 a_NextPosition; // next point in line
layout(location=2) in float a_Side;        // -1.0 or +1.0 for left/right

out vec4 v_Color;

void main() {
    vec4 currentClip = u_Proj * u_View * vec4(a_Position, 1.0);
    vec4 nextClip = u_Proj * u_View * vec4(a_NextPosition, 1.0);
    
    // Convert to NDC
    vec2 currentNDC = currentClip.xy / currentClip.w;
    vec2 nextNDC = nextClip.xy / nextClip.w;
    
    // Direction in screen space
    vec2 dir = normalize(nextNDC - currentNDC);
    vec2 normal = vec2(-dir.y, dir.x); // perpendicular
    
    // Line width in NDC coordinates
    vec2 offset = normal * (u_LineWidth / u_ScreenSize) * a_Side;
    
    // Apply offset
    gl_Position = vec4(currentNDC + offset, currentClip.z / currentClip.w, 1.0);

    v_Color = decodeColor(u_Color); 
}

)";

static const std::string fragment =
  std::string(terrakit::platform::OpenGLContext::GetShaderVersion()) + R"(

in vec4 v_Color;
out vec4 color;


void main() {
    color = v_Color;
}
)";

}
}