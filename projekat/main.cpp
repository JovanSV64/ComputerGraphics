// Autor: Jovan Vučković
// Opis: 2d Projekat iz predmeta računarska grafika

#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <fstream>
#include <sstream>
#include <random>  // For random number generation

#include <GL/glew.h>   
#include <GLFW/glfw3.h>

//stb_image.h je header-only biblioteka za ucitavanje tekstura.
//Potrebno je definisati STB_IMAGE_IMPLEMENTATION prije njenog ukljucivanja
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "src/Player/Player.h"
#include "src/Projectile/Projectile.h"
#include <vector>
#include <deque>
#include "src/enemy/Enemy.h"
#include "src/background/Background.h"
#include <map>
#include "src/Text/Text.h"
#include "src/Button/Button.h"


unsigned int compileShader(GLenum type, const char* source);
unsigned int createShader(const char* vsSource, const char* fsSource);
void framebufferSizeCallback(GLFWwindow* window, int width, int height);
static unsigned loadImageToTexture(const char* filePath);
bool isWithinBounds(float newX, float newY, float width, float height);
std::pair<float, float> generateOffScreenPosition();
bool CheckCollision(GameObject& projectile, GameObject& enemy); // AABB - AABB collision
void checkCollisions();
void updateProjectiles();
void spawnEnemy();
void shootProjectile();
void resolveEnemyCollision(Enemy& e1, Enemy& e2);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);



const double TARGET_FPS = 144.0;
const double FRAME_DURATION = 1.0 / TARGET_FPS;
double mouseX, mouseY;


int wWidth = 1920;
int wHeight = 1080;

float normFactorX = 2.0f / wWidth;  // normFactorX = x piksel
float normFactorY = 2.0f / wHeight; // normFactorY = y piksel



float playerSpeedX = 200.0f * normFactorX;
float playerSpeedY = 200.0f * normFactorY;

unsigned int enemiesKilled = 0;
float initalEnemySpeed = 0.001f;
double initalEnemyCooldown = 3.0;
float baseEnemySpeed = 0.001f;
double baseEnemyCooldown = 3.0;


float projectileSpeed = 0.01f;
double projectileCooldown = 2.0;
bool pause = false;
bool quit = false;

std::random_device rd;            
std::mt19937 gen(rd());  

// generate random number for texture 0,1,2 (3 textures)
std::uniform_int_distribution<int> textureID(0, 2); 

// generate random spawn point off screen for enemies
std::uniform_real_distribution<float> enemySpawnPoint(-1.1f, 1.1f);

// Randomly choose a side (0 = top, 1 = bottom, 2 = left, 3 = right)
std::uniform_int_distribution<int> enemySpawnSide(0, 3);

std::deque<Projectile> projectiles;
std::vector<Enemy> enemies;
Player* player;

GLFWwindow* window;

