#include "Projectile.h"
#define M_PI 3.14159265358979323846

float Projectile::width = 0;
float Projectile::height = 0;
unsigned int Projectile::VAO = 0;
unsigned int Projectile::VBO = 0;
unsigned int Projectile::textureID = 0;
unsigned int Projectile::shaderProgram = 0;

Projectile::Projectile(float startX, float startY,double mouseX, double mouseY): GameObject(startX,startY){


    directionX = static_cast<float>(mouseX) - startX;
    directionY = static_cast<float>(mouseY) - startY;

    float magnitude = sqrt(directionX * directionX + directionY * directionY);

    directionX /= magnitude;
    directionY /= magnitude;
    angle = atan2(directionX, directionY) - M_PI / 2.0f;


}

// Statička funkcija za inicijalizaciju VAO, VBO, teksture i šejdera
void Projectile::initializeProjectileBuffers(float w, float h){

    width = w;
    height = h;
    float halfWidth = width / 2.0f;
    float halfHeight = height / 2.0f;


    // Generiši VAO i VBO samo jednom
    if (VAO == 0 && VBO == 0) {


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
}

// Funkcija za učitavanje teksture
// Funkcija za učitavanje teksture
void Projectile::loadTexture(unsigned int texture, unsigned int shader) {
    textureID = texture;
    shaderProgram = shader;

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

    // Podesi shader
    glUseProgram(shader);
    unsigned int uTexLoc = glGetUniformLocation(shader, "uTex");
    glUniform1i(uTexLoc, 0);  // Učitavanje teksture u shader
    glUseProgram(0);
}


// Destruktor
Projectile::~Projectile() {
}

void Projectile::draw() {
    glUseProgram(shaderProgram);
    glBindVertexArray(VAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureID);

    // Prosledi poziciju objekta
    glUniform2f(glGetUniformLocation(shaderProgram, "uPos"), x, y);

    // Prosledi ugao rotacije u radijanima
    glUniform1f(glGetUniformLocation(shaderProgram, "uAngle"), angle);  // 'angle' je ugao u radijanima

    glUniform2f(glGetUniformLocation(shaderProgram, "uSize"), width, height);  // Širina i visina projektila

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    glBindTexture(GL_TEXTURE_2D, 0);
    glBindVertexArray(0);
    glUseProgram(0);
}

void Projectile::Update(float speed){

    x += directionX * speed;
    y += directionY * speed;
}

void Projectile::setPosition(float newX, float newY) {
    x = newX;
    y = newY;
}

void Projectile::onCollision(GameObject& gameObject)
{
    std::cout << "Projectile collided width" << typeid(gameObject).name() << std::endl;
}


float Projectile::getVAO() const {
    return VAO;
}
float Projectile::getVBO() const {
    return VBO;
}

float Projectile::getTexture() const {
    return textureID;
}
float Projectile::getShader() const {
    return shaderProgram;
}
// Getteri za poziciju
float Projectile::getX() const {
    return x;
}

float Projectile::getY() const {
    return y;
}

float Projectile::getHeight() const
{
    return height;
}

float Projectile::getWidth() const
{
    return width;
}