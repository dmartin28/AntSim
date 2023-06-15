#include "vector3D.hpp"
#include <math.h>
#include <stdexcept>

Vector3D::Vector3D(double x, double y, double z)
{
    set_value(x,y,z);
}

Vector3D::Vector3D()
{
    set_value(0,0,0);
}

void Vector3D::set_value(double x, double y, double z)
{
    xyzArray[0] = x;
    xyzArray[1] = y;
    xyzArray[2] = z;
}

const double& Vector3D::operator[](unsigned int index) const
{
    return xyzArray[index];
}

double& Vector3D::operator[](unsigned int index)
{
    return xyzArray[index];
}

bool operator==(const Vector3D v1, const Vector3D v2)
{
    if (v1[0] == v2[0] && v1[1] == v2[1] && v1[2] == v2[2])
    {
        return true;
    }
    else
    {
        return false;
    }
}

Vector3D operator+(const Vector3D v1, const Vector3D v2)
{
    return Vector3D(v1[0]+v2[0], v1[1]+v2[1], v1[2]+v2[2]);
}

Vector3D operator-(const Vector3D v1, const Vector3D v2)
{
    return Vector3D(v1[0]-v2[0], v1[1]-v2[1], v1[2]-v2[2]);
}

Vector3D operator*(const double constant, const Vector3D v1)
{
    return Vector3D(v1[0]*constant, v1[1]*constant, v1[2]*constant);
}

Vector3D operator*(const Vector3D v1, const double constant)
{
    return constant * v1;
}

Vector3D operator/(const Vector3D v1, const double constant)
{
    return Vector3D(v1[0]/constant, v1[1]/constant, v1[2]/constant);
}

double length_squared(const Vector3D v1)
{
    return v1[0]*v1[0] + v1[1]*v1[1] + v1[2]*v1[2];
}

double length(const Vector3D v1)
{
    return sqrt(length_squared(v1));
}

Vector3D normalize(const Vector3D v1)
{
    double L = length(v1);
    return Vector3D(v1[0]/L,v1[1]/L,v1[2]/L);
}

Vector3D cross_product(const Vector3D v1, const Vector3D v2)
{
    double x = v1[1]*v2[2] - v1[2]*v2[1];
    double y = v1[2]*v2[0] - v1[0]*v2[2];
    double z = v1[0]*v2[1] - v1[1]*v2[0];
    return Vector3D(x,y,z);
}

double dot_product(const Vector3D v1, const Vector3D v2)
{
    return v1[0]*v2[0] + v1[1]*v2[1] + v1[2]*v2[2];
}
