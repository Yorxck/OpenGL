#include <cmath>
#include <Vector2.h>

// define static members
const Vector2 Vector2::up(0, 1);
const Vector2 Vector2::down(0, -1);
const Vector2 Vector2::left(-1, 0);
const Vector2 Vector2::right(1, 0);
const Vector2 Vector2::one(1, 1);
const Vector2 Vector2::zero(0, 0);

// functions
Vector2 Vector2::rotate(float deg) {
    float rad = -deg * (PI / 180); // deg2rad
    float x = X * cos(rad) - Y * sin(rad);
    float y = X * sin(rad) + Y * cos(rad);
    return Vector2(x, y);
}

float Vector2::magnitude() {
    return sqrt(pow(X, 2) + pow(Y, 2));
}

Vector2 Vector2::unit() {
    return (*this / magnitude());
}

Vector2 Vector2::normalized() {
    return (*this / magnitude());
}

void Vector2::normalize() {
    Vector2 Normalized = normalized();
    X = Normalized.X;
    Y = Normalized.Y;
}

Vector2 Vector2::lerp(Vector2 Vector, float a) {
    float x = X + (Vector.X - X) * a;
    float y = Y + (Vector.Y - Y) * a;
    return Vector2(x, y);
}

Vector2 Vector2::clamp(Vector2 Min, Vector2 Max) {
    return Vector2(
        (this->X) < (Min.X) ? (Min.X) : ((this->X) > (Max.X) ? (Max.X) : (this->X)),
        (this->Y) < (Min.Y) ? (Min.Y) : ((this->Y) > (Max.Y) ? (Max.Y) : (this->Y))
    );
}

Vector2 Vector2::absolute() {
    return Vector2(
        abs(X),
        abs(Y)
    );
}

float Vector2::dot(Vector2 Vector) {
    return X * Vector.X + Y * Vector.Y;
}

float Vector2::distance(Vector2 Vector) {
    return (*this - Vector).magnitude();
}

bool Vector2::equals(Vector2 Vector) {
    return (X == Vector.X && Y == Vector.Y);
}

// math operators
Vector2 Vector2::operator=(const Vector2& other) {
    this->X = other.X;
    this->Y = other.Y;

    return *this;
}

Vector2 Vector2::operator+(const Vector2& other) {
    return Vector2(
        X + other.X,
        Y + other.Y
    );
}

Vector2 Vector2::operator-(const Vector2& other) {
    return Vector2(
        X - other.X,
        Y - other.Y
    );
}

Vector2 Vector2::operator*(const Vector2& other) {
    return Vector2(
        X * other.X,
        Y * other.Y
    );
}

Vector2 Vector2::operator/(const Vector2& other) {
    return Vector2(
        X / other.X,
        Y / other.Y
    );
}

Vector2 Vector2::operator*(const float& value) {
    return Vector2(
        X * value,
        Y * value
    );
}

Vector2 Vector2::operator/(const float& value) {
    return Vector2(
        X / value,
        Y / value
    );
}

Vector2& Vector2::operator+=(const Vector2& other) {
    this -> X += other.X;
    this -> Y += other.Y;

    return *this;
}

Vector2& Vector2::operator-=(const Vector2& other) {
    this -> X -= other.X;
    this -> Y -= other.Y;

    return *this;
}

Vector2& Vector2::operator*=(const Vector2& other) {
    this -> X *= other.X;
    this -> Y *= other.Y;

    return *this;
}

Vector2& Vector2::operator/=(const Vector2& other) {
    this -> X /= other.X;
    this -> Y /= other.Y;
 
    return *this;
}

Vector2& Vector2::operator*=(const float& value) {
    this -> X *= value;
    this -> Y *= value;

    return *this;
}

Vector2& Vector2::operator/=(const float& value) {
    this -> X /= value;
    this -> Y /= value;

    return *this;
}

// bool operators
bool Vector2::operator==(const Vector2& other) {
    return this->equals(other);
}

bool Vector2::operator!=(const Vector2& other) {
    return !this->equals(other);
}