#version 330 core

layout(location = 0) in vec2 inPos;   // Ulazna pozicija
layout(location = 1) in vec2 inTex;   // Ulazne koordinate teksture

uniform vec2 uPos;                    // Pozicija objekta
uniform bool uFlipX;                  // Da li treba flipovati teksturu po X osi?

out vec2 chTex;                       // Izlazne koordinate teksture

void main()
{
    gl_Position = vec4(inPos + uPos, 0.0, 1.0);   // Pozicija objekta u prostoru

    // Ako je uFlipX true, flipujemo teksturu
    if (uFlipX) {
        chTex = vec2(1.0 - inTex.x, inTex.y);   // Flip po X osi
    } else {
        chTex = inTex;   // Bez promene
    }
}
