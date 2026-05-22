#pragma once

#include "Bonus.h"

class RepaintBonus : public Bonus {
public:
    void apply(GameField& field, Position target, int sourceColor) override;
};