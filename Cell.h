#pragma once

enum class BonusType {
    None,
    Bomb,
    Repaint
};

class Cell {
private:
    int color;
    bool empty;
    BonusType bonusType;

public:
    Cell();
    explicit Cell(int color);

    int getColor() const;
    void setColor(int color);

    bool isEmpty() const;
    void makeEmpty();

    BonusType getBonusType() const;
    void setBonusType(BonusType type);
    void clearBonus();
    bool hasBonus() const;
};