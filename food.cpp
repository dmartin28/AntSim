#include "food.hpp"

Food::Food(const int& x, const int& y, const int& initialQuantity)
{
    location = Vector3D(x, y, 0);
    quantity = initialQuantity;
}

int Food::get_quantity()
{
    return quantity;
}

Vector3D Food::get_location() const
{
    return location;
}

void Food::reduce_quantity(const int& amount)
{
    quantity = quantity - amount;
}
