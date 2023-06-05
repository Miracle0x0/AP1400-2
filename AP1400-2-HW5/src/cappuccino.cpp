#include "cappuccino.h"
#include "sub_ingredients.h"
#include <iostream>

// Constructors

/// @brief A cappuccino needs 2 units of espresso, 2 units of milk, and 1 unit
/// of milk foam.
Cappuccino::Cappuccino() : EspressoBased("Cappuccino") {
    ingredients.push_back(new Espresso(2));
    ingredients.push_back(new Milk(2));
    ingredients.push_back(new MilkFoam(1));
}

Cappuccino::Cappuccino(const Cappuccino &cap) : EspressoBased(cap) {
    name = cap.name;
    for (const auto &i : cap.side_items)
        side_items.push_back(i->clone());
}

Cappuccino &Cappuccino::operator=(const Cappuccino &cap) {
    if (&cap == this)
        return *this;
    for (const auto &i : side_items)
        delete i;
    ingredients.clear();
    side_items.clear();
    for (const auto &i : cap.ingredients)
        ingredients.push_back(i->clone());
    for (const auto &i : cap.side_items)
        side_items.push_back(i->clone());
    return *this;
}

// Destructor

Cappuccino::~Cappuccino() {
    for (const auto &i : side_items)
        delete i;
    side_items.clear();
}

// Methods

double Cappuccino::price() {
    double res = 0;
    for (const auto &i : ingredients)
        res += i->price();
    for (const auto &i : side_items)
        res += i->price();
    return res;
}

void Cappuccino::brew() {
    std::cout << "makeing " + get_name() << " ..." << std::endl;
    for (const auto &i : ingredients)
        std::cout << "adding " + i->get_name() << " ..." << std::endl;
    for (const auto &i : side_items)
        std::cout << "adding " + i->get_name() << " ..." << std::endl;
    std::cout << get_name() << " finished" << std::endl;
}

void Cappuccino::add_side_item(Ingredient *side) { side_items.push_back(side); }

std::vector<Ingredient *> &Cappuccino::get_side_items() { return side_items; }