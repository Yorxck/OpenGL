#include <cmath>
#include <string>
#include <Vector3.h>

// define static members
const Vector3 Vector3::up(0, 1, 0);
const Vector3 Vector3::down(0, -1, 0);
const Vector3 Vector3::left(-1, 0, 0);
const Vector3 Vector3::right(1, 0, 0);
const Vector3 Vector3::front(0, 0, 1);
const Vector3 Vector3::back(0, 0, -1);
const Vector3 Vector3::one(1, 1, 1);
const Vector3 Vector3::zero(0, 0, 0);

// functions

float Vector3::magnitude() {
    return sqrt(pow(X, 2) + pow(Y, 2) + pow(Z, 2));
}

Vector3 Vector3::unit() {
    return (*this / magnitude());
}

Vector3 Vector3::normalized() {
    return (*this / magnitude());
}

void Vector3::normalize() {
    Vector3 Normalized = normalized();
    X = Normalized.X;
    Y = Normalized.Y;
    Z = Normalized.Z;
}

Vector3 Vector3::cross(const Vector3& a, const Vector3& b) {
        
    return Vector3(
        (a.Y * b.Z - a.Z * b.Y),
        (a.Z * b.X - a.X * b.Z),
        (a.X * b.Y - a.Y * b.X)
    );
}

Vector3 Vector3::lerp(const Vector3& Vector, float a) {
    float x = X + (Vector.X - X) * a;
    float y = Y + (Vector.Y - Y) * a;
    float z = Z + (Vector.Z - Z) * a;
    return Vector3(x, y, z);
}

Vector3 Vector3::clamp(const Vector3& Min, const Vector3& Max) {
    return Vector3(
        (this->X) < (Min.X) ? (Min.X) : ((this->X) > (Max.X) ? (Max.X) : (this->X)),
        (this->Y) < (Min.Y) ? (Min.Y) : ((this->Y) > (Max.Y) ? (Max.Y) : (this->Y)),
        (this->Z) < (Min.Z) ? (Min.Z) : ((this->Z) > (Max.Z) ? (Max.Z) : (this->Z))
    );
}

Vector3 Vector3::absolute() {
    return Vector3(
        abs(X),
        abs(Y),
        abs(Z)
    );
}

float Vector3::dot(const Vector3& a, const Vector3& b) {
    return a.X * b.X + a.Y * b.Y + a.Z * b.Z;
}

float Vector3::distance(const Vector3& Vector) {
    return (*this - Vector).magnitude();
}

bool Vector3::equals(const Vector3& Vector) {
    return (X == Vector.X && Y == Vector.Y && Z == Vector.Z);
}

std::string Vector3::toString() {
    return (std::to_string((int) this->X) + ", " + std::to_string((int) this->Y) + ", " + std::to_string((int) this->Z));
}

// math operators
Vector3 Vector3::operator=(const Vector3& other) {
    this->X = other.X;
    this->Y = other.Y;
    this->Z = other.Z;

    return *this;
}

Vector3 Vector3::operator+(const Vector3& other) {
    return Vector3(
        X + other.X,
        Y + other.Y,
        Z + other.Z
    );
}

Vector3 Vector3::operator-(const Vector3& other) {
    return Vector3(
        X - other.X,
        Y - other.Y,
        Z - other.Z
    );
}

Vector3 Vector3::operator*(const Vector3& other) {
    return Vector3(
        X * other.X,
        Y * other.Y,
        Z * other.Z
    );
}

Vector3 Vector3::operator/(const Vector3& other) {
    return Vector3(
        X / other.X,
        Y / other.Y,
        Z / other.Z
    );
}

Vector3 Vector3::operator*(const float& value) {
    return Vector3(
        X * value,
        Y * value,
        Z * value
    );
}

Vector3 Vector3::operator/(const float& value) {
    return Vector3(
        X / value,
        Y / value,
        Z / value
    );
}

Vector3& Vector3::operator+=(const Vector3& other) {
    this -> X += other.X;
    this -> Y += other.Y;
    this -> Z += other.Z;

    return *this;
}

Vector3& Vector3::operator-=(const Vector3& other) {
    this -> X -= other.X;
    this -> Y -= other.Y;
    this -> Z -= other.Z;

    return *this;
}

Vector3& Vector3::operator*=(const Vector3& other) {
    this -> X *= other.X;
    this -> Y *= other.Y;
    this -> Z *= other.Z;

    return *this;
}

Vector3& Vector3::operator/=(const Vector3& other) {
    this -> X /= other.X;
    this -> Y /= other.Y;
    this -> Z /= other.Z;
 
    return *this;
}

Vector3& Vector3::operator*=(const float& value) {
    this -> X *= value;
    this -> Y *= value;
    this -> Z *= value;

    return *this;
}

Vector3& Vector3::operator/=(const float& value) {
    this -> X /= value;
    this -> Y /= value;
    this -> Z /= value;

    return *this;
}

// bool operators
bool Vector3::operator==(const Vector3& other) {
    return this->equals(other);
}

bool Vector3::operator!=(const Vector3& other) {
    return !this->equals(other);
}