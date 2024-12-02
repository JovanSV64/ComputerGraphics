#include "Button.h"


float Button::width = 0;
float Button::height = 0;
unsigned int Button::VAO = -1;
unsigned int Button::VBO = -1;
unsigned int Button::shaderProgram = -1;
unsigned int Button::textureID = -1;



Button::Button(float startX, float startY) :
    x(startX), y(startY) {}

void Button::initializeButtonBuffers(float w, float h)
{
    width = w;
    height = h;
    float halfWidth = width / 2.0f;
    float halfHeight = height / 2.0f;

    // Računanje koordinata na osnovu veličine projektila
    float vertices[] = {
        -halfWidth, -halfHeight, 0.0f, 0.0f,   // Donji levi ugao
        halfWidth, -halfHeight, 1.0f, 0.0f,   // Donji desni ugao
        -halfWidth,  halfHeight, 0.0f, 1.0f,   // Gornji levi ugao
        halfWidth,  halfHeight, 1.0f, 1.0f    // Gornji desni ugao
    };

    unsigned int stride = (2 + 2) * sizeof(float);

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, stride, (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Button::loadTexture(unsigned int texture)
{
    textureID = texture;

    // Poveži teksturu sa OpenGL-om
    glBindTexture(GL_TEXTURE_2D, texture);

    // Generiši mip-mape
    glGenerateMipmap(GL_TEXTURE_2D);

    // Podesi parametre teksture
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    // Očisti bind
    glBindTexture(GL_TEXTURE_2D, 0);

}

void Button::loadShader(unsigned int shader)
{
    shaderProgram = shader;
    glUseProgram(shaderProgram);
    unsigned int uTexLoc = glGetUniformLocation(shaderProgram, "uTex");
    glUniform1i(uTexLoc, 0);  // Učitavanje teksture u shader
    glUseProgram(0);
}

// Destruktor
Button::~Button() {
    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO);
}

bool Button::isMouseInside(double xpos, double ypos) {
    std::cout <<"from: " << x - width / 2 << "to" << x + width / 2 << std::endl;
    return xpos >= x-width/2 && xpos <= (x + width/2) && ypos >= y-height/2 && ypos <= (y + height/2);
}
// Crta igrača na ekranu
void Button::draw() {
    glUseProgram(shaderProgram);   // Aktiviraj shader
    glBindVertexArray(VAO);        // Binduj VAO

    glActiveTexture(GL_TEXTURE0);  // Aktiviraj teksturnu jedinicu
    glBindTexture(GL_TEXTURE_2D, textureID); // Binduj teksturu

    // Postavljanje uniform vrednosti
    glUniform2f(glGetUniformLocation(shaderProgram, "uPos"), x, y);

    // Crtanje pravougaonika
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    // Resetuj stanje
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindVertexArray(0);
    glUseProgram(0);
}

// Postavljanje nove pozicije igrača
void Button::setPosition(float newX, float newY) {
    x = newX;
    y = newY;
}

// Getteri za poziciju
float Button::getX() const {
    return x;
}

float Button::getY() const {
    return y;
}

float Button::getHeight() const
{
    return height;
}

float Button::getWidth() const
{
    return width;
}


