#pragma once

#include "Bonus.h"

class BombBonus : public Bonus {
public:
    void apply(GameField& field, Position target, int sourceColor) override;
};