Button quitButton = Button(0,0);
int main(void)
{
    if (!glfwInit())
    {
        std::cout << "GLFW biblioteka se nije učitala! :(\n";
        return 1;
    }


    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    const char wTitle[] = "[Imagine Sisyphus happy]";
    window = glfwCreateWindow(wWidth, wHeight, wTitle, NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Prozor nije napravljen! :(\n";
        glfwTerminate();
        return 2;
    }

    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK)
    {
        std::cout << "GLEW nije mogao da se učita! :'(\n";
        return 3;
    }

    /*FT_Library ft;
    if (FT_Init_FreeType(&ft))
    {
        std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
        return -1;
    }

    FT_Face face;
    if (FT_New_Face(ft, "fonts/arial.ttf", 0, &face))
    {
        std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
        return -1;
    }*/
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    unsigned int playerShader = createShader("shaders/player/player.vert", "shaders/player/player.frag");
    unsigned int projectileShader = createShader("shaders/projectile/projectile.vert", "shaders/projectile/projectile.frag");
    unsigned int enemyShader = createShader("shaders/enemy/enemy.vert", "shaders/enemy/enemy.frag");
    unsigned int backgroundShader = createShader("shaders/background/background.vert", "shaders/background/background.frag");
    unsigned int textShader = createShader("shaders/text/text.vert", "shaders/text/text.frag");
    unsigned int buttonShader = createShader("shaders/button/button.vert", "shaders/button/button.frag");


    unsigned int playerTexture = loadImageToTexture("res/balrog.png");
    unsigned int projectileTexture = loadImageToTexture("res/projectile.png");
    unsigned int enemyTexture1 = loadImageToTexture("res/enemy1.png");
    unsigned int enemyTexture2 = loadImageToTexture("res/enemy2.png");
    unsigned int enemyTexture3 = loadImageToTexture("res/enemy3.png");
    unsigned int backgroundTexture = loadImageToTexture("res/background.png");
    unsigned int quitButtonTexture = loadImageToTexture("res/quitButton.png");

    unsigned int textureID = loadImageToTexture("res/text.bmp");
    Text::initializeTextBuffers();
    Text::LoadFontDataFromCSV("res/textData.csv");
    Text::loadTexture(textureID);
    Text::loadShader(textShader);

    Button::initializeButtonBuffers(300.0f * normFactorX, 120.0f * normFactorY);
    Button::loadShader(buttonShader);
    Button::loadTexture(quitButtonTexture);




    Background::initializeBackgroundBuffers();
    Background::loadShader(backgroundShader);
    Background::loadTexture(backgroundTexture);
    Background background = Background();


    Enemy::initializeEnemyBuffers(100.0f * normFactorX, 150.0f * normFactorY);
    Enemy::loadShader(enemyShader);
    Enemy::loadTexture(enemyTexture1);
    Enemy::loadTexture(enemyTexture2);
    Enemy::loadTexture(enemyTexture3);
    
    Projectile::initializeProjectileBuffers(100.0f * normFactorX, 40.0f * normFactorY);
    Projectile::loadTexture(projectileTexture, projectileShader);

    // Kreiraj objekat Player
    Player::initializePlayerBuffers(100.0f * normFactorX, 150.0f * normFactorY);
    Player::loadShader(playerTexture);
    Player::loadTexture(playerShader);
    player = new Player();
    glEnable(GL_BLEND); // nesto za alfa kanal
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // nesto za alfa kanal

    double previousTime = glfwGetTime();
    double elapsedTimeSinceProjectile = 0.0;  // Variable to track time for projectile drawing
    double elapsedTimeSinceEnemy = 0.0;  // Variable to track time for projectile drawing


    // Definisanje promenljivih
    float lastPressTime = 0.0f;  // poslednje vreme kada je dugme pritisnuto
    float debounceTime = 0.2f;    // vreme u sekundama između dva pritiska

    glClearColor(0.0, 0.0, 0.0, 1.0);

    while (!quit)
    {
        double currentTime = glfwGetTime();
        double elapsedTime = currentTime - previousTime;
        double elapsedTimeForProjectile = currentTime - elapsedTimeSinceProjectile;  // Track time for projectile draw
        double elapsedTimeForEnemy = currentTime - elapsedTimeSinceEnemy;  // Track time for projectile draw

        //check inputs
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS && (currentTime - lastPressTime) > debounceTime) {
            pause = !pause;  // promena stanja pauze
            std::cout << pause <<std::endl;
            lastPressTime = currentTime;  // ažuriranje poslednjeg vremena pritiska
        }


        if (elapsedTime >= FRAME_DURATION and !pause) {
            //set timers for procetile, enemy, delta speed
            float pSpeedX = playerSpeedX * elapsedTime;
            float pSpeedY = playerSpeedY * elapsedTime;


            if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS && isWithinBounds(player->getX(), player->getY() + pSpeedY, player->getWidth(), player->getHeight())) {
                player->move(0.0f, pSpeedY);
            }

            if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS && isWithinBounds(player->getX(), player->getY() - pSpeedY, player->getWidth(), player->getHeight())) {
                player->move(0.0f, -pSpeedY);
            }

            if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS && isWithinBounds(player->getX() + pSpeedX, player->getY(), player->getWidth(), player->getHeight())) {
                player->move(pSpeedX, 0.0f);
            }

            if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS && isWithinBounds(player->getX() - pSpeedX, player->getY(), player->getWidth(), player->getHeight())) {
                player->move(-pSpeedX, 0.0f);
            }
            

            //create projectile
            if (elapsedTimeForProjectile >= projectileCooldown) {
                shootProjectile();
                // Resetuj vreme
                elapsedTimeSinceProjectile = currentTime;
            }

            //spawn enemy
            if (elapsedTimeForEnemy >= initalEnemyCooldown) {
                spawnEnemy();
                elapsedTimeSinceEnemy = currentTime;
            }

            checkCollisions();

            

            previousTime = currentTime;

            glClearColor(0.5, 0.5, 0.5, 1.0);
            glClear(GL_COLOR_BUFFER_BIT);

            background.draw();
            player->draw();
            updateProjectiles();
            for (auto it = enemies.begin(); it != enemies.end();) {
                it->draw();
                it->Update(player->getX(), player->getY());
                ++it;
            }

            Text::RenderText("Hello, World!", 0.0f, 0.0f, 1.0f, glm::vec3(0.0f, 0.0f, 1.0f)); // Crvena boja

            glfwSwapBuffers(window);
            glfwPollEvents();
        }

        if (elapsedTime >= FRAME_DURATION and pause) 
        {

            previousTime = currentTime;

            glClearColor(0.5, 0.5, 0.5, 1.0);
            glClear(GL_COLOR_BUFFER_BIT);

            background.draw();
            player->draw();
            for (auto it = projectiles.begin(); it != projectiles.end();) {
                it->draw();
                it++;
            }
            for (auto it = enemies.begin(); it != enemies.end();) {
                it->draw();
                ++it;
            }
            quitButton.draw();

            Text::RenderText("Hello, World!", 0.0f, 0.0f, 1.0f, glm::vec3(0.0f, 0.0f, 1.0f)); // Crvena boja

            glfwSwapBuffers(window);
            glfwPollEvents();
        }
    }

    glfwTerminate();
    return 0;
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS){
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        // Dobijanje dimenzija prozora
        int windowWidth, windowHeight;
        glfwGetWindowSize(window, &windowWidth, &windowHeight);

        // Konvertovanje koordinata miša u OpenGL normalizovane koordinate
        float xNorm = (float(xpos) / windowWidth) * 2.0f - 1.0f;
        float yNorm = 1.0f - (float(ypos) / windowHeight) * 2.0f;


        // Provera kolizije između miša i dugmeta
        if (quitButton.isMouseInside(xNorm, yNorm))
        {
            quit = true;
        }
    }
}

