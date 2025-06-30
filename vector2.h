#ifndef VECTOR2_H
#define VECTOR2_H

class Vector2
{
private:
    double x;
    double y;

public:
    Vector2();
    Vector2(double, double);
    Vector2(const Vector2&);

    Vector2& operator=(const Vector2&);
    Vector2& operator+=(const Vector2&);
    Vector2& operator-=(const Vector2&);
    Vector2& operator*=(const double&);
    Vector2& operator/=(const double&);

    Vector2 operator+(const Vector2&);
    Vector2 operator-(const Vector2&);
    Vector2 operator*(const double&);
    Vector2 operator/(const double&);

    Vector2 operator-();

    double operator*(const Vector2&);

    double length() const;
    double xP() const;
    double yP() const;
    Vector2 normalized() const;
    Vector2 normalized(double) const;


};

#endif // VECTOR2_H
