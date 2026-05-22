#include "GameField.h"
#include "BombBonus.h"
#include "RepaintBonus.h"

#include <iostream>
#include <random>
#include <vector>
#include <cmath>
#include <algorithm>
#include <set>

GameField::GameField(int rows, int cols, int colorCount)
    : rows(rows), cols(cols), colorCount(colorCount), field(rows, std::vector<Cell>(cols)) {
}

void GameField::fillRandom() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(1, colorCount);

    for (int row = 0; row < rows; ++row) {
        for (int col = 0; col < cols; ++col) {
            field[row][col].setColor(dist(gen));
        }
    }
}

void GameField::print() const {
    for (int row = 0; row < rows; ++row) {
        for (int col = 0; col < cols; ++col) {
            if (field[row][col].isEmpty()) {
                std::cout << ". ";
            }
            else {
                std::cout << field[row][col].getColor() << ' ';
            }
        }

        std::cout << '\n';
    }
}
bool GameField::areNeighbors(Position first, Position second) const {
    int rowDiff = std::abs(first.row - second.row);
    int colDiff = std::abs(first.col - second.col);

    return rowDiff + colDiff == 1;
}

bool GameField::swapCells(Position first, Position second) {
    if (!isInside(first) || !isInside(second)) {
        return false;
    }

    if (!areNeighbors(first, second)) {
        return false;
    }

    std::swap(field[first.row][first.col], field[second.row][second.col]);
    return true;
}
std::vector<Position> GameField::findMatches() const {
    std::vector<Position> matches;

    for (int row = 0; row < rows; ++row) {
        int count = 1;

        for (int col = 1; col <= cols; ++col) {
            bool same = false;

            if (col < cols &&
                !field[row][col].isEmpty() &&
                !field[row][col - 1].isEmpty() &&
                field[row][col].getColor() == field[row][col - 1].getColor()) {
                same = true;
            }

            if (same) {
                ++count;
            }
            else {
                if (count >= 3) {
                    for (int i = col - count; i < col; ++i) {
                        matches.push_back(Position{ row, i });
                    }
                }

                count = 1;
            }
        }
    }

    for (int col = 0; col < cols; ++col) {
        int count = 1;

        for (int row = 1; row <= rows; ++row) {
            bool same = false;

            if (row < rows &&
                !field[row][col].isEmpty() &&
                !field[row - 1][col].isEmpty() &&
                field[row][col].getColor() == field[row - 1][col].getColor()) {
                same = true;
            }

            if (same) {
                ++count;
            }
            else {
                if (count >= 3) {
                    for (int i = row - count; i < row; ++i) {
                        matches.push_back(Position{ i, col });
                    }
                }

                count = 1;
            }
        }
    }
    std::set<std::pair<int, int>> uniquePositions;
    std::vector<Position> uniqueMatches;

    for (const Position& pos : matches) {
        std::pair<int, int> key(pos.row, pos.col);

        if (uniquePositions.insert(key).second) {
            uniqueMatches.push_back(pos);
        }
    }

    matches = uniqueMatches;

    return matches;
}
void GameField::removeCells(const std::vector<Position>& cells) {
    for (const Position& pos : cells) {
        int sourceColor = field[pos.row][pos.col].getColor();
        BonusType bonusType = field[pos.row][pos.col].getBonusType();

        field[pos.row][pos.col].makeEmpty();

        if (bonusType != BonusType::None) {
            activateBonus(pos, bonusType, sourceColor);
        }
        else {
            applyRandomBonus(pos, sourceColor);
        }
    }
}
void GameField::dropCells() {
    for (int col = 0; col < cols; ++col) {
        int writeRow = rows - 1;

        for (int row = rows - 1; row >= 0; --row) {
            if (!field[row][col].isEmpty()) {
                field[writeRow][col] = field[row][col];

                if (writeRow != row) {
                    field[row][col].makeEmpty();
                }

                --writeRow;
            }
        }

        for (int row = writeRow; row >= 0; --row) {
            field[row][col].makeEmpty();
        }
    }
}
void GameField::fillEmptyCells() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(1, colorCount);

    for (int row = 0; row < rows; ++row) {
        for (int col = 0; col < cols; ++col) {
            if (field[row][col].isEmpty()) {
                field[row][col].setColor(dist(gen));
            }
        }
    }
}
void GameField::processMatches() {
    std::vector<Position> matches = findMatches();

    while (!matches.empty()) {
        removeCells(matches);
        dropCells();
        fillEmptyCells();

        matches = findMatches();
    }
}
bool GameField::makeMove(Position first, Position second) {
    if (!swapCells(first, second)) {
        return false;
    }

    std::vector<Position> matches = findMatches();

    if (matches.empty()) {
        swapCells(first, second);
        return false;
    }

    processMatches();
    return true;
}

bool GameField::isInside(Position position) const {
    return position.row >= 0 &&
        position.row < rows &&
        position.col >= 0 &&
        position.col < cols;
}

int GameField::getColor(Position position) const {
    return field[position.row][position.col].getColor();
}

void GameField::setColor(Position position, int color) {
    field[position.row][position.col].setColor(color);
}

void GameField::makeEmpty(Position position) {
    field[position.row][position.col].makeEmpty();
}

int GameField::getRows() const {
    return rows;
}

int GameField::getCols() const {
    return cols;
}

void GameField::applyRandomBonus(Position source, int sourceColor) {
    std::random_device rd;
    std::mt19937 gen(rd());

    std::uniform_int_distribution<> chanceDist(1, 100);

    if (chanceDist(gen) > 20) {
        return;
    }

    std::vector<Position> targets;

    for (int row = source.row - 3; row <= source.row + 3; ++row) {
        for (int col = source.col - 3; col <= source.col + 3; ++col) {
            Position position{ row, col };

            if (!isInside(position)) {
                continue;
            }

            if (isEmpty(position)) {
                continue;
            }

            targets.push_back(position);
        }
    }

    if (targets.empty()) {
        return;
    }

    std::uniform_int_distribution<> targetDist(0, static_cast<int>(targets.size()) - 1);
    Position target = targets[targetDist(gen)];

    std::uniform_int_distribution<> bonusDist(0, 1);
    int bonusType = bonusDist(gen);

    if (bonusType == 0) {
        setBonusType(target, BonusType::Bomb);
    }
    else {
        setBonusType(target, BonusType::Repaint);
    }
}

bool GameField::isEmpty(Position position) const {
    return field[position.row][position.col].isEmpty();
}

BonusType GameField::getBonusType(Position position) const {
    return field[position.row][position.col].getBonusType();
}

void GameField::setBonusType(Position position, BonusType type) {
    field[position.row][position.col].setBonusType(type);
}
void GameField::activateBonus(Position position, BonusType type, int sourceColor) {
    if (type == BonusType::Bomb) {
        BombBonus bonus;
        bonus.apply(*this, position, sourceColor);
    }
    else if (type == BonusType::Repaint) {
        RepaintBonus bonus;
        bonus.apply(*this, position, sourceColor);
    }
}