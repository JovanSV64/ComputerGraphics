#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "../GameObject/GameObject.h"

class Player : public GameObject {
private:
    bool isRight;
    static float width, height;        // Dimenzije igrača
    static float normFactorX, normFactorY; // Normalizacioni faktori za veličinu
    static unsigned int textureID;     // ID teksture
    static unsigned int VAO, VBO;      // OpenGL objekti za crtanje
    static unsigned int shaderProgram; // Shader program

    // Metoda za ažuriranje vertiksa u OpenGL VBO

public:
    // Constructor
    Player(float startX = 0, float startY = 0);

    static void initializePlayerBuffers(float w, float h);

    static void loadTexture(unsigned int texture);
    static void loadShader( unsigned int shader);
    // Destruktor
    ~Player();

    // Metode za crtanje i pomeranje igrača
    void draw();
    void move(float dx, float dy);
    void setPosition(float newX, float newY);

    // Getteri za poziciju
    float getX() const;
    float getY() const;
    float getHeight() const;
    float getWidth() const;
    void onCollision(GameObject& gameObject);
};

