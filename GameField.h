#pragma once

#include <vector>
#include "Cell.h"
#include "Position.h"

class GameField {
private:
    int rows;
    int cols;
    int colorCount;
    std::vector<std::vector<Cell>> field;

public:
    GameField(int rows, int cols, int colorCount);

    void fillRandom();
    void print() const;

    bool areNeighbors(Position first, Position second) const;
    bool swapCells(Position first, Position second);
    std::vector<Position> findMatches() const;
    void removeCells(const std::vector<Position>& cells);
    void dropCells();
    void fillEmptyCells();
    void processMatches();
    bool makeMove(Position first, Position second);
    bool isInside(Position position) const;
    int getColor(Position position) const;
    void setColor(Position position, int color);
    void makeEmpty(Position position);
    int getRows() const;
    int getCols() const;
    void applyRandomBonus(Position source, int sourceColor);
    bool isEmpty(Position position) const;
    BonusType getBonusType(Position position) const;
    void setBonusType(Position position, BonusType type);
    void activateBonus(Position position, BonusType type, int sourceColor);
};