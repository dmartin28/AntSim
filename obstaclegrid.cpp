#include "obstaclegrid.hpp"

#include <cmath>
#include <math.h>
#include <random>

ObstacleGrid::ObstacleGrid(){}

ObstacleGrid::ObstacleGrid(const int& width, const int& height): worldWidth(width),worldHeight(height)
{
    gridWidth = width + 1;
    gridHeight = height + 1;
    obstacles = std::vector<bool>((gridWidth)*(gridHeight));
    fill(obstacles.begin(), obstacles.end(), false);
}

int ObstacleGrid::get_width()
{
    return worldWidth;
}

int ObstacleGrid::get_height()
{
    return worldHeight;
}

int ObstacleGrid::get_index(const double& x, const double& y) const
{
    int roundedX = int(x+.5);
    int roundedY = int(y+.5);
    return (roundedY)*gridWidth + roundedX;
}

Vector3D ObstacleGrid::get_location(const int& index)
{
    int x = index % gridWidth;
    int y = index / gridWidth;
    return Vector3D(x,y,0);
}

std::vector<Vector3D> ObstacleGrid::get_obstacle_locations()
{
    std::vector<Vector3D> obstacleLocations;
    for (int i = 0; i < obstacles.size(); i++)
    {
        if (obstacles[i] == true)
        {
            obstacleLocations.push_back(get_location(i));
        }
    }
    return obstacleLocations;
}

std::vector<bool> ObstacleGrid::get_obstacle_vector()
{
    return obstacles;
}

void ObstacleGrid::add_obstacle_line(const int &x1, const int &y1, const int &x2, const int &y2, const int &thickness)
{

    double startX = x2;
    double startY = y2;
    double endX = x1;
    double endY = y1;

    if (abs(startX - endX) < verticalLineLimit)
    {
        add_vertical_obstacle_line(x1,y1,y2,thickness);
    }
    else
    {
        if (x1 <= x2)
        {
            startX = x1;
            startY = y1;
            endX = x2;
            endY = y2;
        }

        double slope = (endY-startY)/(endX-startX);

        for (double x = startX; x <= endX; x = x + 1)
        {
            double y = startY + slope*(x-startX);
            add_obstacle_circle(x,y,thickness);

        }
    }
}

void ObstacleGrid::add_vertical_obstacle_line(const int& x1, const int& y1, const int& y2, const int& thickness)
{
    if (y1 < y2)
    {
        for (int y = y1; y <= y2; y++)
        {
            add_obstacle_circle(x1,y,thickness);
        }
    }
    else
    {
        for (int y = y2; y <= y1; y++)
        {
            add_obstacle_circle(x1,y,thickness);
        }
    }
}

void ObstacleGrid::add_obstacle_circle(const int &originX, const int &originY, const double &radius)
{
    for (double x = -radius; x < radius; x++)
    {
        for (double y = 0; (pow(y,2) + pow(x,2)) < pow(radius,2); y++)
        {
            int newX1 = originX + x;
            int newY1 = originY + y;
            int newY2 = originY - y;

            if (0 < newX1 && newX1 < worldWidth)
            {
                if (0 < newY1 && newY1 < worldHeight )
                {
                    add_obstacle(newX1, newY1);
                }
                if (0 < newY2 && newY2 < worldHeight )
                {
                    add_obstacle(newX1, newY2);
                }
            }
        }
    }
}

void ObstacleGrid::generate_random_caves()
{
    randomly_fill_map(caveFillPercent);
    fill_borders();
    for (int i = 0; i < smoothIterations; i++)
    {
        smooth();
    }
}

void ObstacleGrid::randomly_fill_map(const int &fillPercent)
{
    clear();
    for (int x = 0; x < gridWidth-randomSquareSize; x = x+randomSquareSize)
    {
        for (int y = 0; y < gridHeight-randomSquareSize; y = y+randomSquareSize)
        {
            double rand = generate_random_percent();
            if (rand < caveFillPercent)
            {
                fill_square(x,y);
            }
        }
    }
}

void ObstacleGrid::smooth()
{
    std::vector<bool> obstacleCopy = obstacles;
    for (int x = 0; x < gridWidth-randomSquareSize; x = x+randomSquareSize)
    {
        for (int y = 0; y < gridHeight-randomSquareSize; y = y+randomSquareSize)
        {
            int wallCount = get_neighbor_wall_count(x,y, obstacleCopy);
            if (wallCount >= 12)
            {
                fill_square(x,y);
            }
            else
            {
                unfill_square(x,y);
            }
        }
    }
}

