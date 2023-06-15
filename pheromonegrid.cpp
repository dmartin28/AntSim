#include "pheromonegrid.hpp"

PheromoneGrid::PheromoneGrid()
{

}

PheromoneGrid::PheromoneGrid(const int& width, const int& height, const int& scale):
    worldWidth(width), worldHeight(height), scaling(scale)
{
    gridWidth = width/scaling +1;
    gridHeight = height/scaling +1;

    toHomePheromones = std::vector<int>(gridWidth*gridHeight);
    toFoodPheromones = std::vector<int>(gridWidth*gridHeight);
    fill(toHomePheromones.begin(), toHomePheromones.end(), 0);
    fill(toFoodPheromones.begin(), toFoodPheromones.end(), 0);
}

int PheromoneGrid::get_index(const int& x, const int& y) const
{
    int gridX;
    int gridY;

    if (x % scaling > scaling/2.0)
    {
        gridX = x/scaling + 1;
    }
    else
    {
        gridX = x/scaling;
    }

    if (y % scaling > scaling/2.0)
    {
        gridY = y/scaling + 1;
    }
    else
    {
        gridY = y/scaling;
    }

    return gridY*gridWidth + gridX;
}

Vector3D PheromoneGrid::get_location(const int& index)
{
    int column = index % gridWidth;
    int row = index / gridWidth;
    int x = (column+0.5)*scaling;
    int y = (row+0.5)*scaling;

    return Vector3D(x,y,0);
}

int PheromoneGrid::get_grid_width()
{
    return gridWidth;
}

int PheromoneGrid::get_scaling()
{
    return scaling;
}

int PheromoneGrid::get_grid_height()
{
    return gridHeight;
}

int PheromoneGrid::get_pheromone_decay_value()
{
    return pheromoneDecayValue;
}

void PheromoneGrid::clear()
{
    for (int i = 0; i < toHomePheromones.size(); i++)
    {
        toHomePheromones[i] = 0;
        toFoodPheromones[i] = 0;
    }
}

std::vector<int> PheromoneGrid::get_home_pheromones()
{
    return toHomePheromones;
}

std::vector<int> PheromoneGrid::get_food_pheromones()
{
    return toFoodPheromones;
}

int PheromoneGrid::get_home_pheromone(const int& x, const int& y) const
{
    int index = get_index(x,y);
    return toHomePheromones[index];
}

int PheromoneGrid::get_food_pheromone(const int& x, const int& y) const
{
    int index = get_index(x,y);
    return toFoodPheromones[index];
}

void PheromoneGrid::add_home_pheromone(const int& x, const int& y, const int& pheromoneValue)
{
    int index = get_index(x,y);
    toHomePheromones[index] += pheromoneValue;
}

void PheromoneGrid::add_food_pheromone(const int& x, const int& y, const int& pheromoneValue)
{
    int index = get_index(x,y);
    toFoodPheromones[index] += pheromoneValue;
}

void PheromoneGrid::spread_home_pheromone(const int &x, const int &y, const int &pheromoneValue, const int &spread)
{
    for (int xNew = x-spread; xNew < x+spread; xNew++)
    {
        for (int yNew = y-spread; yNew < y +spread; yNew++)
        {
            add_home_pheromone(xNew,yNew,pheromoneValue);
        }
    }
}

void PheromoneGrid::spread_food_pheromone(const int &x, const int &y, const int &pheromoneValue, const int &spread)
{
    for (int xNew = x-spread; xNew < x+spread; xNew++)
    {
        for (int yNew = y-spread; yNew < y +spread; yNew++)
        {
            add_food_pheromone(xNew,yNew,pheromoneValue);
        }
    }
}

void PheromoneGrid::decay_all_pheromones()
{
    decay_pheromone_vector(toHomePheromones);
    decay_pheromone_vector(toFoodPheromones);
}

