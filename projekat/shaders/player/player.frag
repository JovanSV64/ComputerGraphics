#version 330 core

in vec2 chTex; // Koordinate teksture
out vec4 outCol;

uniform sampler2D uTex; // Tekstura

void main()
{
    outCol = texture(uTex, chTex); // Uzima boju iz teksture na datim koordinatama
    if (outCol.a < 0.1)
        discard; // Ignori�emo piksele sa niskim alfa kanalima
}
