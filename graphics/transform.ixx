export module transform;

import geometry;
import <stdexcept>;
import <array>;

export class Mat {
private:
  std::array<std::array<double, 4>, 4> data;

public:
  Mat();
  Mat(const std::array<std::array<double, 4>, 4>& arr);
  void translate(double dx, double dy, double dz);
  void scale(double factor);
  Pnt3 operator*(const Pnt3& point) const;
  std::array<double, 4>& operator[](int row);
  const std::array<double, 4>& operator[](int row) const;
};

Mat::Mat() {
  for (int i = 0; i < 4; ++i) {
    for (int j = 0; j < 4; ++j) {
      data[i][j] = (i == j) ? 1.0 : 0.0;
    }
  }
}

Mat::Mat(const std::array<std::array<double, 4>, 4>& arr) : data(arr) {}

void Mat::translate(double dx, double dy, double dz) {
  data[0][3] += dx;
  data[1][3] += dy;
  data[2][3] += dz;
}

void Mat::scale(double factor) {
  data[0][0] *= factor;
  data[1][1] *= factor;
  data[2][2] *= factor;
}

Pnt3 Mat::operator*(const Pnt3& point) const {
  Pnt3 res(0.0, 0.0, 0.0);

  for (int i = 0; i < 4; ++i) {
    for (int j = 0; j < 4; ++j) {
      res[i] += data[i][j] * point[j];
    }
  }
  return res;
}

std::array<double, 4>& Mat::operator[](int row) {
  if (row >= 0 && row < 4) {
    return data[row];
  } else {
    throw std::out_of_range("Matrix row index out of range");
  }
}

const std::array<double, 4>& Mat::operator[](int row) const {
  if (row >= 0 && row < 4) {
    return data[row];
  } else {
    throw std::out_of_range("Matrix row index out of range");
  }
}