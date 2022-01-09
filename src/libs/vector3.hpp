#pragma once

struct Vector3 {
    double x, y, z;

    double Length() const;
    
    Vector3 Normalized() const;

    Vector3 operator+(const Vector3& other) const;
    Vector3 operator-(const Vector3& other) const;
    Vector3 operator*(double s) const;
    Vector3 operator/(double s) const;

    void operator-=(const Vector3& other);
    void operator+=(const Vector3& other);

};