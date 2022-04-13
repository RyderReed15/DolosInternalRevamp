#pragma once

#ifndef VECTOR_H
#define VECTOR_H

#include <iostream>
#define DECL_ALIGN(x) __declspec(align(x))
#define ALIGN16 DECL_ALIGN(16)
#define ALIGN16_POST DECL_ALIGN(16)
#ifndef PI
#define PI 3.14159265358979323846f
#endif

class Vector
{
public:
    Vector(void)
    {
        Invalidate();
    }
    Vector(float X, float Y, float Z)
    {
        x = X;
        y = Y;
        z = Z;
    }
    
    
    Vector(const float* clr)
    {
        x = clr[0];
        y = clr[1];
        z = clr[2];
    }

    void Init(float ix = 0.0f, float iy = 0.0f, float iz = 0.0f)
    {
        x = ix; y = iy; z = iz;
    }
    bool IsValid() const
    {
        return std::isfinite(x) && std::isfinite(y) && std::isfinite(z);
    }
    void Invalidate()
    {
        x = y = z = std::numeric_limits<float>::infinity();
    }

    float& operator[](int i)
    {
        return ((float*)this)[i];
    }
    float operator[](int i) const
    {
        return ((float*)this)[i];
    }

    void Zero()
    {
        x = y = z = 0.0f;
    }

    bool operator==(const Vector& src) const
    {
        return (src.x == x) && (src.y == y) && (src.z == z);
    }
    bool operator!=(const Vector& src) const
    {
        return (src.x != x) || (src.y != y) || (src.z != z);
    }

    Vector& operator+=(const Vector& v)
    {
        x += v.x; y += v.y; z += v.z;
        return *this;
    }
    Vector& operator-=(const Vector& v)
    {
        x -= v.x; y -= v.y; z -= v.z;
        return *this;
    }
    Vector& operator*=(float fl)
    {
        x *= fl;
        y *= fl;
        z *= fl;
        return *this;
    }
    Vector& operator*=(const Vector& v)
    {
        x *= v.x;
        y *= v.y;
        z *= v.z;
        return *this;
    }
    Vector& operator/=(const Vector& v)
    {
        x /= v.x;
        y /= v.y;
        z /= v.z;
        return *this;
    }
    Vector& operator+=(float fl)
    {
        x += fl;
        y += fl;
        z += fl;
        return *this;
    }
    Vector& operator/=(float fl)
    {
        x /= fl;
        y /= fl;
        z /= fl;
        return *this;
    }
    Vector& operator-=(float fl)
    {
        x -= fl;
        y -= fl;
        z -= fl;
        return *this;
    }

    std::ostream& operator<< (std::ostream& output) {
        output << "X: " << x << " Y: " << y << " Z: " << z;
        return output;
    }
    Vector Forward(float dist) {
        Vector out;
        out.x = cos(y / 180.f * PI) * cos(x / 180.f * PI) * dist;;
        out.y = cos(x / 180.f * PI) * sin(y / 180.f * PI) * dist;
        
        out.z = sin(-x / 180.f * PI) * dist;
        return out;
    }
    Vector Left(float dist) {
        Vector out;
        out.x = sin(-y / 180.f * PI) * dist;
        out.y = cos(-y / 180.f * PI) * dist;
        
        out.z = 0;
        return out;
    }
    Vector Up(float dist) {
        Vector out;
        out.x = cos(y / 180.f * PI) * sin(x / 180.f * PI) * dist;

        out.y = sin(x / 180.f * PI) * sin(y / 180.f * PI) * dist;
        
        out.z = cos(x / 180.f * PI) * dist;
        return out;
    }
    // @param Vector to calculate dot product with
    float DotProduct(Vector vec) {
        return x * vec.x + y * vec.y + z * vec.z;
    }

    float Magnitude() {
        return (float)sqrtf(x * x + y * y + z * z);
    }
    //@param Vector to calculate distance to
    float Distance(Vector vec) {
        return (*this - vec).Magnitude();
    }

    Vector Normalize() {
        return *this / Magnitude();
    }
    float LengthSqr(void) const
    {
        return (x * x + y * y + z * z);
    }
    float Length2D() const
    {
        return sqrt(x * x + y * y);
    }
    
    /*
 Moves a certain percent between to points
 @param destination to move to
 @param percent to move towards the destination
 @return location of partial movement towards destination
*/
    Vector Lerp(Vector vec, float percent) {
        Vector Dist = (*this - vec);
        if (Dist.y < -180) {
            Dist.y += 360;
        }
        else if (Dist.y > 180) {
            Dist.y -= 360;
        }
        Dist = Dist * percent;
        return Dist;
    }
    Vector ToAngles() {
        while (abs(y) > 180) {
            y += 360 * (y > 0 ? -1 : 1);
        }

        if (abs(x) > 89) {
            x = 89.f * (x > 0 ? 1 : -1);
        }
        return *this;
    }

    Vector& operator=(const Vector& vOther)
    {
        x = vOther.x; y = vOther.y; z = vOther.z;
        return *this;
    }

    Vector operator-(void) const
    {
        return Vector(-x, -y, -z);
    }
    Vector operator+(const Vector& v) const
    {
        return Vector(x + v.x, y + v.y, z + v.z);
    }
    Vector operator-(const Vector& v) const
    {
        return Vector(x - v.x, y - v.y, z - v.z);
    }
    Vector operator*(float fl) const
    {
        return Vector(x * fl, y * fl, z * fl);
    }
    Vector operator*(const Vector& v) const
    {
        return Vector(x * v.x, y * v.y, z * v.z);
    }
    Vector operator/(float fl) const
    {
        return Vector(x / fl, y / fl, z / fl);
    }
    Vector operator/(const Vector& v) const
    {
        return Vector(x / v.x, y / v.y, z / v.z);
    }

    float x, y, z;
};

inline Vector operator*(float lhs, const Vector& rhs)
{
    return rhs * lhs;
}
inline Vector operator/(float lhs, const Vector& rhs)
{
    return rhs / lhs;
}

class ALIGN16 VectorAligned : public Vector
{
public:
    inline VectorAligned(void) {};

    inline VectorAligned(Vector X, Vector Y, Vector Z)
    {
        //Init(X, Y, Z);
    }

    explicit VectorAligned(const Vector& vOther)
    {
        Init(vOther.x, vOther.y, vOther.z);
    }

    VectorAligned& operator=(const Vector& vOther)
    {
        Init(vOther.x, vOther.y, vOther.z);
        return *this;
    }

    float w;	// this space is used anyway
} ALIGN16_POST;
#endif // !VECTOR_H