void shootProjectile() {
    glfwGetCursorPos(window, &mouseX, &mouseY);

    // Normalizuj mouseX i mouseY na opseg od -1 do 1
    float normalizedMouseX = (2.0f * static_cast<float>(mouseX) / wWidth) - 1.0f;
    float normalizedMouseY = 1.0f - (2.0f * static_cast<float>(mouseY) / wHeight);


    // Dodaj projektil svakih 2 sekunde
    projectiles.push_back(Projectile(
        player->getX(),
        player->getY(),
        normalizedMouseX,
        normalizedMouseY));
}

void spawnEnemy() {
    int randomSkin = textureID(gen);
    std::pair<float, float> spawnPoints = generateOffScreenPosition();
    enemies.push_back(Enemy(spawnPoints.first, spawnPoints.second, randomSkin, initalEnemySpeed));
}
std::pair<float, float> generateOffScreenPosition() {
    int side = enemySpawnSide(gen);

    float x, y;

    switch (side) {
    case 0: // Top (y = 1.1, x is random)
        x = enemySpawnPoint(gen);
        y = 1.1f;
        break;
    case 1: // Bottom (y = -0.1, x is random)
        x = enemySpawnPoint(gen);
        y = -1.1f;
        break;
    case 2: // Left (x = -0.1, y is random)
        x = -1.1f;
        y = enemySpawnPoint(gen);
        break;
    case 3: // Right (x = 1.1, y is random)
        x = 1.1f;
        y = enemySpawnPoint(gen);
        break;
    }

    return { x, y };
}

