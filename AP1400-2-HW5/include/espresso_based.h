#ifndef ESPRESSO_BASED_H
#define ESPRESSO_BASED_H

#include "ingredient.h"
#include <string>
#include <vector>

/// @brief base to create coffees
class EspressoBased {
  public:
    virtual std::string get_name();
    virtual double price() = 0;

    virtual void brew() = 0; // TODO TUI implement ...
    std::vector<Ingredient *> &get_ingredients();

    virtual ~EspressoBased();

  protected:
    EspressoBased() = default;
    EspressoBased(const std::string &name);
    EspressoBased(const EspressoBased &esp);
    EspressoBased &operator=(const EspressoBased &esp) = delete;

    std::vector<Ingredient *> ingredients;
    std::string name;
};

#endif // ESPRESSO_BASED_H