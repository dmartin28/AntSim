#ifndef FOOD_HPP
#define FOOD_HPP

#include "vector3D.hpp"

class Food
{
public:
    Food(const int& x, const int& y, const int& initialQuantity);
    int get_quantity();
    Vector3D get_location() const;
    void reduce_quantity(const int& amount);

protected:
    int quantity;
    Vector3D location;
};

#endif // FOOD_HPP
