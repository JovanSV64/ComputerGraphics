#version 330 core

layout(location = 0) in vec2 inPos;   // Ulazna pozicija
layout(location = 1) in vec2 inTex;   // Ulazne koordinate teksture

uniform vec2 uPos;                    // Pozicija objekta
uniform float uAngle;                 // Ugao rotacije objekta u radijanima
uniform vec2 uSize;                   // Širina i visina objekta

out vec2 chTex;                       // Izlazne koordinate teksture

void main()
{
    // Kreiraj matricu rotacije
    mat2 rotationMatrix = mat2(cos(uAngle), -sin(uAngle), sin(uAngle), cos(uAngle));

    // Pozicija objekta je u sredini
    vec2 offset = inPos * uSize * 10;   // Skaliraj pozicije vertiksa prema dimenzijama objekta

    // Rotiraj poziciju oko centra objekta
    vec2 rotatedPos = rotationMatrix * offset; 

    // Dodaj poziciju objekta u prostoru
    gl_Position = vec4(rotatedPos + uPos, 0.0, 1.0);   // Pozicija objekta u prostoru

    // Flipuj y-koordinate teksture (invertuj y-osu)
    chTex = vec2(inTex.x, inTex.y);
}
