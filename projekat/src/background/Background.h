#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

class Background
{
private:

    static unsigned int VAO;            // Zajednički VAO
    static unsigned int VBO;            // Zajednički VBO
    static unsigned int shaderProgram;  // Shader program
    static unsigned int textureID;              // ID teksture

public:
    static void initializeBackgroundBuffers();

    static void loadTexture(unsigned int texture);
    static void loadShader(unsigned int shader);

    // Konstruktor
    Background();


    // Destruktor
    ~Background();

    // Metode za crtanje i pomeranje igrača
    void draw();
};

