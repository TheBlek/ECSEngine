#include "vector3.hpp"
#include <cmath>

double Vector3::Length() const {
    return std::sqrt(x*x + y*y + z*z);
}

Vector3 Vector3::Normalized() const {
    return *this / this->Length();
}

Vector3 Vector3::operator+(const Vector3& other) const {
    return (Vector3){ x + other.x, y + other.y, z + other.z };
}

Vector3 Vector3::operator-(const Vector3& other) const {
    return (Vector3){ x - other.x, y - other.y, z - other.z };
}

void Vector3::operator-=(const Vector3& other) {
    *this = *this - other;
}

void Vector3::operator+=(const Vector3& other) {
    *this = *this + other;
}

Vector3 Vector3::operator*(double s) const {
    return (Vector3){ x*s, y*s, z*s };
}

Vector3 Vector3::operator/(double s) const {
    return (Vector3){ x/s, y/s, z/s };
}