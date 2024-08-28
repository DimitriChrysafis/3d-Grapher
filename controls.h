#ifndef CONTROLS_H
#define CONTROLS_H

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

using namespace sf;

void handleMouseWheelScrolled(Event event, float &zoom) {
    zoom += event.mouseWheelScroll.delta * 10.0f;
    if (zoom < 10.0f) {
        zoom = 10.0f;
    }
}

void handleMouseButtonPressed(Event event, bool &isDragging, Vector2i &previousMousePosition, RenderWindow &window) {
    if (event.mouseButton.button == Mouse::Left) {
        isDragging = true;
        previousMousePosition = Mouse::getPosition(window);
    }
}

void handleMouseButtonReleased(Event event, bool &isDragging) {
    if (event.mouseButton.button == Mouse::Left) {
        isDragging = false;
    }
}

void handleMouseMoved(Event event, bool isDragging, float &angleX, float &angleY, Vector2i &previousMousePosition) {
    if (isDragging) {
        Vector2i mousePosition = Mouse::getPosition();
        Vector2i delta = mousePosition - previousMousePosition;
        angleX += delta.y * 0.005f;
        angleY += delta.x * 0.005f;
        previousMousePosition = mousePosition;
    }
}

void handleKeyPressed(Event event, float &sphereRadius) {
    if (event.key.code == Keyboard::Up) {
        sphereRadius += 5.0f;
    } else if (event.key.code == Keyboard::Down) {
        sphereRadius -= 5.0f;
        if (sphereRadius < 1.0f) sphereRadius = 1.0f;
    }
}

#endif
