#include "Text.h"
#include <iostream>
#include <fstream>
#include <sstream>

unsigned int Text::VAO = -1;
unsigned int Text::VBO = -1;
unsigned int Text::shaderProgram = -1;
unsigned int Text::textureID = -1;
std::map<char, Character> Text::characters;


std::map<char, Character> Text::LoadFontDataFromCSV(const std::string& fontPath) {
    std::map<char, Character> characters;
    std::ifstream file(fontPath);

    if (!file.is_open()) {
        std::cerr << "Failed to open font file: " << fontPath << std::endl;
        return characters;
    }

    std::string line;
    std::map<int, float> baseWidthMap; // Za "Char Base Width" podatke
    int imageWidth, imageHeight, cellWidth, cellHeight;
    int startChar, fontHeight, fontWidth;
    int charID;
    float charWidth, charHeight;
    float xoffset, yoffset, xadvance;

    // Čitanje inicijalnih podataka
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string key;
        std::string value;
        std::getline(iss, key, ','); // Delimo na ključ i vrednost
        std::getline(iss, value);

        if (key == "Image Width") {
            imageWidth = std::stoi(value);
        }
        else if (key == "Image Height") {
            imageHeight = std::stoi(value);
        }
        else if (key == "Cell Width") {
            cellWidth = std::stoi(value);
        }
        else if (key == "Cell Height") {
            cellHeight = std::stoi(value);
        }
        else if (key == "Start Char") {
            startChar = std::stoi(value);
        }
        else if (key == "Font Height") {
            fontHeight = std::stoi(value);
        }
        else if (key == "Font Width (0 is default)") {
            fontWidth = std::stoi(value);
        }
        // Ovdje možeš dodati i ostale inicijalne podatke
    }

    // Učitavanje širina za svaki karakter
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string key;
        float width;
        std::getline(iss, key, ',');
        if (key.substr(0, 10) == "Char Base") {
            std::string charNumStr = key.substr(5, key.length() - 15); // Izvuci broj char ID
            int charID = std::stoi(charNumStr);
            std::getline(iss, key, ','); // Skip the "Base Width" part
            width = std::stof(key);
            baseWidthMap[charID] = width;
        }
    }

    // Učitavanje podataka za karaktere (Char ID, X, Y, Width, Height itd.)
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string key;
        std::getline(iss, key, ','); // Ovdje čitamo samo ključ i vrednost

        if (key.substr(0, 5) == "Char ") {
            charID = std::stoi(key.substr(5)); // Uzimamo ID karaktera iz "Char X"

            // Pretpostavljamo da su podaci za karakter u sledećem formatu:
            Character ch;
            while (std::getline(iss, key, ',')) {
                if (key.substr(0, 2) == "x=") {
                    ch.x = std::stof(key.substr(2));
                }
                else if (key.substr(0, 2) == "y=") {
                    ch.y = std::stof(key.substr(2));
                }
                else if (key.substr(0, 6) == "width=") {
                    ch.width = std::stof(key.substr(6));
                }
                else if (key.substr(0, 7) == "height=") {
                    ch.height = std::stof(key.substr(7));
                }
                else if (key.substr(0, 8) == "xoffset=") {
                    ch.xoffset = std::stof(key.substr(8));
                }
                else if (key.substr(0, 8) == "yoffset=") {
                    ch.yoffset = std::stof(key.substr(8));
                }
                else if (key.substr(0, 9) == "xadvance=") {
                    ch.xadvance = std::stof(key.substr(9));
                }
            }

            // Ako imamo "Char Base Width", postavi širinu karaktera na osnovu tog podatka
            if (baseWidthMap.find(charID) != baseWidthMap.end()) {
                ch.width = baseWidthMap[charID];
            }

            // Dodaj karakter u mapu
            characters[charID] = ch;
        }
    }

    file.close();
    return characters;
}

