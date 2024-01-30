#include "geometry.h"

Pnt3 Pnt3::operator+(const Vec3& other) const
{
    return Pnt3(x + other.x, y + other.y, z + other.z);
}

void Pnt3::operator+=(const Vec3& other)
{
    x += other.x;
    y += other.y;
    z += other.z;
}

Vec3 Pnt3::operator-(const Pnt3& other) const
{
    return Vec3(x - other.x, y - other.y, z - other.z);
}

std::ostream& operator<<(std::ostream& os, const Pnt3& p)
{
    os << "(" << p.x << ", " << p.y << ", " << p.z << ")";
    return os;
}

Vec3 Vec3::operator+(const Vec3& other) const
{
    return Vec3(x + other.x, y + other.y, z + other.z);
}

Vec3 Vec3::operator-(const Vec3& other) const
{
    return Vec3(x - other.x, y - other.y, z - other.z);
}

Vec3 Vec3::operator-() const
{
    return Vec3(-x, -y, -z);
}

Vec3 Vec3::operator*(const double& scalar) const
{
    return Vec3(scalar * x, scalar * y, scalar * z);
}

Vec3 Vec3::operator/(const double& scalar) const
{
    return Vec3(x / scalar, y / scalar, z / scalar);
}

double Vec3::dot(const Vec3& first, const Vec3& second)
{
    return first.x * second.x + first.y * second.y + first.z * second.z;
}

Vec3 Vec3::cross(const Vec3& first, const Vec3& second)
{
    double x = first.y * second.z - first.z * second.y;
    double y = first.z * second.x - first.x * second.z;
    double z = first.x * second.y - first.y * second.x;
    return Vec3(x, y, z);
}

Vec3 Vec3::reflect(const Vec3& incident, const Vec3& normal)
{
    return incident - normal * 2 * Vec3::dot(incident, normal);
}

void Vec3::reciprocal()
{
    this->x = 1 / this->x;
    this->y = 1 / this->y;
    this->z = 1 / this->z;
}

void Vec3::negate()
{
    this->x = -this->x;
    this->y = -this->y;
    this->z = -this->z;
}

double Vec3::dot(const Vec3& other) const
{
    return x * other.x + y * other.y + z * other.z;
}

double Vec3::length() const
{
    return sqrt(x * x + y * y + z * z);
}

Vec3 Vec3::normalize() const
{
    return *this / length();
}

Mat3 Mat3::identity()
{
    Mat3 m {};
    m[0][0] = 1.0;
    m[1][1] = 1.0;
    m[2][2] = 1.0;
    return m;
}

double* Mat3::operator[](size_t row)
{
    return data[row];
}

const double* Mat3::operator[](size_t row) const
{
    return data[row];
}

Vec3 Mat3::operator*(const Vec3& v) const
{
    double x = data[0][0] * v.x + data[0][1] * v.y + data[0][2] * v.z;
    double y = data[1][0] * v.x + data[1][1] * v.y + data[1][2] * v.z;
    double z = data[2][0] * v.x + data[2][1] * v.y + data[2][2] * v.z;
    return Vec3 { x, y, z };
}

Mat3 Mat3::operator*(const Mat3& m) const
{
    Mat3 res {};

    for (size_t i = 0; i < SIZE; ++i) {
        for (size_t j = 0; j < SIZE; ++j) {
            for (size_t k = 0; k < SIZE; ++k) {
                res[i][j] += data[i][k] * m[k][j];
            }
        }
    }

    return res;
}

void Mat3::setReciprocalDiag()
{
    data[0][0] = 1 / data[0][0];
    data[1][1] = 1 / data[1][1];
    data[2][2] = 1 / data[2][2];
}

Mat3 Mat3::extractScaling() const
{
    Mat3 m {};
    m[0][0] = data[0][0];
    m[1][1] = data[1][1];
    m[2][2] = data[2][2];
    return m;
}

size_t Mat3::size()
{
    return SIZE;
}

void Mat3::transpose()
{
    for (size_t i = 0; i < SIZE; ++i) {
        for (size_t j = i + 1; j < SIZE; ++j) {
            std::swap(data[i][j], data[j][i]);
        }
    }
}

std::ostream& operator<<(std::ostream& os, const Mat3& m)
{
    for (int i = 0; i < Mat3::SIZE; ++i) {
        os << "[ ";
        for (int j = 0; j < Mat3::SIZE; ++j) {
            os << m[i][j];
            if (j < Mat3::SIZE - 1) {
                os << ", ";
            }
        }
        os << " ]" << std::endl; // Close the square brackets and start a new line
                                 // for the next row
    }
    return os;
}

Mat4::Mat4()
{
    for (size_t i = 0; i < SIZE; ++i) {
        for (size_t j = 0; j < SIZE; ++j) {
            data[i][j] = 0;
        }
    }
}

Mat4::Mat4(const std::vector<std::vector<double>> arr)
{
    for (size_t i = 0; i < SIZE; ++i) {
        for (size_t j = 0; j < SIZE; ++j) {
            data[i][j] = arr[i][j];
        }
    }
}

