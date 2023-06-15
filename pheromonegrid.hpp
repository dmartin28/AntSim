#ifndef PHEROMONEGRID_HPP
#define PHEROMONEGRID_HPP

#include "vector3D.hpp"

#include<vector>

class PheromoneGrid
{
public:
    PheromoneGrid();
    PheromoneGrid(const int& width, const int& height, const int& scale);

    int get_index(const int& x, const int& y) const;
    Vector3D get_location(const int& index);
    int get_grid_width();
    int get_scaling();
    int get_grid_height();
    int get_pheromone_decay_value();

    void clear();

    std::vector<int> get_home_pheromones();
    std::vector<int> get_food_pheromones();
    int get_home_pheromone(const int& x, const int& y) const;
    int get_food_pheromone(const int& x, const int& y) const;

    void add_home_pheromone(const int& x, const int& y, const int& pheromoneValue);
    void add_food_pheromone(const int& x, const int& y, const int& pheromoneValue);
    void spread_home_pheromone(const int& x, const int& y, const int& pheromoneValue, const int& spread);
    void spread_food_pheromone(const int& x, const int& y, const int& pheromoneValue, const int& spread);
    void decay_all_pheromones();
    void decay_pheromone_vector(std::vector<int>& pheromonesVector);

    double average_food_pheromones_right(const Vector3D& locationVector, const double& orientation, const int& smellRange) const;
    double average_food_pheromones_left(const Vector3D& locationVector, const double& orientation, const int& smellRange) const;
    double average_home_pheromones_right(const Vector3D& locationVector, const double& orientation, const int& smellRange) const;
    double average_home_pheromones_left(const Vector3D& locationVector, const double& orientation, const int& smellRange) const;

protected:
    int worldWidth;
    int worldHeight;
    int gridWidth;
    int gridHeight;
    int scaling;
    int exponentialDecayLimit{200};
    std::vector<int> toHomePheromones;
    std::vector<int> toFoodPheromones;
    int smellSamplingResolution{1};
    int pheromoneDecayValue{1};
};

#endif // PHEROMONEGRID_HPP