void updateEnemyAttributes() {
    // Povećava brzinu neprijatelja sa više ubistava, sa gornjom granicom
    initalEnemySpeed = baseEnemySpeed + 0.0001f * enemiesKilled; // Skaliranje brzine
    if (initalEnemySpeed > 0.01f) {
        initalEnemySpeed = 0.01f; // Maksimalna brzina
    }

    // Smanjuje cooldown neprijatelja sa više ubistava, sa donjom granicom
    initalEnemyCooldown = baseEnemyCooldown - 0.1 * enemiesKilled; // Skaliranje cooldown-a
    if (initalEnemyCooldown < 0.5) {
        initalEnemyCooldown = 0.5; // Minimalni cooldown
    }
    std::cout << "EMEMY ATTRIBUTES: " << std::endl;
    std::cout << "enemiesKilled: " << enemiesKilled << std::endl;
    std::cout << "enemySpeed: " << initalEnemySpeed << std::endl;
    std::cout << "enemyCooldown: " << initalEnemyCooldown << std::endl;
}

void updateProjectiles() {
    for (auto it = projectiles.begin(); it != projectiles.end();) {
        it->draw();
        it->Update(projectileSpeed);

        if (!isWithinBounds(it->getX() + projectileSpeed,
            it->getY(),
            it->getWidth(),
            it->getHeight())) {

            it = projectiles.erase(it);  // erase vraća sledeći validan iterator
        }
        else {
            ++it;  // Idi na sledeći projektil
        }
    }
}

unsigned int compileShader(GLenum type, const char* source)
{
    //Uzima kod u fajlu na putanji "source", kompajlira ga i vraca sejder tipa "type"
    //Citanje izvornog koda iz fajla
    std::string content = "";
    std::ifstream file(source);
    std::stringstream ss;
    if (file.is_open())
    {
        ss << file.rdbuf();
        file.close();
        std::cout << "Uspjesno procitao fajl sa putanje \"" << source << "\"!" << std::endl;
    }
    else {
        ss << "";
        std::cout << "Greska pri citanju fajla sa putanje \"" << source << "\"!" << std::endl;
    }
     std::string temp = ss.str();
     const char* sourceCode = temp.c_str(); //Izvorni kod sejdera koji citamo iz fajla na putanji "source"

    int shader = glCreateShader(type); //Napravimo prazan sejder odredjenog tipa (vertex ili fragment)
    
    int success; //Da li je kompajliranje bilo uspjesno (1 - da)
    char infoLog[512]; //Poruka o gresci (Objasnjava sta je puklo unutar sejdera)
    glShaderSource(shader, 1, &sourceCode, NULL); //Postavi izvorni kod sejdera
    glCompileShader(shader); //Kompajliraj sejder

    glGetShaderiv(shader, GL_COMPILE_STATUS, &success); //Provjeri da li je sejder uspjesno kompajliran
    if (success == GL_FALSE)
    {
        glGetShaderInfoLog(shader, 512, NULL, infoLog); //Pribavi poruku o gresci
        if (type == GL_VERTEX_SHADER)
            printf("VERTEX");
        else if (type == GL_FRAGMENT_SHADER)
            printf("FRAGMENT");
        printf(" sejder ima gresku! Greska: \n");
        printf(infoLog);
    }
    return shader;
}

