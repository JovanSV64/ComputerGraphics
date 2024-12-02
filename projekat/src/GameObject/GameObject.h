#pragma once

class GameObject {
protected:
    float x, y; // Pozicija objekta

public:
    // Konstruktor
    GameObject(float startX = 0, float startY = 0) : x(startX), y(startY) {}

    // Virtualni destruktor
    virtual ~GameObject() {}



    virtual float getWidth() const = 0;  // Čista virtuelna funkcija
    virtual float getHeight() const = 0; // Čista virtuelna funkcija

    // Metode za upravljanje pozicijom
    virtual void setPosition(float newX, float newY) {
        x = newX;
        y = newY;
    }

    float getX() const { return x; }
    float getY() const { return y; }
};