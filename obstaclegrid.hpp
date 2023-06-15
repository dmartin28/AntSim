#ifndef OBSTACLEGRID_HPP
#define OBSTACLEGRID_HPP

#include "vector3D.hpp"

#include<vector>

class ObstacleGrid
{
public:
    ObstacleGrid();
    ObstacleGrid(const int& width, const int& height);

    int get_width();
    int get_height();
    int get_index(const double& x, const double& y) const;
    Vector3D get_location(const int& index);
    std::vector<Vector3D> get_obstacle_locations();
    std::vector<bool> get_obstacle_vector();

    void add_vertical_obstacle_line(const int& x1, const int& y1, const int& y2, const int& thickness);
    void add_obstacle_line(const int &x1, const int &y1, const int &x2, const int &y2, const int &thickness);
    void add_obstacle_circle(const int& x, const int& y, const double& radius);

    void generate_random_caves();
    void randomly_fill_map(const int& fillPercent);
    void smooth();
    double get_neighbor_wall_count(const int &x, const int &y, const std::vector<bool>& obstacleCopy);
    void fill_square(const int& x, const int& y);
    void unfill_square(const int& x, const int& y);

    void erase(const int& x, const int& y, const double& radius);
    void clear();

    bool check_obstacle(const double& x, const double& y) const;
    bool check_for_obstacle_front(const Vector3D& locationVector, const double& orientation, const int& detectionRange) const;
    int check_obstacle_distance(const Vector3D& locationVector, const double& orientation, const int& detectionRange) const;

    void add_obstacle(const int& x, const int& y);
    void remove_obstacle(const int& x, const int& y);
    void fill_borders();

private:
    int worldWidth;
    int worldHeight;
    int gridWidth;
    int gridHeight;
    int borderWidth{3};
    int caveFillPercent{43};
    int smoothIterations{10};
    int randomSquareSize{5};
    int verticalLineLimit{8};

    std::vector<bool> obstacles;
};

double generate_random_percent();
#endif // OBSTACLEGRID_HPP