bool CheckCollision(GameObject& one, GameObject& two) // AABB - AABB collision
{
    // collision x-axis?
    bool collisionX = one.getX() + one.getWidth() >= two.getX() &&
        two.getX() + two.getWidth() >= one.getX();
    // collision y-axis?
    bool collisionY = one.getY() + one.getHeight() >= two.getY() &&
        two.getY() + two.getHeight() >= one.getY();
    // collision only if on both axes
    return collisionX && collisionY;
}

void checkCollisions() {
    for (auto itE = enemies.begin(); itE != enemies.end();) {
        bool enemyErased = false; // Da li je trenutni neprijatelj obrisan.

        // Provera sudara neprijatelja sa projektilima.
        for (auto itP = projectiles.begin(); itP != projectiles.end();) {
            if (CheckCollision(*itE, *itP)) {
                itE->onCollision(*itP); // Obradi sudar neprijatelja.
                itP->onCollision(*itE); // Obradi sudar projektila.

                // Ukloni neprijatelja i ažuriraj iterator.
                itE = enemies.erase(itE);
                enemyErased = true;
                enemiesKilled += 1;

                // Ažuriraj atribute neprijatelja.
                updateEnemyAttributes();
                // Ukloni projektil i izađi iz unutrašnje petlje.
                itP = projectiles.erase(itP);
                break;
            }
            else {
                ++itP; // Pređi na sledeći projektil.
            }
        }

        // Ako je neprijatelj već obrisan, ne treba dalje proveravati.
        if (enemyErased) {
            continue;
        }

        // Provera sudara neprijatelja sa igračem.
        if (CheckCollision(*itE, *player)) {
            std::cout << "Game Over!" << std::endl;
            //exit(0); // Gasi program.
        }

        // Provera sudara neprijatelja sa drugim neprijateljima.
        for (auto itOther = enemies.begin(); itOther != enemies.end(); ++itOther) {
            // Proverite da li je drugi neprijatelj različit od trenutnog neprijatelja
            if (itE != itOther && CheckCollision(*itE, *itOther)) {
                // Ako su se sudarili, možete ih postaviti tako da se gurnu ili sprečiti preklapanje.
                resolveEnemyCollision(*itE, *itOther);
            }
        }

        ++itE; // Pređi na sledećeg neprijatelja.
    }
}


void resolveEnemyCollision(Enemy& e1, Enemy& e2) {
    // Calculate overlap in both x and y directions.
    float overlapX = (e1.getX() + e1.getWidth() / 2) - (e2.getX() + e2.getWidth() / 2);
    float overlapY = (e1.getY() + e1.getHeight() / 2) - (e2.getY() + e2.getHeight() / 2);

    // If there is an overlap, correct the position of both enemies
    if (std::abs(overlapX) < (e1.getWidth() + e2.getWidth()) / 2 &&
        std::abs(overlapY) < (e1.getHeight() + e2.getHeight()) / 2) {

        // Adjust their positions along the X or Y axis to resolve the overlap
        if (std::abs(overlapX) > std::abs(overlapY)) {
            // Move in the X direction
            if (overlapX > 0) {
                // e1 is to the right of e2, move e1 to the right
                e1.setPosition(e1.getX() + (overlapX / 2), e1.getY());
                e2.setPosition(e2.getX() - (overlapX / 2), e2.getY());
            }
            else {
                // e1 is to the left of e2, move e1 to the left
                e1.setPosition(e1.getX() - (overlapX / 2), e1.getY());
                e2.setPosition(e2.getX() + (overlapX / 2), e2.getY());
            }
        }
        else {
            // Move in the Y direction
            if (overlapY > 0) {
                // e1 is above e2, move e1 up
                e1.setPosition(e1.getX(), e1.getY() + (overlapY / 2));
                e2.setPosition(e2.getX(), e2.getY() - (overlapY / 2));
            }
            else {
                // e1 is below e2, move e1 down
                e1.setPosition(e1.getX(), e1.getY() - (overlapY / 2));
                e2.setPosition(e2.getX(), e2.getY() + (overlapY / 2));
            }
        }
    }
}
bool isWithinBounds(float newX, float newY, float width, float height) {
    float left = newX - width / 2; // Left edge
    float right = newX + width / 2; // Right edge
    float top = newY + height / 2; // Top edge
    float bottom = newY - height / 2; // Bottom edge

    // Check if the player is still within the normalized screen bounds
    return left >= -1.0f && right <= 1.0f && bottom >= -1.0f && top <= 1.0f;
}

