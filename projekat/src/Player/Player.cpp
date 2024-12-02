#include "Player.h"


float Player::width = 0;
float Player::height = 0;
unsigned int Player::VAO = -1;
unsigned int Player::VBO = -1;
unsigned int Player::shaderProgram = -1;
unsigned int Player::textureID = -1;



Player::Player(float startX, float startY) : GameObject(startX, startY), isRight(true) {}

void Player::initializePlayerBuffers(float w, float h)
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

void Player::loadTexture(unsigned int texture)
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

void Player::loadShader(unsigned int shader)
{
    shaderProgram = shader;
    glUseProgram(shaderProgram);
    unsigned int uTexLoc = glGetUniformLocation(shaderProgram, "uTex");
    glUniform1i(uTexLoc, 0);  // Učitavanje teksture u shader
    glUseProgram(0);
}

// Destruktor
Player::~Player() {
    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO);
}


// Crta igrača na ekranu
void Player::draw() {
    glUseProgram(shaderProgram);   // Aktiviraj shader
    glBindVertexArray(VAO);        // Binduj VAO

    glActiveTexture(GL_TEXTURE0);  // Aktiviraj teksturnu jedinicu
    glBindTexture(GL_TEXTURE_2D, textureID); // Binduj teksturu

    // Postavljanje uniform vrednosti
    glUniform2f(glGetUniformLocation(shaderProgram, "uPos"), x, y);
    glUniform1i(glGetUniformLocation(shaderProgram, "uFlipX"), isRight ? GL_FALSE : GL_TRUE);

    // Crtanje pravougaonika
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    // Resetuj stanje
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindVertexArray(0);
    glUseProgram(0);
}

// Pomeranje igrača
void Player::move(float dx, float dy) {
    // Nova pozicija nakon pomeranja
    if (dx > 0)
        isRight = false;
    if (dx < 0)
        isRight = true;
    x += dx;
    y += dy;

}

// Postavljanje nove pozicije igrača
void Player::setPosition(float newX, float newY) {
    x = newX;
    y = newY;
}

// Getteri za poziciju
float Player::getX() const {
    return x;
}

float Player::getY() const {
    return y;
}

float Player::getHeight() const
{
    return height;
}

float Player::getWidth() const
{
    return width;
}

void Player::onCollision(GameObject& gameObject)
{
    std::cout << "player collided width" << typeid(gameObject).name() << std::endl;
}