double ObstacleGrid::get_neighbor_wall_count(const int &x, const int &y, const std::vector<bool>& obstacleCopy)
{
    double wallCount = 0;
    for (int neighbourX = x - 2*randomSquareSize; neighbourX <= x + 2*randomSquareSize; neighbourX+=randomSquareSize)
    {
        for (int neighbourY = y - 2*randomSquareSize; neighbourY <= y + 2*randomSquareSize; neighbourY+=randomSquareSize)
        {
            if (neighbourX >= 0 && neighbourX < gridWidth && neighbourY >= 0 && neighbourY < gridHeight)
            {
                if (neighbourX != x || neighbourY != y)
                {
                    int index = get_index(neighbourX,neighbourY);
                    wallCount += obstacleCopy[index];
                }
            }
            else
            {
                wallCount++;
            }
        }
    }
    return wallCount;
}

void ObstacleGrid::fill_square(const int &x, const int &y)
{
    for (int i = 0; i < randomSquareSize; i++)
    {
        for (int j = 0; j < randomSquareSize; j++)
        {
            add_obstacle(x+i, y+j);
        }
    }
}

void ObstacleGrid::unfill_square(const int &x, const int &y)
{
    for (int i = 0; i < randomSquareSize; i++)
    {
        for (int j = 0; j < randomSquareSize; j++)
        {
            remove_obstacle(x+i, y+j);
        }
    }
}

void ObstacleGrid::erase(const int &originX, const int &originY, const double &radius)
{
    for (double x = -radius; x < radius; x++)
    {
        for (double y = 0; (pow(y,2) + pow(x,2)) < pow(radius,2); y++)
        {
            int newX1 = originX + x;
            int newY1 = originY + y;
            int newY2 = originY - y;

            if (0 < newX1 && newX1 < worldWidth)
            {
                if (0 < newY1 && newY1 < worldHeight )
                {
                    remove_obstacle(newX1, newY1);
                }
                if (0 < newY2 && newY2 < worldHeight )
                {
                    remove_obstacle(newX1, newY2);
                }
            }
        }
    }
}

void ObstacleGrid::clear()
{
    for (int i = 0; i < obstacles.size(); i++)
    {
        obstacles[i] = false;
    }
    fill_borders();
}

bool ObstacleGrid::check_obstacle(const double& x, const double& y) const
{
    int index = get_index(x,y);
    return obstacles[index];
}

bool ObstacleGrid::check_for_obstacle_front(const Vector3D& locationVector, const double& orientation, const int& detectionRange) const
{
    Vector3D orientationVector{cos(orientation),sin(orientation),0};
    bool obstacleInFront{false};
    int timestep = 0;

    while (obstacleInFront==false && timestep <= detectionRange)
    {
        double x{locationVector[0]+timestep*orientationVector[0]};
        double y{locationVector[1]+timestep*orientationVector[1]};

        if (check_obstacle(x,y))
        {
            obstacleInFront = true;
        }
        timestep++;
    }
    return obstacleInFront;
}

int ObstacleGrid::check_obstacle_distance(const Vector3D& locationVector, const double& orientation, const int& detectionRange) const
{
    bool obstacle{false};
    int distanceForward{0};

    Vector3D orientationVector{cos(orientation),sin(orientation),0};

    while (obstacle == false && distanceForward <= detectionRange)
    {
        double x{locationVector[0]+distanceForward*orientationVector[0]};
        double y{locationVector[1]+distanceForward*orientationVector[1]};

        if (check_obstacle(x,y))
        {
            obstacle = true;
        }
        distanceForward++;
    }
    return distanceForward - 1;
}

void ObstacleGrid::add_obstacle(const int& x, const int& y)
{
    int index = get_index(x,y);
    obstacles[index] = true;
}

void ObstacleGrid::remove_obstacle(const int& x, const int& y)
{
    int index = get_index(x,y);
    obstacles[index] = false;
}

void ObstacleGrid::fill_borders()
{
    for (int x = 0; x <= worldWidth; x++)
    {
        for (int i = 0; i < borderWidth; i++)
        {
            add_obstacle(x,0+i);
            add_obstacle(x,worldHeight-i);
        }
    }
    for (int y = 0; y <= worldHeight; y++)
    {
        for (int i = 0; i < borderWidth; i++)
        {
            add_obstacle(i,y);
            add_obstacle(worldWidth-i,y);
        }
    }
}

double generate_random_percent()
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0, 100);
    return dis(gen);
}
