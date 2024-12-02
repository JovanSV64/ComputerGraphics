#version 330 core

layout(location = 0) in vec2 inPos;   // Ulazna pozicija
layout(location = 1) in vec2 inTex;   // Ulazne koordinate teksture

out vec2 chTex;                       // Izlazne koordinate teksture

void main()
{
    gl_Position = vec4(inPos, 0.0, 1.0);   // Pozicija objekta u prostoru
    chTex = inTex;
}
