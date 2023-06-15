#include "world.hpp"

#include <cmath>
#include <math.h>
#include <random>


World::World()
{
    height = 100;
    width = 100;
    pheromones = PheromoneGrid(width, height, pheromoneGridScaling);
    obstacles = ObstacleGrid{height,width};
    obstacles.fill_borders();
}

World::World(const int& worldWidth, const int& worldHeight)
{
    height = worldHeight;
    width = worldWidth;
    pheromones = PheromoneGrid(width, height, pheromoneGridScaling);
    obstacles = ObstacleGrid{width,height};
    obstacles.fill_borders();
}

void World::update()
{
    move_ants();
    turn_ants();
    add_ant_pheromones();
    decay_pheromones();
    collect_food();
    drop_food();
    diminish_ant_pheromone_strengths();
    reset_ant_pheromone_strengths();
}

int World::get_height()
{
    return height;
}

int World::get_width()
{
    return width;
}

std::vector<Ant> World::get_ants()
{
    return ants;
}

std::vector<Food> World::get_food_vector()
{
    return foodVector;
}

Colony World::get_colony()
{
    return colony;
}

PheromoneGrid World::get_pheromones()
{
    return pheromones;
}

int World::get_grid_scaling()
{
    return pheromones.get_scaling();
}

bool World::has_colony()
{
    return hasColony;
}

std::vector<bool> World::get_obstacle_vector()
{
    return obstacles.get_obstacle_vector();
}

ObstacleGrid World::get_obstacles()
{
    return obstacles;
}

void World::add_obstacle(const int& x, const int& y, const int& radius)
{
    obstacles.add_obstacle_circle(x,y,radius);
}

void World::add_obstacle_line(const int& x1, const int& y1, const int& x2, const int& y2, const int& radius)
{
    obstacles.add_obstacle_line(x1,y1,x2,y2,radius);
}

void World::generate_random_caves()
{
    obstacles.generate_random_caves();
}

void World::erase_line(const int& x1, const int& y1,const int& x2, const int& y2, const int& thickness)
{
    double startX = x2;
    double startY = y2;
    double endX = x1;
    double endY = y1;
    int verticalLineLimit = 8;

    if (abs(startX - endX) < verticalLineLimit)
    {
        erase_vertical_line(x1,y1,y2,thickness);
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
            erase(x,y,thickness);
        }
    }
}

void World::erase_vertical_line(const int &x, const int &y1, const int &y2, const int &thickness)
{
    if (y1 < y2)
    {
        for (int y = y1; y <= y2; y++)
        {
            erase(x,y,thickness);
        }
    }
    else
    {
        for (int y = y2; y <= y1; y++)
        {
            erase(x,y,thickness);
        }
    }
}

void World::erase(const int& x, const int& y, const int& radius)
{
    obstacles.erase(x,y,radius);
    erase_ants(x,y,radius);
    erase_food(x,y,radius);
}

void World::erase_ants(const int& x, const int& y, const int& radius)
{
    for (int i = 0; i < ants.size(); i++)
    {
        Vector3D antLocation = ants[i].get_location();
        double antX = antLocation[0];
        double antY = antLocation[1];
        double distance = sqrt(pow((antX-x), 2) + pow((antY-y), 2));
        if (distance < radius)
        {
            ants.erase(ants.begin()+i);
        }
    }
}

void World::erase_food(const int& x, const int& y, const int& radius)
{
    for (int i = 0; i < foodVector.size(); i++)
    {
        Vector3D foodLocation = foodVector[i].get_location();
        double foodX = foodLocation[0];
        double foodY = foodLocation[1];
        double distance = sqrt(pow((foodX-x), 2) + pow((foodY-y), 2));
        if (distance < radius)
        {
            foodVector.erase(foodVector.begin()+i);
        }
    }
}

void World::clear_all()
{
    ants.clear();
    foodVector.clear();
    pheromones.clear();
    obstacles.clear();
    hasColony = false;
}

