#include <Vector3.h>

#ifndef VECTOR2_H
#define VECTOR2_H

# define PI 3.14159265358979323846

struct Vector2 {
    public:
      // Constructors
      Vector2() : X(0), Y(0) {}
      Vector2(float x, float y) : X(x), Y(y) {}
      Vector2(Vector3 v) : X(v.X), Y(v.Y) {}
    
      // Properties
      float X;
      float Y;
      float& x = X;
      float& y = Y;
    
      // Static
      static const Vector2 up;
      static const Vector2 down;
      static const Vector2 left;
      static const Vector2 right;
      static const Vector2 one;
      static const Vector2 zero;
    
      // Methods
      Vector2 rotate(float deg);
      float magnitude();
      Vector2 unit();
      Vector2 normalized();
      void normalize();
      Vector2 lerp(Vector2 Vector, float a);
      Vector2 clamp(Vector2 Min, Vector2 Max);
      Vector2 absolute();
      float dot(Vector2 Vector);
      float distance(Vector2 Vector);
      bool equals(Vector2 Vector);
    
      // Math Operators
      Vector2 operator=(const Vector2 &other);
      Vector2 operator+(const Vector2 &other);
      Vector2 operator-(const Vector2 &other);
      Vector2 operator*(const Vector2 &other);
      Vector2 operator/(const Vector2 &other);
      Vector2 operator*(const float &value);
      Vector2 operator/(const float &value);
      Vector2& operator+=(const Vector2 &other);
      Vector2& operator-=(const Vector2 &other);
      Vector2& operator*=(const Vector2 &other);
      Vector2& operator/=(const Vector2 &other);
      Vector2& operator*=(const float &value);
      Vector2& operator/=(const float &value);
    
      // Bool Operators
      bool operator==(const Vector2 &other);
      bool operator!=(const Vector2 &other);
};

#endif