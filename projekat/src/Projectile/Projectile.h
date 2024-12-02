#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "../GameObject/GameObject.h"

class Projectile: public GameObject{
private:

    static float width, height;        // size of projectile
    static float normFactorX, normFactorY; // norm factors
    static unsigned int textureID;     // ID teksture
    static unsigned int VAO;    // Zajednički VAO
    static unsigned int VBO;    // Zajednički VBO
    static unsigned int shaderProgram; // Shader program
    float directionX;
    float directionY;
    float angle;

public:
    static void initializeProjectileBuffers(float width, float height);

    static void loadTexture(unsigned int texture, unsigned int shader);

    // Konstruktor
    Projectile(float startX, float startY,double mouseX,double mouseY);
    void Update(float speed);
    // Destruktor
    ~Projectile();

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

