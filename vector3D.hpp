#ifndef MYLIBRARY_HPP
#define MYLIBRARY_HPP

#include <array>

class Vector3D
{
private:

    std::array<double,3> xyzArray;

public:
    Vector3D(double x, double y, double z);
    Vector3D();
    void set_value(double x, double y, double z);
    const double& operator[](unsigned int index) const;
    double& operator[](unsigned int index);
};

bool operator==(const Vector3D vector1, const Vector3D vector2);
Vector3D operator+(const Vector3D v1, const Vector3D v2);
Vector3D operator-(const Vector3D v1, const Vector3D v2);
Vector3D operator*(const Vector3D v1, const double constant);
Vector3D operator*(const double constant, const Vector3D v1);
Vector3D operator/(const Vector3D v1, const double constant);

double length_squared(const Vector3D v1);
double length(const Vector3D v1);
Vector3D normalize(const Vector3D v1);
Vector3D cross_product(const Vector3D v1, const Vector3D v2);
double dot_product(const Vector3D v1, const Vector3D v2);

#endif
