#ifndef VECTOR3_H
#define VECTOR3_H

# define PI 3.14159265358979323846

struct Vector3 {
    public:
      // Constructors
      Vector3() : X(0), Y(0), Z(0) {}
      Vector3(float x, float y, float z) : X(x), Y(y), Z(z) {}
    
      // Properties
      float X;
      float Y;
      float Z;
      float* const x = &X;
      float* const y = &Y;
      float* const z = &Z;
    
      // Static
      static const Vector3 up;
      static const Vector3 down;
      static const Vector3 left;
      static const Vector3 right;
      static const Vector3 front;
      static const Vector3 back;
      static const Vector3 one;
      static const Vector3 zero;
      static Vector3 cross(const Vector3& a, const Vector3& b) ;
    
      // Methods
      float magnitude();
      Vector3 unit();
      Vector3 normalized();
      void normalize();
      Vector3 lerp(const Vector3& Vector, float a);
      Vector3 clamp(const Vector3& Min, const Vector3& Max);
      Vector3 absolute();
      float dot(const Vector3& Vector);
      float distance(const Vector3& Vector);
      bool equals(const Vector3& Vector);
    
      // Math Operators
      Vector3 operator=(const Vector3 &other);
      Vector3 operator+(const Vector3 &other);
      Vector3 operator-(const Vector3 &other);
      Vector3 operator*(const Vector3 &other);
      Vector3 operator/(const Vector3 &other);
      Vector3 operator*(const float &value);
      Vector3 operator/(const float &value);
      Vector3& operator+=(const Vector3 &other);
      Vector3& operator-=(const Vector3 &other);
      Vector3& operator*=(const Vector3 &other);
      Vector3& operator/=(const Vector3 &other);
      Vector3& operator*=(const float &value);
      Vector3& operator/=(const float &value);
    
      // Bool Operators
      bool operator==(const Vector3 &other);
      bool operator!=(const Vector3 &other);
};

#endif