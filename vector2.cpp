#include "vector2.h"
#include <cmath>

Vector2::Vector2() : x(0), y(0) {}

Vector2::Vector2(double _x, double _y) : x(_x), y(_y) {}

Vector2::Vector2(const Vector2& src) : x(src.x), y(src.y) {}

Vector2& Vector2::operator=(const Vector2& rhs){
    if(this != &rhs){
        x = rhs.x;
        y = rhs.y;
    }
    return *this;
}

Vector2& Vector2::operator+=(const Vector2& rhs){
    x += rhs.x;
    y += rhs.y;
    return *this;
}

Vector2& Vector2::operator-=(const Vector2& rhs){
    x -= rhs.x;
    y -= rhs.y;
    return *this;
}

Vector2& Vector2::operator*=(const double& rhs){
    x *= rhs;
    y *= rhs;
    return *this;
}

Vector2& Vector2::operator/=(const double& rhs){
    x /= rhs;
    y /= rhs;
    return *this;
}

Vector2 Vector2::operator+(const Vector2& rhs){
    Vector2 res(*this);
    res += rhs;
    return res;
}

Vector2 Vector2::operator-(const Vector2& rhs){
    Vector2 res(*this);
    res -= rhs;
    return res;
}

Vector2 Vector2::operator*(const double& rhs){
    Vector2 res(*this);
    res *= rhs;
    return res;
}

Vector2 Vector2::operator/(const double& rhs){
    Vector2 res(*this);
    res /= rhs;
    return res;
}

double Vector2::operator*(const Vector2& rhs){
    return x * rhs.x + y * rhs.y;
}

Vector2 Vector2::operator-(){
    Vector2 res(-x, -y);
    return res;
}

double Vector2::length() const{
    return std::sqrt(x * x + y * y);
}

Vector2 Vector2::normalized() const{
    Vector2 res(x/length(), y/length());
    return res;
}

Vector2 Vector2::normalized(double length) const{
    Vector2 res(this->normalized() * length);
    return res;
}

double Vector2::xP() const{
    return x;
}

double Vector2::yP() const{
    return y;
}
