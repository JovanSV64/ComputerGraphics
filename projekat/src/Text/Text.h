#pragma once
#include <map>
#include <string>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <glm/gtc/type_ptr.hpp>

struct Character {
    float x, y;       // Početne UV koordinate
    float width, height; // Dimenzije u teksturi
    float xoffset, yoffset;
    float xadvance;
};

class Text
{
private:
	static unsigned int VAO, VBO;
    static unsigned int textureID;
    static unsigned int shaderProgram;
    static std::map<char, Character> characters;

public:
	static void initializeTextBuffers();

	static void loadTexture(unsigned int texture);
    static void loadShader(unsigned int shader);

    static void RenderText(std::string text, float x, float y, float scale, glm::vec3 color);
    static std::map<char, Character> LoadFontDataFromCSV(const std::string& fontPath);
    //static std::map<char, Character> LoadFontData(const std::string& fontPath);

};

