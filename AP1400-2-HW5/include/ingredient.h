#ifndef INGREDIENT_H
#define INGREDIENT_H

#include <string>

class Ingredient {
  public:
    double get_price_unit() { return price_unit; };
    size_t get_units() { return units; };
    std::string get_name() { return name; }
    virtual double price() = 0;

    virtual ~Ingredient() = default;
    virtual Ingredient *clone() = 0;

  protected:
    Ingredient(double price_unit, size_t units)
        : price_unit(price_unit), units(units){};

    double price_unit; // price of this ingredient per unit
    size_t units;      // how many of this ingredient we will be needing
    std::string name;  // name of the ingredient
};

#endif // INGREDIENT_H