void World::add_ant(const Vector3D& locationVector, const double& orientationAngle)
{
    ants.push_back(Ant(locationVector, orientationAngle));
}

void World::move_ants()
{
    for (int i = 0; i < ants.size(); i++)
    {
        ants[i].move();
        ants[i].turn_if_at_boundary(width, height);
    }
}

void World::turn_ants()
{
    for (int i = 0; i < ants.size(); i++)
    {
        ants[i].turn(pheromones, obstacles, foodVector, colony);
    }
}

void World::diminish_ant_pheromone_strengths()
{
    for (int i = 0; i < ants.size(); i++)
    {
        ants[i].diminish_pheromone_strength();
    }
}

void World::add_ant_pheromones()
{
    for (int i = 0; i < ants.size(); i++)
    {
        int x = ants[i].get_location()[0];
        int y = ants[i].get_location()[1];
        int pheromoneAddValue = ants[i].get_pheromone_strength();

        if (ants[i].has_food() == true)
        {
            pheromones.spread_food_pheromone(x,y,pheromoneAddValue,pheromoneSpread);

        }
        else
        {
            pheromones.spread_home_pheromone(x,y,pheromoneAddValue,pheromoneSpread);
        }
    }
}

void World::decay_pheromones()
{
    pheromones.decay_all_pheromones();
}

void World::add_colony(const int &x, const int &y)
{
    colony = Colony{x, y};
    hasColony = true;
    for (int i = 0; i < defaultNumAnts; i++)
    {
        add_ant(colony.get_location(),i);
    }
}

void World::add_food(const int& x, const int& y, const int& quantity)
{
    if (x > 0 && x < width - 5)
    {
        if (y > 0 && y < height - 5)
        {
            foodVector.push_back(Food(x,y,quantity));
        }
    }
}

void World::collect_food()
{
    for (int i = 0; i < ants.size(); i++)
    {
        int j{0};
        Vector3D antLocation = ants[i].get_location();

        while (j < foodVector.size() && ants[i].has_food() == false)
        {
            Vector3D foodLocation = foodVector[j].get_location();
            bool nearFood = abs(antLocation[0]-foodLocation[0]) < reachForFood && abs(antLocation[1]-foodLocation[1]) < reachForFood;

            if (nearFood)
            {
                ants[i].set_has_food(true);
                ants[i].reset_pheromone_strength();
                double initialOrientation = ants[i].get_orientation();
                ants[i].set_orientation(initialOrientation + 3.14);

                foodVector[j].reduce_quantity(1);
                if (foodVector[j].get_quantity() < 1)
                {
                    foodVector.erase(foodVector.begin()+j);
                }
            }
            j++;
        }
    }
}

void World::drop_food()
{
    for (int i =0 ; i < ants.size(); i++)
    {
        if (ants[i].has_food())
        {
            Vector3D antLocation = ants[i].get_location();
            Vector3D colonyLocation = colony.get_location();

            if (abs(antLocation[0]-colonyLocation[0]) < reachForFood)
            {
                if (abs(antLocation[1]-colonyLocation[1]) < reachForFood)
                {
                    ants[i].set_has_food(false);
                    ants[i].set_orientation(ants[i].get_orientation() + 3.14);
                    ants[i].reset_pheromone_strength();
                }
            }
        }
    }
}

void World::reset_ant_pheromone_strengths()
{
    for (int i =0 ; i < ants.size(); i++)
    {
        if (ants[i].has_food() == false)
        {
            Vector3D antLocation = ants[i].get_location();
            Vector3D colonyLocation = colony.get_location();
            if (abs(antLocation[0]-colonyLocation[0]) < resetPheromoneDistance)
            {
                if (abs(antLocation[1]-colonyLocation[1]) < resetPheromoneDistance)
                {
                    ants[i].reset_pheromone_strength();
                }
            }
        }
    }
}
