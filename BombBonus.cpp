#include "BombBonus.h"
#include "GameField.h"

#include <vector>
#include <random>
#include <algorithm>

void BombBonus::apply(GameField& field, Position target, int sourceColor) {
    std::vector<Position> cells;

    for (int row = 0; row < field.getRows(); ++row) {
        for (int col = 0; col < field.getCols(); ++col) {
            Position pos{ row, col };

            if (!(pos.row == target.row && pos.col == target.col)) {
                cells.push_back(pos);
            }
        }
    }

    std::random_device rd;
    std::mt19937 gen(rd());
    std::shuffle(cells.begin(), cells.end(), gen);

    field.makeEmpty(target);

    int count = 1;

    for (const Position& pos : cells) {
        if (count >= 5) {
            break;
        }

        field.makeEmpty(pos);
        ++count;
    }
}