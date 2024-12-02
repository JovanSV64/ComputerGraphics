#include "Background.h"

unsigned int Background::VAO = -1;
unsigned int Background::VBO = -1;
unsigned int Background::shaderProgram = -1;
unsigned int Background::textureID = -1;

void Background::initializeBackgroundBuffers()
{

    // Računanje koordinata na osnovu veličine projektila
    float vertices[] = {
        -1, -1, 0.0f, 0.0f,   // Donji levi ugao
         1, -1, 1.0f, 0.0f,   // Donji desni ugao
        -1,  1, 0.0f, 1.0f,   // Gornji levi ugao
         1,  1, 1.0f, 1.0f    // Gornji desni ugao
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

void Background::loadTexture(unsigned int texture)
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

void Background::loadShader(unsigned int shader)
{
    shaderProgram = shader;
    glUseProgram(shaderProgram);
    unsigned int uTexLoc = glGetUniformLocation(shaderProgram, "uTex");
    glUniform1i(uTexLoc, 0);  // Učitavanje teksture u shader
    glUseProgram(0);
}

Background::Background()
{
}

Background::~Background()
{
    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO);
}

void Background::draw()
{
    glUseProgram(shaderProgram);
    glBindVertexArray(VAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindVertexArray(0);
    glUseProgram(0);
}