Mat4::Mat4(const Mat4& other)
{
    for (size_t i = 0; i < SIZE; ++i) {
        for (size_t j = 0; j < SIZE; ++j) {
            data[i][j] = other[i][j];
        }
    }
}

Mat4::Mat4(const Mat3& linear, Vec3 translation)
{
    fill(linear);
    data[0][3] = translation.x;
    data[1][3] = translation.y;
    data[2][3] = translation.z;
    data[3][0] = 0.0;
    data[3][1] = 0.0;
    data[3][2] = 0.0;
    data[3][3] = 1.0;
}

Mat3 Mat4::extractLinear() const
{
    Mat3 m {};

    for (size_t i = 0; i < 3; ++i) {
        for (size_t j = 0; j < 3; ++j) {
            m[i][j] = data[i][j];
        }
    }

    return m;
}

Mat4 Mat4::identity()
{
    Mat4 result {};
    result[0][0] = 1.0;
    result[1][1] = 1.0;
    result[2][2] = 1.0;
    result[3][3] = 1.0;
    return result;
}

Pnt3 Mat4::operator*(const Pnt3& other) const
{
    double x = data[0][0] * other.x + data[0][1] * other.y + data[0][2] * other.z + data[0][3];
    double y = data[1][0] * other.x + data[1][1] * other.y + data[1][2] * other.z + data[1][3];
    double z = data[2][0] * other.x + data[2][1] * other.y + data[2][2] * other.z + data[2][3];
    return Pnt3 { x, y, z };
}

Vec3 Mat4::operator*(const Vec3& other) const
{
    double x = data[0][0] * other.x + data[0][1] * other.y + data[0][2] * other.z;
    double y = data[1][0] * other.x + data[1][1] * other.y + data[1][2] * other.z;
    double z = data[2][0] * other.x + data[2][1] * other.y + data[2][2] * other.z;
    return Vec3 { x, y, z };
}

Mat4 Mat4::operator*(const Mat4& other) const
{
    Mat4 result {};

    for (size_t i = 0; i < 4; ++i) {
        for (size_t j = 0; j < 4; ++j) {
            for (size_t k = 0; k < 4; ++k) {
                result[i][j] += data[i][k] * other[k][j];
            }
        }
    }

    return result;
}

double* Mat4::operator[](size_t row)
{
    return data[row];
}

const double* Mat4::operator[](size_t row) const
{
    return data[row];
}

void Mat4::fill(Mat3 m)
{
    for (size_t i = 0; i < m.size(); ++i) {
        for (size_t j = 0; j < m.size(); ++j) {
            data[i][j] = m[i][j];
        }
    }
}

void Mat4::translate(Pnt3 p)
{
    data[0][3] = p.x;
    data[1][3] = p.y;
    data[2][3] = p.z;
}

void Mat4::translate(double dx, double dy, double dz)
{
    data[0][3] += dx;
    data[1][3] += dy;
    data[2][3] += dz;
}

void Mat4::setTranslate(double x, double y, double z)
{
    data[0][3] = x;
    data[1][3] = y;
    data[2][3] = z;
}

void Mat4::scale(double scalar)
{
    data[0][0] *= scalar;
    data[1][1] *= scalar;
    data[2][2] *= scalar;
}

void Mat4::scale(double kx, double ky, double kz)
{
    data[0][0] *= kx;
    data[1][1] *= ky;
    data[2][2] *= kz;
}

double Mat4::getLength(size_t col) const
{
    return std::sqrt((data[0][col] * data[0][col]) + (data[1][col] * data[1][col]) + (data[2][col] * data[2][col]));
}

Mat4 Mat4::inverse() const
{
    Mat3 linear = extractLinear();

    // We extract the scaling matrix and invert it.
    Mat3 inverseScale = linear.extractScaling();
    inverseScale.setReciprocalDiag();

    // Multiplying by the inverse of the scaling matrix removes it from the
    // original linear transformation. This leaves us with just rotation. We then
    // invert the rotation to get the entire inverse.
    Mat3 inverseRot = inverseScale * linear;
    inverseRot.transpose();

    Mat3 inverseRotScale = inverseRot * inverseScale;
    Vec3 inverseTrans = inverseRotScale * Vec3 { -data[0][3], -data[1][3], -data[2][3] };
    return Mat4 { inverseRotScale, inverseTrans };
}

std::ostream& operator<<(std::ostream& os, const Mat4& m)
{
    for (int i = 0; i < 4; ++i) {
        os << "[ ";
        for (int j = 0; j < 4; ++j) {
            os << m[i][j];
            if (j < 3) {
                os << ", ";
            }
        }
        os << " ]" << std::endl; // Close the square brackets and start a new line
                                 // for the next row
    }
    return os;
}

Pnt3 Ray::at(double t) const
{
    return origin + direction * t;
}

Ray Ray::transformed(const Mat4& m) const
{
    Pnt3 newOrigin = m * origin;
    Vec3 newDirection = m * direction;
    return Ray { newOrigin, newDirection };
}

void Ray::transform(const Mat4& m)
{
    origin = m * origin;
    direction = m * direction;
}