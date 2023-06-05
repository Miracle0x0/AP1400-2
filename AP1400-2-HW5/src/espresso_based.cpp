#include "espresso_based.h"

std::string EspressoBased::get_name() { return name; }

std::vector<Ingredient *> &EspressoBased::get_ingredients() {
    return ingredients;
}

// Constructors
EspressoBased::EspressoBased(const std::string &name) : name(name) {}

EspressoBased::EspressoBased(const EspressoBased &esp) {
    for (const auto &i : esp.ingredients)
        ingredients.push_back(i->clone());
}

EspressoBased::~EspressoBased() {
    for (const auto &i : ingredients)
        delete i;
    ingredients.clear();
}