#include "light.h"
#include<sstream>
#include <random>

Color Color::operator+(const Color &c) const {
    return Color(r + c.r, g + c.g, b + c.b);
}

Color Color::operator-(const Color &c) const {
    return Color(r - c.r, g - c.g, b - c.b);
}

Color Color::operator*(const Color &c) const {
    return Color(r * c.r, g * c.g, b * c.b);
}

Color Color::operator*(const double scalar) const {
    return Color(r * scalar, g * scalar, b * scalar);
}

Color Color::operator/(const double scalar) const {
    return Color(r / scalar, g / scalar, b / scalar);
}

void Color::operator+=(const Color &c) {
    r += c.r;
    g += c.g;
    b += c.b;
}

void Color::operator/=(const double scalar) {
    r /= scalar;
    g /= scalar;
    b /= scalar;
}

void Color::operator*=(const Color &c) {
    r *= c.r;
    g *= c.g;
    b *= c.b;
}

void Color::operator*=(const float scalar) {
    r *= scalar;
    g *= scalar;
    b *= scalar;
}

void Color::clamp() {
    r = std::clamp(r, 0.0, 1.0);
    g = std::clamp(g, 0.0, 1.0);
    b = std::clamp(b, 0.0, 1.0);
    a = std::clamp(a, 0.0, 1.0);
}

std::string Color::toString() const {
	std::stringstream ss;
	ss << "(" << r << ", " << g << ", " << b << ")" << "\n";
	return ss.str();
}

std::vector<Pnt3> SquareLight::getRandomPoints(unsigned samples) const {
    std::mt19937 gen(12345);
    std::uniform_real_distribution<> dis(-width / 2.0, width / 2.0);

    Vec3 up(0, 1, 0);
    if (std::abs(Vec3::dot(up, direction)) > 0.9) {
        up = Vec3(1, 0, 0);
    }

    Vec3 right = Vec3::cross(direction, up);

    std::vector<Pnt3> points(samples);
    for (int i = 0; i < samples; ++i) {
        double u = dis(gen);
        double v = dis(gen);
        Pnt3 point = center + right * u + up * v;
        points[i] = point;
    }
    return points;
}

Color Color::white() { return Color{1, 1, 1}; }

Color Color::black() { return Color{0.1, 0.1, 0.1}; }

Color Color::red() { return Color{1, 0, 0}; }

Color Color::blue() { return Color{0, 0, 1}; }

Color Color::green() { return Color{0, 1, 0}; }

Color Color::grey() { return Color{0.5, 0.5, 0.5}; }