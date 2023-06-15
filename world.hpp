#ifndef WORLD_HPP
#define WORLD_HPP

#include "ant.hpp"
#include "vector3D.hpp"
#include "colony.hpp"
#include "food.hpp"
#include "pheromonegrid.hpp"
#include "obstaclegrid.hpp"

#include <vector>

class World
{
public:
    World();
    World(const int& worldWidth, const int& worldHeight);

    void update();

    int get_height();
    int get_width();
    std::vector<Ant> get_ants();
    std::vector<Food> get_food_vector();
    Colony get_colony();
    PheromoneGrid get_pheromones();
    int get_grid_scaling();
    bool has_colony();
    std::vector<bool> get_obstacle_vector();
    ObstacleGrid get_obstacles();

    void add_obstacle(const int& x, const int& y, const int& radius);
    void add_obstacle_line(const int& x1, const int& y1, const int& x2, const int& y2, const int& radius);
    void generate_random_caves();

    void erase_line(const int& x1, const int& y1,const int& x2, const int& y2, const int& thickness);
    void erase_vertical_line(const int& x, const int& y1, const int& y2, const int& thickness);
    void erase(const int& x, const int& y, const int& radius);
    void erase_ants(const int& x, const int& y, const int& radius);
    void erase_food(const int& x, const int& y, const int& radius);
    void clear_all();

    void add_ant(const Vector3D& locationVector, const double& orientationAngle);
    void move_ants();
    void turn_ants();

    void diminish_ant_pheromone_strengths();
    void add_ant_pheromones();
    void decay_pheromones();

    void add_colony(const int& x, const int& y);
    void add_food(const int& x, const int& y, const int& quantity);

    void collect_food();
    void drop_food();
    void reset_ant_pheromone_strengths();

protected:
    std::vector<Ant> ants{};
    std::vector<Food> foodVector;
    Colony colony;
    PheromoneGrid pheromones;
    ObstacleGrid obstacles;
    bool hasColony = false;
    int pheromoneGridScaling{1};
    int defaultNumAnts{300};
    int pheromoneSpread{3};
    int reachForFood{5};
    int resetPheromoneDistance{8};
    int height;
    int width;
    double pi = 3.141592654;
};

#endif // WORLD_HPP
