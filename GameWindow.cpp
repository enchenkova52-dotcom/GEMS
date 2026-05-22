#include "GameWindow.h"

GameWindow::GameWindow(int rows, int cols, int colorCount, int cellSize)
    : game(rows, cols, colorCount),
    window(sf::VideoMode(cols* cellSize, rows* cellSize), "GEMS"),
    cellSize(cellSize),
    hasSelected(false),
    selected{ 0, 0 } {
    game.fillRandom();
    game.processMatches();
}

sf::Color GameWindow::getColorByNumber(int color) const {
    switch (color) {
    case 1:
        return sf::Color::Red;
    case 2:
        return sf::Color::Blue;
    case 3:
        return sf::Color::Green;
    case 4:
        return sf::Color::Yellow;
    case 5:
        return sf::Color::Magenta;
    default:
        return sf::Color::White;
    }
}

Position GameWindow::getCellByMouse(int mouseX, int mouseY) const {
    Position position;
    position.row = mouseY / cellSize;
    position.col = mouseX / cellSize;
    return position;
}

void GameWindow::handleClick(Position position) {
    if (!game.isInside(position)) {
        return;
    }

    if (!hasSelected) {
        selected = position;
        hasSelected = true;
        return;
    }

    if (selected.row == position.row && selected.col == position.col) {
        hasSelected = false;
        return;
    }

    game.makeMove(selected, position);
    hasSelected = false;
}

void GameWindow::draw() {
    window.clear(sf::Color::Black);

    for (int row = 0; row < game.getRows(); ++row) {
        for (int col = 0; col < game.getCols(); ++col) {
            Position position{ row, col };

            sf::RectangleShape cell;
            cell.setSize(sf::Vector2f(cellSize - 2.0f, cellSize - 2.0f));
            cell.setPosition(col * cellSize + 1.0f, row * cellSize + 1.0f);
            cell.setFillColor(getColorByNumber(game.getColor(position)));

            if (hasSelected && selected.row == row && selected.col == col) {
                cell.setOutlineThickness(4.0f);
                cell.setOutlineColor(sf::Color::White);
            }
            else {
                cell.setOutlineThickness(1.0f);
                cell.setOutlineColor(sf::Color(60, 60, 60));
            }

            window.draw(cell);
            BonusType bonusType = game.getBonusType(position);

            if (bonusType == BonusType::Bomb) {
                sf::CircleShape bomb(cellSize / 5.0f);
                bomb.setFillColor(sf::Color::Black);
                bomb.setPosition(
                    col * cellSize + cellSize * 0.3f,
                    row * cellSize + cellSize * 0.3f
                );

                window.draw(bomb);
            }
            else if (bonusType == BonusType::Repaint) {
                sf::CircleShape diamond(cellSize / 5.0f, 4);
                diamond.setFillColor(sf::Color::White);
                diamond.setPosition(
                    col * cellSize + cellSize * 0.3f,
                    row * cellSize + cellSize * 0.3f
                );
                diamond.setRotation(45.0f);

                window.draw(diamond);
            }

        }
    }

    window.display();
}

void GameWindow::run() {
    while (window.isOpen()) {
        sf::Event event;

        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            if (event.type == sf::Event::MouseButtonPressed &&
                event.mouseButton.button == sf::Mouse::Left) {
                Position position = getCellByMouse(event.mouseButton.x, event.mouseButton.y);
                handleClick(position);
            }
        }

        draw();
    }
}