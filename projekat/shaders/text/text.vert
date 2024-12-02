#version 330 core

layout(location = 0) in vec2 aPos;  // Pozicija
layout(location = 1) in vec2 aTexCoord; // Koordinate teksture

out vec2 TexCoord;  // Koordinate teksture koje šaljemo fragment shaderu

uniform vec2 uPos;
uniform vec2 uSize;
uniform float uAngle;

void main() {
    // Rotacija (ako je potrebno)
    mat2 rotation = mat2(cos(uAngle), -sin(uAngle), sin(uAngle), cos(uAngle));

    vec2 rotatedPos = rotation * aPos;

    // Krajnja pozicija
    gl_Position = vec4(rotatedPos * uSize + uPos, 0.0, 1.0);

    // Koordinate teksture
    TexCoord = aTexCoord;
}