void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
    wWidth = width;
    wHeight = height;
    normFactorX = 2.0f / wWidth;  // normFactorX = x piksel
    normFactorY = 2.0f / wHeight; // normFactorY = y piksel
}

unsigned int createShader(const char* vsSource, const char* fsSource)
{
    //Pravi objedinjeni sejder program koji se sastoji od Vertex sejdera ciji je kod na putanji vsSource

    unsigned int program; //Objedinjeni sejder
    unsigned int vertexShader; //Verteks sejder (za prostorne podatke)
    unsigned int fragmentShader; //Fragment sejder (za boje, teksture itd)

    program = glCreateProgram(); //Napravi prazan objedinjeni sejder program

    vertexShader = compileShader(GL_VERTEX_SHADER, vsSource); //Napravi i kompajliraj vertex sejder
    fragmentShader = compileShader(GL_FRAGMENT_SHADER, fsSource); //Napravi i kompajliraj fragment sejder

    //Zakaci verteks i fragment sejdere za objedinjeni program
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);

    glLinkProgram(program); //Povezi ih u jedan objedinjeni sejder program
    glValidateProgram(program); //Izvrsi provjeru novopecenog programa

    int success;
    char infoLog[512];
    glGetProgramiv(program, GL_VALIDATE_STATUS, &success); //Slicno kao za sejdere
    if (success == GL_FALSE)
    {
        glGetShaderInfoLog(program, 512, NULL, infoLog);
        std::cout << "Objedinjeni sejder ima gresku! Greska: \n";
        std::cout << infoLog << std::endl;
    }

    //Posto su kodovi sejdera u objedinjenom sejderu, oni pojedinacni programi nam ne trebaju, pa ih brisemo zarad ustede na memoriji
    glDetachShader(program, vertexShader);
    glDeleteShader(vertexShader);
    glDetachShader(program, fragmentShader);
    glDeleteShader(fragmentShader);

    return program;
}

static unsigned loadImageToTexture(const char* filePath) {
    int TextureWidth;
    int TextureHeight;
    int TextureChannels;
    unsigned char* ImageData = stbi_load(filePath, &TextureWidth, &TextureHeight, &TextureChannels, 0);
    if (ImageData != NULL)
    {
        //Slike se osnovno ucitavaju naopako pa se moraju ispraviti da budu uspravne
        stbi__vertical_flip(ImageData, TextureWidth, TextureHeight, TextureChannels);

        // Provjerava koji je format boja ucitane slike
        GLint InternalFormat = -1;
        switch (TextureChannels) {
        case 1: InternalFormat = GL_RED; break;
        case 2: InternalFormat = GL_RG; break;
        case 3: InternalFormat = GL_RGB; break;
        case 4: InternalFormat = GL_RGBA; break;
        default: InternalFormat = GL_RGB; break;
        }

        unsigned int Texture;
        glGenTextures(1, &Texture);
        glBindTexture(GL_TEXTURE_2D, Texture);
        glTexImage2D(GL_TEXTURE_2D, 0, InternalFormat, TextureWidth, TextureHeight, 0, InternalFormat, GL_UNSIGNED_BYTE, ImageData);
        glBindTexture(GL_TEXTURE_2D, 0);
        // oslobadjanje memorije zauzete sa stbi_load posto vise nije potrebna
        stbi_image_free(ImageData);
        return Texture;
    }
    else
    {
        std::cout << "Textura nije ucitana! Putanja texture: " << filePath << std::endl;
        stbi_image_free(ImageData);
        return 0;
    }
}
