#define _USE_MATH_DEFINES

#include "ant.hpp"

#include <cmath>
#include <math.h>
#include <random>

Ant::Ant(){}

Ant::Ant(const Vector3D& initialLocation, const double& initialOrientation):
    locationVector(initialLocation), orientationAngle(initialOrientation){}

Ant::Ant(const Vector3D &initialLocation, const double &initialOrientation, const int &customSpeed):
    locationVector(initialLocation), orientationAngle(initialOrientation), speed(customSpeed){}

Vector3D Ant::get_location()
{
    return locationVector;
}

double Ant::get_orientation()
{
    return orientationAngle;
}

Vector3D Ant::get_orientation_vector()
{
    return Vector3D(cos(orientationAngle), sin(orientationAngle), 0);
}

double Ant::get_speed()
{
    return speed;
}

double Ant::get_default_speed()
{
    return defaultSpeed;
}

double Ant::get_pheromone_strength()
{
    return pheromoneStrength;
}

double Ant::get_obstacle_turn_angle()
{
    return obstacleTurnAngle;
}

double Ant::get_pheromone_turn_angle()
{
    return pheromoneTurnAngle;
}

double Ant::get_sight_range()
{
    return sightRange;
}

void Ant::diminish_pheromone_strength()
{
    if (pheromoneStrength > diminishPheromoneValue)
    {
        pheromoneStrength -= diminishPheromoneValue;
    }
    else
    {
        pheromoneStrength = 0;
    }
}

void Ant::reset_pheromone_strength()
{
    pheromoneStrength = maxPheromoneStrength;
}

bool Ant::has_food()
{
    return hasFood;
}

void Ant::set_has_food(const bool& newHasFood)
{
    hasFood = newHasFood;
}

void Ant::set_orientation(const double& newOrientation)
{
    orientationAngle = newOrientation;
}

void Ant::move()
{
    locationVector = locationVector + speed*get_orientation_vector();
}

void Ant::turn(const PheromoneGrid& pheromones, const ObstacleGrid& obstacles, const std::vector<Food>& foodVector, const Colony& colony)
{
    wander();

    turn_towards_pheromones(pheromones);

    if (hasFood)
    {
        look_for_colony(colony);
    }
    else
    {
        look_for_food(foodVector);
    }

    turn_to_avoid_obstacles(obstacles);
}

void Ant::wander()
{
    orientationAngle = orientationAngle - wanderRange/2 + generate_random_double(0, wanderRange);

    if (orientationAngle >= 2*M_PI)
    {
        orientationAngle -= 2*M_PI;
    }
    if (orientationAngle < 0)
    {
        orientationAngle += 2*M_PI;
    }
}

void Ant::turn_towards_pheromones(const PheromoneGrid &pheromones)
{
    int averageRight{0};
    int averageLeft{0};

    if (hasFood)
    {
        averageRight = pheromones.average_home_pheromones_right(locationVector, orientationAngle, smellRange);
        averageLeft = pheromones.average_home_pheromones_left(locationVector, orientationAngle, smellRange);
    }
    else
    {
        averageRight = pheromones.average_food_pheromones_right(locationVector, orientationAngle, smellRange);
        averageLeft = pheromones.average_food_pheromones_left(locationVector, orientationAngle, smellRange);
    }

    if (averageRight > averageLeft)
    {
        orientationAngle += pheromoneTurnAngle;
    }
    else if (averageRight < averageLeft)
    {
        orientationAngle -= pheromoneTurnAngle;
    }
}

void Ant::look_for_food(const std::vector<Food>& foodVector)
{
    int x = locationVector[0];
    int y = locationVector[1];

    for (int i = 0; i < foodVector.size(); i++)
    {
        Vector3D foodLocation = foodVector[i].get_location();
        if (abs(foodLocation[0]- x) < sightRange)
        {
            if (abs(foodLocation[1]-y) < sightRange)
            {
                orientationAngle = get_angle_to_point(locationVector, foodLocation);
            }
        }
    }
}

void Ant::look_for_colony(const Colony& colony)
{
    int x = locationVector[0];
    int y = locationVector[1];

    Vector3D colonyLocation = colony.get_location();
    if (abs(colonyLocation[0]- x) < sightRange)
    {
        if (abs(colonyLocation[1]-y) < sightRange)
        {
            orientationAngle = get_angle_to_point(locationVector, colonyLocation);
        }
    }
}

void Ant::turn_to_avoid_obstacles(const ObstacleGrid &obstacles)
{
    bool obstacleInFront = obstacles.check_for_obstacle_front(locationVector, orientationAngle, sightRange);
    if (obstacleInFront)
    {
        double rightAngle = orientationAngle + obstacleTurnAngle;
        double leftAngle = orientationAngle - obstacleTurnAngle;
        int rightDistance = obstacles.check_obstacle_distance(locationVector, rightAngle, sightRange);
        int leftDistance = obstacles.check_obstacle_distance(locationVector, leftAngle, sightRange);

        if (rightDistance < leftDistance - cornerTolerance)
        {
            orientationAngle -= obstacleTurnAngle;
        }
        else if (leftDistance < rightDistance - cornerTolerance)
        {
            orientationAngle += obstacleTurnAngle;
        }
        else
        {
            corner_evasive_action(rightDistance, leftDistance);
        }
    }
}

void Ant::corner_evasive_action(const int &rightDistance, const int &leftDistance)
{
    if (rightDistance > sightRange-1 && leftDistance > sightRange-1)
    {
        double randomDouble = generate_random_double(0,1);
        if (randomDouble > .5)
        {
            orientationAngle += 3.14/4;
        }
        else
        {
            orientationAngle -= 3.14/4;
        }
    }
    else
    {
        orientationAngle += 3.14;
    }
}

void Ant::turn_if_at_boundary(const int& width, const int& height)
{
    if (locationVector[0] > width - 2*speed)
    {
        orientationAngle += 3.14;
        move();
    }
    if (locationVector[0] < 2*speed)
    {
        orientationAngle += 3.14;
        move();
    }
    if (locationVector[1] < 2*speed)
    {
        orientationAngle += 3.14;
        move();
    }
    if (locationVector[1] > height - 2*speed)
    {
        orientationAngle += 3.14;
        move();
    }
}

double get_angle_to_point(const Vector3D &initialLocation, const Vector3D &targetLocation)
{
    Vector3D orientationV = initialLocation - targetLocation;
    double dx = orientationV[0];
    double dy = orientationV[1];

    if (dx >= 0)
    {
        return atan(dy/dx)+ 3.14159;
    }
    else
    {
        return atan(dy/dx);
    }
}

double generate_random_double(double minValue, double maxValue)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(minValue, maxValue);
    return dis(gen);
}
