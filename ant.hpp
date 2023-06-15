#ifndef ANT_H
#define ANT_H

#include "vector3D.hpp"
#include "pheromonegrid.hpp"
#include "obstaclegrid.hpp"
#include "colony.hpp"
#include "food.hpp"

class Ant
{
public:

    Ant();
    Ant(const Vector3D& locationVector, const double& initialOrientation);
    Ant(const Vector3D& locationVector, const double& initialOrientation, const int& customSpeed);

    Vector3D get_location();
    double get_orientation();
    Vector3D get_orientation_vector();
    double get_speed();
    double get_default_speed();
    double get_pheromone_strength();
    double get_obstacle_turn_angle();
    double get_pheromone_turn_angle();
    double get_sight_range();

    void diminish_pheromone_strength();
    void reset_pheromone_strength();

    bool has_food();
    void set_has_food(const bool& newHasFood);
    void set_orientation(const double& newOrientation);

    void move();
    void turn(const PheromoneGrid& pheromones, const ObstacleGrid& obstacles, const std::vector<Food>& foodVector, const Colony& colony);
    void wander();
    void turn_towards_pheromones(const PheromoneGrid& pheromones);
    void look_for_food(const std::vector<Food>& foodVector);
    void look_for_colony(const Colony& colony);
    void turn_to_avoid_obstacles(const ObstacleGrid& obstacles);
    void corner_evasive_action(const int& rightDistance, const int& leftDistance);
    void turn_if_at_boundary(const int& width, const int& height);

protected:
    Vector3D locationVector{0,0,0};
    double orientationAngle{0};
    double defaultSpeed{3};
    double speed{defaultSpeed};
    bool hasFood{false};
    double wanderRange{3.14/6};
    double maxPheromoneStrength{400};
    double pheromoneStrength{0};
    double diminishPheromoneValue{1.5};
    int smellRange{12};
    double pheromoneTurnAngle{3.14/8};
    int sightRange{30};
    double obstacleTurnAngle{3.14/6};
    int cornerTolerance{0};

    double pi = 3.141592654;
};

double get_angle_to_point(const Vector3D& initialLocation, const Vector3D& targetLocation);

double generate_random_double(double minValue, double maxValue);

#endif // ANT_H
