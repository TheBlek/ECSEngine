#include "vectors.hpp"
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

double Vector2::Length() const {
    return std::sqrt(x*x + y*y);
}

Vector2 Vector2::Normalized() const {
    return *this / this->Length();
}

Vector2 Vector2::operator+(const Vector2& other) const {
    return (Vector2){ x + other.x, y + other.y };
}

Vector2 Vector2::operator-(const Vector2& other) const {
    return (Vector2){ x - other.x, y - other.y };
}

void Vector2::operator-=(const Vector2& other) {
    *this = *this - other;
}

void Vector2::operator+=(const Vector2& other) {
    *this = *this + other;
}

Vector2 Vector2::operator*(double s) const {
    return (Vector2){ x*s, y*s };
}

Vector2 Vector2::operator/(double s) const {
    return (Vector2){ x/s, y/s };
}

double Vector2Int::Length() const {
    return std::sqrt(x*x + y*y);
}

Vector2Int Vector2Int::operator+(const Vector2Int& other) const {
    return (Vector2Int){ x + other.x, y + other.y };
}

Vector2Int Vector2Int::operator-(const Vector2Int& other) const {
    return (Vector2Int){ x - other.x, y - other.y };
}

void Vector2Int::operator-=(const Vector2Int& other) {
    *this = *this - other;
}

void Vector2Int::operator+=(const Vector2Int& other) {
    *this = *this + other;
}

Vector2Int Vector2Int::operator*(int s) const {
    return (Vector2Int){ x*s, y*s };
}

Vector2Int Vector2Int::operator/(int s) const {
    return (Vector2Int){ x/s, y/s };
}
