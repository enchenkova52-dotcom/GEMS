#pragma once

#include <SFML/Graphics.hpp>
#include "GameField.h"

class GameWindow {
private:
    GameField game;
    sf::RenderWindow window;

    int cellSize;
    bool hasSelected;
    Position selected;

    sf::Color getColorByNumber(int color) const;
    Position getCellByMouse(int mouseX, int mouseY) const;

    void handleClick(Position position);
    void draw();

public:
    GameWindow(int rows, int cols, int colorCount, int cellSize);
    void run();
};