//std::map<char, Character> Text::LoadFontData(const std::string& fontPath) {
//    std::map<char, Character> characters;
//    std::ifstream file(fontPath);
//    if (!file.is_open()) {
//        std::cerr << "Failed to open font file: " << fontPath << std::endl;
//        return characters;
//    }
//
//    std::string line;
//    while (std::getline(file, line)) {
//        if (line.substr(0, 5) == "char ") {
//            std::istringstream iss(line);
//            std::string key;
//            Character ch;
//            char id;
//
//            while (iss >> key) {
//                if (key.substr(0, 3) == "id=") id = std::stoi(key.substr(3));
//                else if (key.substr(0, 2) == "x=") ch.x = std::stof(key.substr(2));
//                else if (key.substr(0, 2) == "y=") ch.y = std::stof(key.substr(2));
//                else if (key.substr(0, 6) == "width=") ch.width = std::stof(key.substr(6));
//                else if (key.substr(0, 7) == "height=") ch.height = std::stof(key.substr(7));
//                else if (key.substr(0, 8) == "xoffset=") ch.xoffset = std::stof(key.substr(8));
//                else if (key.substr(0, 8) == "yoffset=") ch.yoffset = std::stof(key.substr(8));
//                else if (key.substr(0, 9) == "xadvance=") ch.xadvance = std::stof(key.substr(9));
//            }
//
//            characters[id] = ch;
//        }
//    }
//
//    file.close();
//    return characters;
//}

void Text::initializeTextBuffers() {
    // Kreiraj VAO i VBO samo jednom
    if (VAO == -1 && VBO == -1) {
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);

        // Računanje koordinata na osnovu veličine projektila
        float vertices[] = {
            -0.5, -0.5, 0.0f, 0.0f,   // Donji levi ugao
             0.5, -0.5, 1.0f, 0.0f,   // Donji desni ugao
            -0.5,  0.5, 0.0f, 1.0f,   // Gornji levi ugao
             0.5,  0.5, 1.0f, 1.0f    // Gornji desni ugao
        };

        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);

        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
        glEnableVertexAttribArray(1);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }
}

void Text::loadTexture(unsigned int texture) {
    textureID = texture;
    glBindTexture(GL_TEXTURE_2D, textureID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, 0); // Unbindovanje teksture
}

void Text::loadShader(unsigned int shader) {
    shaderProgram = shader;
    glUseProgram(shaderProgram);

    // Postavljanje uniform promenljivih
    unsigned int uTexLoc = glGetUniformLocation(shaderProgram, "uTex");
    glUniform1i(uTexLoc, 0);  // Tekstura tekstura je na GL_TEXTURE0

    unsigned int uColorLoc = glGetUniformLocation(shaderProgram, "textColor");
    glUniform3f(uColorLoc, 1.0f, 1.0f, 1.0f);  // Postavljanje boje teksta na belu (1, 1, 1)

    glUseProgram(0);  // Završavanje rada sa šader programom
}
void Text::RenderText(std::string text, float x, float y, float scale, glm::vec3 color) {
    glUseProgram(shaderProgram); // Aktiviraj šader program
    glBindVertexArray(VAO);
    glBindTexture(GL_TEXTURE_2D, textureID);

    // Postavi uniform za boju teksta
    glUniform3fv(glGetUniformLocation(shaderProgram, "textColor"), 1, &color[0]);

    for (char c : text) {
        Character ch = characters[c];

        // Izračunaj poziciju i veličinu karaktera
        float xpos = x + ch.xoffset * scale;
        float ypos = y - (ch.height - ch.yoffset) * scale;
        float w = ch.width * scale;
        float h = ch.height * scale;

        // Postavi unforme za poziciju i veličinu karaktera
        glUniform2f(glGetUniformLocation(shaderProgram, "uPos"), xpos, ypos);
        glUniform2f(glGetUniformLocation(shaderProgram, "uSize"), w, h);
        glUniform1f(glGetUniformLocation(shaderProgram, "uAngle"), 0.0f); // Tekst nije rotiran, ali možeš dodati rotaciju

        // Tekstura karaktera (koristi se za atlas teksture)
        glUniform2f(glGetUniformLocation(shaderProgram, "uTexOffset"), ch.x / 512.0f, ch.y / 512.0f);  // Offset u atlasu
        glUniform2f(glGetUniformLocation(shaderProgram, "uTexSize"), ch.width / 512.0f, ch.height / 512.0f); // Veličina karaktera u atlasu

        // Renderuj karakter (6 vertiksa za GL_TRIANGLE_STRIP)
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

        // Pomeri x poziciju za sledeći karakter
        x += ch.xadvance * scale;
    }

    glBindVertexArray(0);
    glUseProgram(0);
}