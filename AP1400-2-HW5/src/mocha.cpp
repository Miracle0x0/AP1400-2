#include "mocha.h"

#include "sub_ingredients.h"
#include <iostream>

// Constructors

/// @brief A mocha needs 2 units of espresso, 2 units of milk, and 1 unit of
/// milk foam and 1 unit of chocolate.
Mocha::Mocha() : EspressoBased("Mocha") {
    ingredients.push_back(new Espresso(2));
    ingredients.push_back(new Milk(2));
    ingredients.push_back(new MilkFoam(1));
    ingredients.push_back(new Chocolate(1));
}

Mocha::Mocha(const Mocha &moc) : EspressoBased(moc) {
    for (auto &i : moc.side_items)
        side_items.push_back(i->clone());
}

Mocha &Mocha::operator=(const Mocha &moc) {
    if (&moc == this)
        return *this;
    for (const auto &i : side_items)
        delete i;
    ingredients.clear();
    side_items.clear();
    for (auto &i : moc.ingredients)
        ingredients.push_back(i->clone());
    for (auto &i : moc.side_items)
        side_items.push_back(i->clone());
    return *this;
}

// Destructor

Mocha::~Mocha() {
    for (auto &i : side_items)
        delete i;
    side_items.clear();
}

// Methods

double Mocha::price() {
    double res = 0;
    for (const auto &i : ingredients)
        res += i->price();
    for (const auto &i : side_items)
        res += i->price();
    return res;
}

void Mocha::brew() {
    std::cout << "makeing " + get_name() << " ..." << std::endl;
    for (const auto &i : ingredients)
        std::cout << "adding " + i->get_name() << " ..." << std::endl;
    for (const auto &i : side_items)
        std::cout << "adding " + i->get_name() << " ..." << std::endl;
    std::cout << get_name() + " finished" << std::endl;
}

void Mocha::add_side_item(Ingredient *side) { side_items.push_back(side); }

std::vector<Ingredient *> &Mocha::get_side_items() { return side_items; }