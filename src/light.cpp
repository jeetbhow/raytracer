#include "light.h"

std::string Color::toString() const {
    return std::format("({}, {}, {})", r, g, b);
}