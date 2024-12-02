#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include "../GameObject/GameObject.h"

class Enemy: public GameObject{
private:

    bool isRight;
    static float width, height;                // size of enemy
    static float normFactorX, normFactorY;     // norm factors
    static std::vector<unsigned int> textureIDs;  // ID teksture
    static unsigned int VAO;            // Zajednički VAO
    static unsigned int VBO;            // Zajednički VBO
    static unsigned int shaderProgram;  // Shader program
    float directionX = 0.0f;
    float directionY = 0.0f;
    unsigned int textureID;              // ID teksture
    float speed;

public:
    static void initializeEnemyBuffers(float width, float height);

    static void loadTexture(unsigned int texture);
    static void loadShader(unsigned int shader);

    // Konstruktor
    Enemy(float startX, float startY, unsigned int texture, float speed);
    
    void Update(double playerX, double playerY);

    // Destruktor
    ~Enemy();

    // Metode za crtanje i pomeranje igrača
    void draw();

    void setPosition(float newX, float newY);

    void onCollision(GameObject& gameObject);
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

