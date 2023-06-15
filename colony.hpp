#ifndef COLONY_H
#define COLONY_H

#include "vector3D.hpp"

class Colony
{
public:
    Colony(int x, int y);
    Colony();

    Vector3D get_location() const;

protected:
    Vector3D location;

};

#endif // COLONY_H
