#include "Enemy.h"


float Enemy::width = 0;
float Enemy::height = 0;
unsigned int Enemy::VAO = -1;
unsigned int Enemy::VBO = -1;
unsigned int Enemy::shaderProgram = -1;
std::vector<unsigned int> Enemy::textureIDs = {};


Enemy::Enemy(float startX, float startY, unsigned int texture, float speed) :
    GameObject(startX,startY), textureID(textureIDs[texture]), speed(speed), isRight(true) {
}


void Enemy::initializeEnemyBuffers(float w, float h)
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

void Enemy::loadTexture(unsigned int texture)
{

    textureIDs.push_back(texture);

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

void Enemy::loadShader(unsigned int shader)
{
    shaderProgram = shader;
    glUseProgram(shaderProgram);
    unsigned int uTexLoc = glGetUniformLocation(shaderProgram, "uTex");
    glUniform1i(uTexLoc, 0);  // Učitavanje teksture u shader
    glUseProgram(0);
}


void Enemy::Update(double playerX, double playerY)
{
    directionX = static_cast<float>(playerX) - x;
    directionY = static_cast<float>(playerY) - y;

    float magnitude = sqrt(directionX * directionX + directionY * directionY);

    directionX /= magnitude;
    directionY /= magnitude;

    x += directionX * speed;
    y += directionY * speed;

    // Set 'right' based on player's position relative to the enemy
    isRight = (playerX < x);
}

Enemy::~Enemy()
{
}

void Enemy::draw()
{
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

void Enemy::setPosition(float newX, float newY)
{
    x = newX;
    y = newY;
}

void Enemy::onCollision(GameObject& gameObject)
{
    std::cout << "Enemy collided width" << typeid(gameObject).name() << std::endl;
}


float Enemy::getTexture() const
{
	return textureID;
}

float Enemy::getShader() const
{
	return shaderProgram;
}

float Enemy::getVAO() const
{
	return VAO;
}

float Enemy::getVBO() const
{
	return VBO;
}

float Enemy::getX() const
{
	return x;
}

float Enemy::getY() const
{
	return y;
}

float Enemy::getHeight() const
{
	return height;
}

float Enemy::getWidth() const
{
	return width;
}