void PheromoneGrid::decay_pheromone_vector(std::vector<int> &pheromonesVector)
{
    for (int i = 0; i < pheromonesVector.size(); i++)
    {
        if (pheromonesVector[i] >= pheromoneDecayValue*exponentialDecayLimit)
        {
            pheromonesVector[i] = pheromonesVector[i] - 3*pheromoneDecayValue*pheromonesVector[i]/(exponentialDecayLimit);
        }
        else if (pheromonesVector[i] > pheromoneDecayValue)
        {
            pheromonesVector[i] = pheromonesVector[i] - pheromoneDecayValue;
        }
        else
        {
            pheromonesVector[i] = 0;
        }
    }
}

double PheromoneGrid::average_food_pheromones_right(const Vector3D& locationVector, const double& orientation, const int& smellRange) const
{
    double rightAngle = orientation + 3.14/4.0;

    int numValues{0};
    int sumValues{0};
    for (int forwardDistance = 0; forwardDistance < smellRange; forwardDistance++)
    {
        for (double sideWidth = 0; sideWidth < smellRange; sideWidth += smellSamplingResolution)
        {
            double newX = locationVector[0] + forwardDistance*cos(orientation) + sideWidth*cos(rightAngle);
            double newY = locationVector[1] + forwardDistance*sin(orientation) + sideWidth*sin(rightAngle);
            if (newX > 0 && newX < gridWidth && newY > 0 && newY < gridHeight)
            {
                numValues += 1;
                sumValues += get_food_pheromone(newX, newY);
            }
        }
    }
    return sumValues/numValues;
}

double PheromoneGrid::average_food_pheromones_left(const Vector3D& locationVector, const double& orientation, const int& smellRange) const
{
    double leftAngle = orientation - 3.14/4.0;

    int numValues{0};
    int sumValues{0};
    for (int forwardDistance = 0; forwardDistance < smellRange; forwardDistance++)
    {
        for (double sideWidth = 0; sideWidth < smellRange; sideWidth = sideWidth + smellSamplingResolution)
        {
            double newX = locationVector[0]+forwardDistance*cos(orientation)+sideWidth*cos(leftAngle);
            double newY = locationVector[1]+forwardDistance*sin(orientation)+sideWidth*sin(leftAngle);
            if (newX > 0 && newX < gridWidth && newY > 0 && newY < gridHeight)
            {
                numValues += 1;
                sumValues += get_food_pheromone(newX, newY);
            }
        }
    }
    return sumValues/numValues;
}

double PheromoneGrid::average_home_pheromones_right(const Vector3D& locationVector, const double& orientation, const int& smellRange) const
{
    double rightAngle = orientation + 3.14/4.0;

    int numValues{0};
    int sumValues{0};
    for (int forwardDistance = 0; forwardDistance < smellRange; forwardDistance++)
    {
        for (double sideWidth = 0; sideWidth < smellRange; sideWidth = sideWidth + smellSamplingResolution)
        {
            double newX = locationVector[0] + forwardDistance*cos(orientation) + sideWidth*cos(rightAngle);
            double newY = locationVector[1] + forwardDistance*sin(orientation) + sideWidth*sin(rightAngle);
            if (newX > 0 && newX < gridWidth && newY > 0 && newY < gridHeight)
            {
                numValues += 1;
                sumValues += get_home_pheromone(newX, newY);
            }
        }
    }
    return sumValues/numValues;
}

double PheromoneGrid::average_home_pheromones_left(const Vector3D& locationVector, const double& orientation, const int& smellRange) const
{
    double leftAngle = orientation - 3.14/4.0;

    int numValues{0};
    int sumValues{0};
    for (int forwardDistance = 0; forwardDistance < smellRange; forwardDistance++)
    {
        for (double sideWidth = 0; sideWidth < smellRange; sideWidth = sideWidth + smellSamplingResolution)
        {
            double newX = locationVector[0]+forwardDistance*cos(orientation)+sideWidth*cos(leftAngle);
            double newY = locationVector[1]+forwardDistance*sin(orientation)+sideWidth*sin(leftAngle);
            if (newX > 0 && newX < gridWidth && newY > 0 && newY < gridHeight)
            {
                numValues += 1;
                sumValues += get_home_pheromone(newX, newY);
            }
        }
    }
    return sumValues/numValues;
}
