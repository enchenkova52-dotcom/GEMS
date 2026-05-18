#include "Cell.h"

Cell::Cell() : color(0), empty(true), bonusType(BonusType::None) {
}

Cell::Cell(int color) : color(color), empty(false), bonusType(BonusType::None) {
}

int Cell::getColor() const {
    return color;
}

void Cell::setColor(int color) {
    this->color = color;
    empty = false;
}

bool Cell::isEmpty() const {
    return empty;
}

void Cell::makeEmpty() {
    color = 0;
    empty = true;
    bonusType = BonusType::None;
}

BonusType Cell::getBonusType() const {
    return bonusType;
}

void Cell::setBonusType(BonusType type) {
    bonusType = type;
}

void Cell::clearBonus() {
    bonusType = BonusType::None;
}

bool Cell::hasBonus() const {
    return bonusType != BonusType::None;
}