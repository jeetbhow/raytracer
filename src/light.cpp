#include "light.h"

Color Color::operator +(const Color& c) const {
    return Color(r + c.r, g + c.g, b + c.b);
}

Color Color::operator -(const Color& c) const {
    return Color(r - c.r, g - c.g, b - c.b);
}

Color Color::operator *(const Color& c) const {
    return Color(r * c.r, g * c.g, b * c.b);
}

Color Color::operator *(const double scalar) const {
    return Color(r * scalar, g * scalar, b * scalar);
}

void Color::operator +=(const Color& c) {
    r += c.r; 
    g += c.g; 
    b += c.b;
}

void Color::clamp() {
    r = std::clamp(r, 0.0, 1.0);
    g = std::clamp(g, 0.0, 1.0);
    b = std::clamp(b, 0.0, 1.0);
}

std::string Color::toString() const {
    return std::format("({}, {}, {})", r, g, b);
}