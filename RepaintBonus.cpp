#include "RepaintBonus.h"
#include "GameField.h"

#include <vector>
#include <random>
#include <algorithm>
#include <cmath>

static bool areNeighborsLocal(Position a, Position b) {
    int rowDiff = std::abs(a.row - b.row);
    int colDiff = std::abs(a.col - b.col);

    return rowDiff + colDiff == 1;
}

void RepaintBonus::apply(GameField& field, Position target, int sourceColor) {
    if (!field.isInside(target) || field.isEmpty(target)) {
        return;
    }

    field.setColor(target, sourceColor);

    std::vector<Position> candidates;

    for (int row = target.row - 3; row <= target.row + 3; ++row) {
        for (int col = target.col - 3; col <= target.col + 3; ++col) {
            Position pos{ row, col };

            if (!field.isInside(pos)) {
                continue;
            }

            if (field.isEmpty(pos)) {
                continue;
            }

            if (pos.row == target.row && pos.col == target.col) {
                continue;
            }

            if (areNeighborsLocal(pos, target)) {
                continue;
            }

            candidates.push_back(pos);
        }
    }

    std::random_device rd;
    std::mt19937 gen(rd());
    std::shuffle(candidates.begin(), candidates.end(), gen);

    int count = 0;

    for (const Position& pos : candidates) {
        if (count >= 2) {
            break;
        }

        field.setColor(pos, sourceColor);
        ++count;
    }
}