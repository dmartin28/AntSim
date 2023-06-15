#include "colony.hpp"

Colony::Colony()
{
    location = Vector3D(0,0,0);
}

Vector3D Colony::get_location() const
{
    return location;
}

Colony::Colony(int x, int y)
{
    location = Vector3D(x,y,0);
}

