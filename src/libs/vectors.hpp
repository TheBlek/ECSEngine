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

struct Vector2 {
    double x, y;

    double Length() const;
    
    Vector2 Normalized() const;

    Vector2 operator+(const Vector2& other) const;
    Vector2 operator-(const Vector2& other) const;
    Vector2 operator*(double s) const;
    Vector2 operator/(double s) const;

    void operator-=(const Vector2& other);
    void operator+=(const Vector2& other);

};

struct Vector2Int {
    int x, y;

    double Length() const;
    
    Vector2Int operator+(const Vector2Int& other) const;
    Vector2Int operator-(const Vector2Int& other) const;
    Vector2Int operator*(int s) const;
    Vector2Int operator/(int s) const;

    void operator-=(const Vector2Int& other);
    void operator+=(const Vector2Int& other);

};
