#pragma once
#include <algorithm>

#include "geometry.h"

// Encodes an RGBA value.
struct Color {
  double r;
  double g;
  double b;
  double a;

  // Create a pixel with default alpha (1.0).
  Color(double r, double g, double b) : r(r), g(g), b(b), a(1.0){};

  // Create a pixel with the given rgba values.
  Color(double r, double g, double b, double a) : r(r), g(g), b(b), a(a){};

  // Multiply all of the channels of the color by a scalar.
  Color operator*(const double scalar) const;

  // Divive all of the channels of a color by a scalar.
  Color operator/(double scalar) const;

  // Multiply a color by another color. The product of two colors is the product
  // of the individual channels.
  Color operator*(const Color& c) const;

  // Add two colors together. The sum of two colors is the sum of the individual
  // channels.
  Color operator+(const Color& c) const;

  // Subtract two colors. The different between two colors is the difference
  // between their individual channels.
  Color operator-(const Color& c) const;

  // Increment this color by another one. Adds the other color's rgb values to
  // this color.
  void operator+=(const Color& c);

  // Clamp the rgb channels so that their values are between 0 and 1.
  void clamp();

  // Send a string representation of the pixel to an output stream.
  friend std::ostream& operator<<(std::ostream& os, const Color& p);

  // Return a string representation of the color.
  std::string toString() const;
};

// A point light.
class Light {
 public:
  double intensity;
  Pnt3 center;
  Color color;

  Light(double intensity, const Pnt3& center, const Color& color)
      : intensity(intensity), center(center), color(color) {}

  virtual ~Light() {}
};

// A square light.
class SquareLight : public Light {
 public:
  // The direction of the light. Should be a unit vector.
  Vec3 direction;

  // The width of the square.
  double width;

 public:
  // Create a square light. The direction of the light should be a unit vector.
  SquareLight(double intensity, const Pnt3& center, const Color& color,
              const Vec3& direction, const double width)
      : Light{intensity, center, color}, direction(direction), width(width) {}

  // Get a list of random points on the area of the light.
  std::vector<Pnt3> getRandomPoints(unsigned samples) const;
};