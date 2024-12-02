#version 330 core

in vec2 chTex; // Koordinate teksture
out vec4 outCol;

uniform sampler2D uTex; // Tekstura

void main()
{
    outCol = texture(uTex, chTex);
}
