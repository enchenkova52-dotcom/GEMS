#pragma once

#include "Position.h"

class GameField;

class Bonus {
public:
    virtual ~Bonus() = default;
    virtual void apply(GameField& field, Position target, int sourceColor) = 0;
};