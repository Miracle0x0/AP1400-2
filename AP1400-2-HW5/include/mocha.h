#ifndef MOCHA_H
#define MOCHA_H

#include "espresso_based.h"
#include "ingredient.h"

class Mocha : public EspressoBased {
  public:
    Mocha();
    Mocha(const Mocha &moc);
    Mocha &operator=(const Mocha &moc);
    ~Mocha() override;

    double price() override;
    void brew() override;

    void add_side_item(Ingredient *side);
    std::vector<Ingredient *> &get_side_items();

  private:
    std::vector<Ingredient *> side_items;
};

#endif // MOCHA_H