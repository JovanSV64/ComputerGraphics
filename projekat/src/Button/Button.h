#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>

class Button{
private:

    static float width, height;        // size of projectile
    static float normFactorX, normFactorY; // norm factors
    static unsigned int textureID;     // ID teksture
    static unsigned int VAO;    // Zajednički VAO
    static unsigned int VBO;    // Zajednički VBO
    static unsigned int shaderProgram; // Shader program
    float x, y;        // size of projectile

public:
    Button(float startX, float startY);

    static void initializeButtonBuffers(float width, float height);

    static void loadTexture(unsigned int texture);
    static void loadShader(unsigned int shader);
    bool isMouseInside(double xpos, double ypos);

    // Konstruktor
    // Destruktor
    ~Button();
    void onClick();

    // Metode za crtanje i pomeranje igrača
    void draw();

    void setPosition(float newX, float newY);

    // Getteri za poziciju
    float getTexture() const;
    float getShader() const;
    float getVAO() const;
    float getVBO() const;
    float getX() const;
    float getY() const;
    float getHeight() const;
    float getWidth() const;
};
