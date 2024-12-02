#version 330 core

in vec2 TexCoord;
out vec4 FragColor;

uniform sampler2D text;  // Atlas teksture
uniform vec3 textColor;
uniform vec2 uTexOffset;
uniform vec2 uTexSize;

void main() {
    // Uzmi teksturu iz atlasnog tekstura
    vec4 texColor = texture(text, TexCoord * uTexSize + uTexOffset);

    // Ako je boja karaktera alfa > 0, prikaži karakter
    if (texColor.a < 0.1)
        discard;

    FragColor = vec4(textColor, 1.0) * texColor;
}
