#define _USE_MATH_DEFINES

#include "gtest/gtest.h"
#include "vector3D.hpp"
#include "ant.hpp"
#include "world.hpp"
#include "colony.hpp"
#include "food.hpp"
#include "pheromonegrid.hpp"
#include "obstaclegrid.hpp"

#include <iostream>


//#################################################################
//Ant Tests
//#################################################################

TEST(AntInitialization, GivenAnInitializedAnt_WhenCheckingVelocity_ExpectZero)
{
    Ant testAnt;

    EXPECT_EQ(testAnt.get_speed(), testAnt.get_default_speed());
}

TEST(AntInitialization, GivenAnInitializedAnt_WhenCheckingOrientation_ExpectZero)
{
    Ant testAnt;
    Vector3D goldV{0,0,0};

    EXPECT_EQ(testAnt.get_location(), goldV);
}

TEST(AntInitialization, GivenAnInitializedAnt_WhenCheckingHasFood_ExpectFalse)
{
    Ant testAnt;

    EXPECT_EQ(testAnt.has_food(), false);
}

TEST(AntInitialization, GivenAnAntInitializedWithValues_WhenCheckingValues_ExpectProperValues)
{
    Vector3D initialLocation{1,2,0};
    int initialVelocity{5};
    double initialOrientation{1};

    Ant testAnt{initialLocation,initialOrientation,initialVelocity};

    EXPECT_EQ(testAnt.get_location(), initialLocation);
    EXPECT_EQ(testAnt.get_speed(), initialVelocity);
    EXPECT_EQ(testAnt.get_orientation(), initialOrientation);
}

TEST(AntMove, GivenAnAntInitializedWithVelocityOneOrientationZeroAfterMoving_WhenCheckingLocation_ExpectProperLocation)
{
    Vector3D initialLocation{0,0,0};
    int initialVelocity{1};
    double initialOrientation{0};
    Ant testAnt{initialLocation, initialOrientation, initialVelocity};
    Vector3D goldLocation{1,0,0};

    testAnt.move();

    EXPECT_EQ(testAnt.get_location(), goldLocation);
}

TEST(AntMove, GivenAnAntInitializedWithVelocityTwoOrientationZeroAfterMoving_WhenCheckingLocation_ExpectProperLocation)
{
    Vector3D initialLocation{0,0,0};
    int initialVelocity{2};
    double initialOrientation{0};
    Ant testAnt{initialLocation, initialOrientation, initialVelocity};
    Vector3D goldLocation{2,0,0};

    testAnt.move();

    EXPECT_EQ(testAnt.get_location(), goldLocation);
}

TEST(AntMove, GivenAnAntInitializedWithVelocityOneOrientationPi_AfterMovingWhenCheckingLocation_ExpectProperLocation)
{
    Vector3D initialLocation{0,0,0};
    int initialVelocity{1};
    double initialOrientation{3.14};
    Ant testAnt{initialLocation, initialOrientation, initialVelocity};
    Vector3D goldLocation{-1,0,0};

    testAnt.move();

    EXPECT_NEAR(testAnt.get_location()[0], goldLocation[0],.01);
    EXPECT_NEAR(testAnt.get_location()[1], goldLocation[1],.01);
}

TEST(AntMove, GivenAnAntInitializedWithVelocityOneOrientationFourthPiAfterMoving_WhenCheckingLocation_ExpectProperLocation)
{
    Vector3D initialLocation{0,0,0};
    int initialVelocity{1};
    double initialOrientation{3.14/4};
    Ant testAnt{initialLocation, initialOrientation, initialVelocity};
    Vector3D goldLocation{.707,.707,0};

    testAnt.move();

    EXPECT_NEAR(testAnt.get_location()[0], goldLocation[0],.01);
    EXPECT_NEAR(testAnt.get_location()[1], goldLocation[1],.01);
}

class AntSpy: public Ant
{
public:
    void set_orientation(double newAngle)
    {
        orientationAngle = newAngle;
    }
    void wander(double x)
    {
        orientationAngle += x;
        if (orientationAngle >= 2*M_PI)
        {
            orientationAngle -= 2*M_PI;
        }
        if (orientationAngle < 0)
        {
            orientationAngle += 2*M_PI;
        }
    }
};

TEST(AntWander, GivenAnAntInitializedWithOrientationZero_AfterWanderingOneRadianWhenCheckingOrientation_ExpectCorrectOrientation)
{
    AntSpy testAnt;

    testAnt.wander(1);

    EXPECT_NEAR(testAnt.get_orientation(), 1, .01);
}

TEST(AntWander, GivenAnAntInitializedWithOrientationZero_AfterWanderingThreePiWhenCheckingOrientation_ExpectCorrectOrientation)
{
    AntSpy testAnt;

    testAnt.wander(3*M_PI);

    EXPECT_NEAR(testAnt.get_orientation(), 3.14, .01);
}

TEST(AntWander, GivenAnAntInitializedWithOrientationZero_AfterWanderingNegativePiWhenCheckingOrientation_ExpectCorrectOrientation)
{
    AntSpy testAnt;

    testAnt.wander(-M_PI);

    EXPECT_NEAR(testAnt.get_orientation(), 3.14, .01);
}

TEST(AntWander, GivenAnAntInitializedWithOrientationPi_AfterWanderingNegativePiWhenCheckingOrientation_ExpectCorrectOrientation)
{
    AntSpy testAnt;
    testAnt.set_orientation(M_PI);

    testAnt.wander(-M_PI);

    EXPECT_NEAR(testAnt.get_orientation(), 0, .01);
}

TEST(SetHasFood, GivenAnAntInitialized_AfterSettingHasFoodTrue_ExpectHasFoodTrue)
{
    Ant testAnt;

    EXPECT_FALSE(testAnt.has_food());

    testAnt.set_has_food(true);

    EXPECT_TRUE(testAnt.has_food());

}

TEST(SetHasFood, GivenAnAntThatHasFood_AfterSettingHasFoodFalse_ExpectHasFoodFalse)
{
    Ant testAnt;
    testAnt.set_has_food(true);

    testAnt.set_has_food(false);

    EXPECT_FALSE(testAnt.has_food());

}

TEST(GetOrientationVector, GivenAnAntWithOrientationZero_WhenGettingOrientationVector_ExpectCorrectOrientation)
{
    Vector3D locationVector{0,0,0};
    double orientationAngle{0};
    Ant testAnt{locationVector,orientationAngle,1};
    Vector3D goldOrientation{1,0,0};

    EXPECT_EQ(testAnt.get_orientation_vector(), goldOrientation);
}

TEST(GetOrientationVector, GivenAnAntWithOrientationHalfPi_WhenGettingOrientationVector_ExpectCorrectOrientation)
{
    Vector3D locationVector{0,0,0};
    double orientationAngle{M_PI/2};
    Ant testAnt{locationVector,orientationAngle,1};
    Vector3D orientationVector = testAnt.get_orientation_vector();
    Vector3D goldOrientation{0,1,0};

    EXPECT_NEAR(orientationVector[0], goldOrientation[0], 0.1);
    EXPECT_NEAR(orientationVector[1], goldOrientation[1], 0.1);
}

class PreBuiltPheromones:public testing::Test
{
public:
    PreBuiltPheromones()
    {
        for (int x = 0; x < 50; x++)
        {
            for (int y = 0; y < 50; y++)
            {
                testPheromones.add_food_pheromone(x,y,100);
            }
        }
        for (int x = 0; x < 50; x++)
        {
            for (int y = 50; y < 100; y++)
            {
                testPheromones.add_home_pheromone(x,y,100);
            }
        }
    }

protected:
    PheromoneGrid testPheromones{1000,1000,1};
};

TEST_F(PreBuiltPheromones, GivenPheromoneGridAndAnAntWithoutFoodNearPheromones_AfterTurningToFollowPheromones_ExpectTurned)
{
    Vector3D location{51,25,0};
    double initialOrientation{3*M_PI/4.0};
    Ant testAnt{location,initialOrientation};
    testAnt.set_has_food(false);
    double turnAngle = testAnt.get_pheromone_turn_angle();

    testAnt.turn_towards_pheromones(testPheromones);
    double newOrientation = testAnt.get_orientation();

    EXPECT_NEAR(testAnt.get_orientation(), initialOrientation + turnAngle, 0.1);
}

TEST_F(PreBuiltPheromones, GivenPheromoneGridAndAnAntWithFoodNearPheromones_AfterTurningToFollowPheromones_ExpectTurned)
{
    Vector3D location{51,75,0};
    double initialOrientation{3*M_PI/4.0};
    Ant testAnt{location,initialOrientation};
    testAnt.set_has_food(true);
    double turnAngle = testAnt.get_pheromone_turn_angle();

    testAnt.turn_towards_pheromones(testPheromones);
    double newOrientation = testAnt.get_orientation();

    EXPECT_NEAR(testAnt.get_orientation(), initialOrientation + turnAngle, 0.1);
}

TEST_F(PreBuiltPheromones, GivenPheromoneGridAndAnAntFarFromPheromones_AfterTurningToFollowPheromones_ExpectSameOrientation)
{
    Vector3D location{300,300,0};
    double initialOrientation{3*M_PI/4.0};
    Ant testAnt{location,initialOrientation};

    testAnt.turn_towards_pheromones(testPheromones);
    double newOrientation = testAnt.get_orientation();

    EXPECT_NEAR(testAnt.get_orientation(), initialOrientation, 0.1);
}

TEST(LookingForFood, GivenAFoodVectorAndAnAntNearFood_AfterLookingForFood_ExpectTurnedTowardFood)
{
    std::vector<Food> foodVector;
    foodVector.push_back(Food(100,100,25));
    Vector3D antLocation{100,98,0};
    double initialOrientation = M_PI;
    Ant testAnt{antLocation, initialOrientation};
    double goldOrientation = M_PI/2;

    testAnt.look_for_food(foodVector);

    EXPECT_NEAR(testAnt.get_orientation(), M_PI/2, 0.1);

}

TEST(LookingForFood, GivenAFoodVectorAndAnAntFarFromFood_AfterLookingForFood_ExpectSameOrientation)
{
    std::vector<Food> foodVector;
    foodVector.push_back(Food(100,100,25));
    Vector3D antLocation{500,500,0};
    double initialOrientation = M_PI;
    Ant testAnt{antLocation, initialOrientation};

    testAnt.look_for_food(foodVector);

    EXPECT_NEAR(testAnt.get_orientation(), initialOrientation, 0.1);
}

TEST(LookingForColony, GivenAColonyAndAnAntNearColony_AfterLookingForColony_ExpectTurnedTowardsColony)
{
    Colony testColony{100,100};
    Vector3D antLocation{100,98,0};
    double initialOrientation = M_PI;
    Ant testAnt{antLocation, initialOrientation};
    double goldOrientation = M_PI/2;

    testAnt.look_for_colony(testColony);

    EXPECT_NEAR(testAnt.get_orientation(), goldOrientation, 0.1);
}

TEST(LookingForColony, GivenAFoodVectorAndAnAntFarFromColony_AfterLookingForColony_ExpectSameOrientation)
{
    Colony testColony{100,100};
    Vector3D antLocation{500,500,0};
    double initialOrientation = M_PI;
    Ant testAnt{antLocation, initialOrientation};

    testAnt.look_for_colony(testColony);

    EXPECT_NEAR(testAnt.get_orientation(), initialOrientation, 0.1);
}

class PreBuiltObstacles:public testing::Test
{
public:
    PreBuiltObstacles()
    {
        for (int x = 0; x < 1000; x++)
        {
            testObstacles.add_obstacle(x,500);
        }
        for (int y = 0; y < 1000; y++)
        {
            testObstacles.add_obstacle(500,y);
        }
    }

    int detectionRange = 25;

protected:
    ObstacleGrid testObstacles{1000,1000};
};

TEST_F(PreBuiltObstacles, GivenAnObstacleGridAndALocationWithOrientaionFacingObstacles_AfterTurningToAvoidObstacles_ExpectNewOrientation)
{
    Vector3D location{498,498,0};
    double initialOrientation{0};
    Ant testAnt{location,initialOrientation};
    double turnAngle = testAnt.get_obstacle_turn_angle();

    testAnt.turn_to_avoid_obstacles(testObstacles);
    double newOrientation = testAnt.get_orientation();

    EXPECT_TRUE(abs(newOrientation-initialOrientation) > 0.1);
}

TEST_F(PreBuiltObstacles, GivenAnObstacleGridAndALocationFarFromObstacles_AfterTurningToAvoidObstacles_ExpectInitialOrientation)
{
    Vector3D location{100,100,0};
    double initialOrientation{0};
    Ant testAnt{location,initialOrientation};

    testAnt.turn_to_avoid_obstacles(testObstacles);
    double newOrientation = testAnt.get_orientation();

    EXPECT_TRUE(newOrientation == initialOrientation);
}

TEST(GetAngleToPoint, GivenTwoLocations_AfterCalculatingAngleBetweenThem_ExpectCorrectAngle)
{
    Vector3D location1{0,0,0};
    Vector3D location2{2,2,0};
    double goldAngle = M_PI/4;

    double angle = get_angle_to_point(location1, location2);

    EXPECT_NEAR(angle,goldAngle,0.1);
}

TEST(GetAngleToPoint, GivenTwoLocationsInVerticalLine_AfterCalculatingAngleBetweenThem_ExpectCorrectAngle)
{
    Vector3D location1{0,0,0};
    Vector3D location2{0,2,0};
    double goldAngle = M_PI/2;

    double angle = get_angle_to_point(location1, location2);

    EXPECT_NEAR(angle,goldAngle,0.1);
}

//#################################################################
//World Tests
//#################################################################

TEST(WorldInitialization, GivenAWorldInitializedWithLengthAndWidth_AfterGettingWidthAndLength_ExpectCorrectValues)
{
    World testWorld{100,200};

    EXPECT_EQ(testWorld.get_height(), 200);
    EXPECT_EQ(testWorld.get_width(), 100);
}

TEST(WorldInitialization, GivenAWorldtInitializedWithLengthAndWidth_AfterGettingWidthndLength_ExpectCorrectValues)
{
    World testWorld{55,111};

    EXPECT_EQ(testWorld.get_height(), 111);
    EXPECT_EQ(testWorld.get_width(), 55);
}

TEST(AddingAntsToWorld, AfterAddingAntToAnInitializedWorld_WhenGettingAnts_ExpectCorrectAntData)
{
    World testWorld{100,50};
    Vector3D testAntLocation{0,0,0};

    testWorld.add_ant(testAntLocation,0);
    std::vector<Ant> testWorldAnts = testWorld.get_ants();

    EXPECT_EQ(testWorldAnts.size(), 1);
    EXPECT_EQ(testWorldAnts[0].get_location(), testAntLocation);
    EXPECT_EQ(testWorldAnts[0].get_orientation(), 0);
}

TEST(AddingAntsToWorld, AfterAddingFiveAntsToAWorldInitialized_WhenGettingNumberOfAnts_ExpectFiveAnts)
{
    World testWorld{100,50};
    Vector3D testAntLocation{0,0,0};
    int goldNumAnts = 5;

    for (int i = 0; i < goldNumAnts; i++)
    {
        testWorld.add_ant(testAntLocation,0);
    }
    std::vector<Ant> testWorldAnts = testWorld.get_ants();

    EXPECT_EQ(testWorldAnts.size(), 5);
}

TEST(AddingAntsToWorld, AfterAddingFiveAntsToWorld_WhenGettingFifthAntData_ExpectProperLocationAndOrientation)
{
    World testWorld{100,50};
    Vector3D goldLocation{4,4,0};
    double goldOrientation = 4;

    for (int i = 0; i < 5; i++)
    {
        testWorld.add_ant(Vector3D(i,i,0),i);
    }
    std::vector<Ant> testWorldAnts = testWorld.get_ants();

    EXPECT_EQ(testWorldAnts[4].get_location(), goldLocation);
    EXPECT_EQ(testWorldAnts[4].get_orientation(), goldOrientation);
}

TEST(MovingAntsInWorld, GivenAWorldInitializedWithAnt_AfterMovingAntsWhenGettingAntLocation_ExpectProperLocation)
{
    World testWorld{100,50};
    Vector3D initialLocation{0,0,0};
    double initialOrientation{0};
    testWorld.add_ant(initialLocation, initialOrientation);
    int AntSpeed = testWorld.get_ants()[0].get_default_speed();
    double goldX = initialLocation[0] + AntSpeed*cos(initialOrientation);
    double goldY = initialLocation[1] + AntSpeed*sin(initialOrientation);
    Vector3D goldLocation{goldX,goldY,0};

    testWorld.move_ants();
    std::vector<Ant> testWorldAnts = testWorld.get_ants();

    EXPECT_NEAR(testWorldAnts[0].get_location()[0], goldLocation[0],.01);
    EXPECT_NEAR(testWorldAnts[0].get_location()[1], goldLocation[1],.01);
}

TEST(MovingAntsInWorld, GivenAWorldInitializedWithAntWithOrientationPi_AfterMovingAntsWhenGettingAntLocation_ExpectProperLocation)
{
    World testWorld{100,50};
    Vector3D initialLocation{10,10,0};
    double initialOrientation{3.1415};
    testWorld.add_ant(initialLocation, initialOrientation);
    int AntSpeed = testWorld.get_ants()[0].get_default_speed();
    double goldX = initialLocation[0] + AntSpeed*cos(initialOrientation);
    double goldY = initialLocation[1] + AntSpeed*sin(initialOrientation);
    Vector3D goldLocation{goldX,goldY,0};

    testWorld.move_ants();
    std::vector<Ant> testWorldAnts = testWorld.get_ants();

    EXPECT_NEAR(testWorldAnts[0].get_location()[0], goldLocation[0],.01);
    EXPECT_NEAR(testWorldAnts[0].get_location()[1], goldLocation[1],.01);
}

class PreBuiltWorld:public testing::Test
{
public:
    PreBuiltWorld()
    {
        for (int i = 0; i < 5; i++)
        {
            testWorld.add_ant(Vector3D(i,i,0),i);
        }
        antSpeed = testWorld.get_ants()[0].get_default_speed();
    }
protected:
    World testWorld{1000,900};
    int antSpeed;
};

TEST_F(PreBuiltWorld, AfterMovingAntsInPrebuiltWorld_WhenGettingAntLocations_ExpectProperLocations)
{
    testWorld.move_ants();

    for (int i =0; i<testWorld.get_ants().size(); i++)
    {
        Vector3D initialLocation = Vector3D(i,i,0);
        double orientation = testWorld.get_ants()[i].get_orientation();
        Vector3D orientationVector{cos(orientation), sin(orientation),0};
        Vector3D goldLocation = initialLocation + antSpeed*orientationVector;

        EXPECT_NEAR(testWorld.get_ants()[i].get_location()[0], goldLocation[0], .01);
        EXPECT_NEAR(testWorld.get_ants()[i].get_location()[1], goldLocation[1], .01);
    }
}

TEST_F(PreBuiltWorld, GivenAPrebuiltWorld_AfterAddingFood_ExpectFoodInFoodVector)
{
    testWorld.add_food(100,100,20);
    testWorld.add_food(50,50,0);
    int goldSize = 2;

    std::vector<Food> foodV = testWorld.get_food_vector();

    EXPECT_EQ(foodV.size(), goldSize);
}

TEST_F(PreBuiltWorld, AfterAddingFoodToInitializedPrebuiltWorld_WhenCheckingFoodVectorSize_ExpectOne)
{
    testWorld.add_food(100,50,39);
    std::vector<Food> foodV = testWorld.get_food_vector();

    EXPECT_EQ(foodV.size(), 1);
}

TEST_F(PreBuiltWorld, AfterAddingTwoFoodsToInitializedPrebuiltWorld_WhenCheckingFoodVectorSizeAndFoodQuantities_ExpectProperValues)
{
    testWorld.add_food(100,50,39);
    testWorld.add_food(33,80,1);
    int goldQuantity1 = 39;
    int goldQuantity2 = 1;

    std::vector<Food> testFoodVector = testWorld.get_food_vector();
    int quantity1 = testFoodVector[0].get_quantity();
    int quantity2 = testFoodVector[1].get_quantity();

    EXPECT_EQ(testWorld.get_food_vector().size(), 2);
    EXPECT_EQ(goldQuantity1, quantity1);
    EXPECT_EQ(goldQuantity2, quantity2);
}

TEST_F(PreBuiltWorld, AfterAddingFourFoodsToInitializedPrebuiltWorld_WhenCheckingFoodVectorSizeAndFoodQuantities_ExpectProperValues)
{
    testWorld.add_food(1,2,3);
    testWorld.add_food(4,5,6);
    testWorld.add_food(7,8,9);
    testWorld.add_food(10,11,12);
    int goldQuantity3 = 9;
    int goldQuantity4 = 12;

    std::vector<Food> testFoodVector = testWorld.get_food_vector();
    int quantity3 = testFoodVector[2].get_quantity();
    int quantity4 = testFoodVector[3].get_quantity();

    EXPECT_EQ(testWorld.get_food_vector().size(), 4);
    EXPECT_EQ(goldQuantity3, quantity3);
    EXPECT_EQ(goldQuantity4, quantity4);
}

class PrebuiltWorldSpy: public World
{
public:
    PrebuiltWorldSpy(int worldWidth, int worldHeight)
    {
        height = worldHeight;
        width = worldWidth;
        colony = Colony{worldWidth/2, worldHeight/2};
        pheromones = PheromoneGrid(width, height, pheromoneGridScaling);

        add_ant(Vector3D(5,5,0), 0);
        add_ant(Vector3D(50,50,0), 0);
        ants[1].set_has_food(true);

        indexAnt5 = pheromones.get_index(5,5);
        indexAnt50 = pheromones.get_index(50,50);
    }
    int indexAnt5;
    int indexAnt50;
    std::vector<Ant>* get_ants_pointer(){return &ants;}
    PheromoneGrid* get_pheromone_grid_pointer(){return &pheromones;}
};

TEST(AddingAntPheromones, GivenAPreBuiltWorldWithAnAntWithoutFood_AfterAddingAntPheromones_ExpectProperPheromoneValuesAtAntLocation)
{
    PrebuiltWorldSpy testWorld{100,100};
    int addValue = testWorld.get_ants()[0].get_pheromone_strength();
    int goldHomeValue = addValue;
    int goldFoodValue = 0;

    testWorld.add_ant_pheromones();

    PheromoneGrid* PheromoneGridPtr = testWorld.get_pheromone_grid_pointer();
    std::vector<int> homePheromones = PheromoneGridPtr->get_home_pheromones();
    std::vector<int> foodPheromones = PheromoneGridPtr->get_food_pheromones();
    EXPECT_EQ(homePheromones[testWorld.indexAnt5],goldHomeValue);
    EXPECT_EQ(foodPheromones[testWorld.indexAnt5],goldFoodValue);
}

TEST(AddingAntPheromones, GivenAPreBuiltWorldWithAnAntWithFood_AfterAddingAntPheromones_ExpectProperPheromoneValuesAtAntLocation)
{
    PrebuiltWorldSpy testWorld{100,100};
    int addValue = testWorld.get_ants()[0].get_pheromone_strength();
    int goldHomeValue = 0;
    int goldFoodValue = addValue;

    testWorld.add_ant_pheromones();

    PheromoneGrid* PheromoneGridPtr = testWorld.get_pheromone_grid_pointer();
    std::vector<int> homePheromones = PheromoneGridPtr->get_home_pheromones();
    std::vector<int> foodPheromones = PheromoneGridPtr->get_food_pheromones();
    EXPECT_EQ(homePheromones[testWorld.indexAnt50],goldHomeValue);
    EXPECT_EQ(foodPheromones[testWorld.indexAnt50],goldFoodValue);
}

TEST(Erasing, GivenAPreBuiltWorldWithAntAndFoodAndObstacles_AfterErasing_ExpectNoMoreAntFoodOrObstacles)
{
    World testWorld{500,500};
    int x{50};
    int y{50};
    testWorld.add_ant(Vector3D(x,y,0),0);
    testWorld.add_obstacle(x,y,5);
    testWorld.add_food(x,y,100);

    testWorld.erase(x,y,10);

    EXPECT_EQ(testWorld.get_ants().size(), 0);
    EXPECT_EQ(testWorld.get_food_vector().size(), 0);
    EXPECT_FALSE(testWorld.get_obstacles().check_obstacle(x,y));
}

TEST(ErasingLine, GivenAPreBuiltWorldWithAntAndFoodAndObstacles_AfterErasingLineWithSlope1_ExpectNoMoreAntFoodOrObstacles)
{
    World testWorld{500,500};
    int x{100};
    int y{50};
    testWorld.add_ant(Vector3D(x,y,0),0);
    testWorld.add_obstacle(x,y,5);
    testWorld.add_food(x,y,100);

    testWorld.erase_line(x-25,y-25,x+25,y+25,10);

    EXPECT_EQ(testWorld.get_ants().size(), 0);
    EXPECT_EQ(testWorld.get_food_vector().size(), 0);
    EXPECT_FALSE(testWorld.get_obstacles().check_obstacle(x,y));
}

TEST(ErasingLine, GivenAPreBuiltWorldWithAntAndFoodAndObstacles_AfterErasingLineWithSlopeNegativeOne_ExpectNoMoreAntFoodOrObstacles)
{
    World testWorld{500,500};
    int x{100};
    int y{50};
    testWorld.add_ant(Vector3D(x,y,0),0);
    testWorld.add_obstacle(x,y,5);
    testWorld.add_food(x,y,100);

    testWorld.erase_line(x+25,y+25,x-25,y-25,10);

    EXPECT_EQ(testWorld.get_ants().size(), 0);
    EXPECT_EQ(testWorld.get_food_vector().size(), 0);
    EXPECT_FALSE(testWorld.get_obstacles().check_obstacle(x,y));
}

TEST(ClearAll, GivenAPreBuiltWorldWithAntAndFoodAndObstacles_AfterClearingAll_ExpectNoMoreAntFoodOrObstacles)
{
    World testWorld{500,500};
    int x{50};
    int y{50};
    testWorld.add_ant(Vector3D(x,y,0),0);
    testWorld.add_obstacle(x,y,5);
    testWorld.add_food(x,y,100);

    testWorld.clear_all();

    EXPECT_EQ(testWorld.get_ants().size(), 0);
    EXPECT_EQ(testWorld.get_food_vector().size(), 0);
    EXPECT_FALSE(testWorld.get_obstacles().check_obstacle(x,y));
}

//########################################################
// Food Tests
//########################################################

TEST(ReduceFoodQuantity, AfterReducingTheQuantityOfAFoodWithQuantityOne_WhenGettingFoodQuantity_ExpectZero)
{
    Food testFood{88,2,1};
    int goldQuantity = 0;

    testFood.reduce_quantity(1);
    int testFoodQuantity = testFood.get_quantity();

    EXPECT_EQ(testFoodQuantity, goldQuantity);
}

TEST(ReduceFoodQuantity, AfterReducingTheQuantityOfFoodByTwo_WhenGettingFoodQuantity_ExpectProperQuantity)
{
    Food testFood{88,2,5};
    int goldQuantity = 3;

    testFood.reduce_quantity(2);
    int testFoodQuantity = testFood.get_quantity();

    EXPECT_EQ(testFoodQuantity, goldQuantity);
}

//######################################################
//PheromoneGrid Tests
//######################################################

TEST(PheromoneGridInitialization, AfterInitializingAPheromoneGrid_WhenGettingGridSize_ExpectProperSize)
{
    PheromoneGrid testGrid{100,200, 1};
    int goldWidth = 100+1;
    int goldHeight = 200+1;

    EXPECT_EQ(testGrid.get_grid_width(), goldWidth);
    EXPECT_EQ(testGrid.get_grid_height(), goldHeight);
}

TEST(PheromoneGridInitialization, AfterInitializingAPheromoneGridWithWidth2Height1_WhenGettingGridSize_ExpectProperSize)
{
    PheromoneGrid testGrid{200,100,100};
    int goldWidth = 200/100 + 1;
    int goldHeight = 100/100 + 1;

    EXPECT_EQ(testGrid.get_grid_width(), goldWidth);
    EXPECT_EQ(testGrid.get_grid_height(), goldHeight);
}

TEST(PheromoneGridInitialization, AfterInitializingAPheromoneGridWithWidth2Height1_WhenGettingPheromoneVectorLengths_ExpectTwo)
{
    int width = 2;
    int height = 1;
    int scale = 1;
    int goldLength = (width/scale+1)*(height/scale+1);

    PheromoneGrid testGrid{width,height,scale};
    std::vector<int> toHomePheromones = testGrid.get_home_pheromones();
    std::vector<int> toFoodPheromones = testGrid.get_food_pheromones();

    EXPECT_EQ(toHomePheromones.size(), goldLength);
    EXPECT_EQ(toFoodPheromones.size(), goldLength);
}

TEST(PheromoneGridInitialization, AfterInitializingAPheromoneGridWithWidth100Height200Scale5_WhenGettingPheromoneVectorLengths_ExpectProperLength)
{
    int width = 100;
    int height = 200;
    int scale = 5;
    int goldLength = (width/scale+1)*(height/scale+1);

    PheromoneGrid testGrid{width,height,scale};
    std::vector<int> toHomePheromones = testGrid.get_home_pheromones();
    std::vector<int> toFoodPheromones = testGrid.get_food_pheromones();

    EXPECT_EQ(toHomePheromones.size(), goldLength);
    EXPECT_EQ(toFoodPheromones.size(), goldLength);
}

TEST(PheromoneGridInitialization, AfterInitializingAPheromoneGridWithWidth100Height199_WhenGettingPheromoneVectorLengths_ExpectProperLength)
{
    int goldLength = (100/5+1)*(199/5+1);

    PheromoneGrid testGrid{100,199,5};
    std::vector<int> toHomePheromones = testGrid.get_home_pheromones();
    std::vector<int> toFoodPheromones = testGrid.get_food_pheromones();

    EXPECT_EQ(toHomePheromones.size(), goldLength);
    EXPECT_EQ(toFoodPheromones.size(), goldLength);
}

TEST(PheromoneGridInitialization, AfterInitializingAPheromoneGrid_WhenGettingPheromoneValues_ExpectZeros)
{
    PheromoneGrid testGrid{100,199,1};
    std::vector<int> toHomePheromones = testGrid.get_home_pheromones();
    std::vector<int> toFoodPheromones = testGrid.get_food_pheromones();

    for (int i = 0; i < toHomePheromones.size(); i++)
    {
        EXPECT_EQ(toHomePheromones[i], 0);
        EXPECT_EQ(toFoodPheromones[i], 0);
    }
}

TEST(PheromoneGridGetIndexFunction, AfterInitializingAPheromoneGrid_WhenGettingIndexOfXAndY_ExpectCorrectIndex)
{
    int width = 3;
    int height = 4;
    PheromoneGrid testGrid{width,height,1};
    int x = 2;
    int y = 3;
    int goldIndex = (y+1)*width+x;

    EXPECT_EQ(testGrid.get_index(x,y), goldIndex);
}

TEST(PheromoneGridGetIndexFunction, AfterInitializingAPheromoneGrid_WhenGettingIndexOfOrigin_ExpectZero)
{
    int width = 3;
    int height = 4;
    PheromoneGrid testGrid{width,height,1};
    int x = 0;
    int y = 0;
    int goldIndex = 0;

    EXPECT_EQ(testGrid.get_index(x,y), goldIndex);
}

TEST(PheromoneGridGetLocationFunction, GivenAPheromoneGrid_WhenGettingLocationOfIndex0_ExpectProperValues)
{
    int width = 5;
    int height = 6;
    int scale = 1;
    PheromoneGrid testGrid{width,height,scale};
    int index = 0;
    int goldX = 0;
    int goldY = 0;

    Vector3D location = testGrid.get_location(index);
    int x = location[0];
    int y = location[1];

    EXPECT_EQ(x, goldX);
    EXPECT_EQ(y, goldY);
}

TEST(PheromoneGridGetLocationFunction, GivenAPheromoneGrid_WhenGettingLocationOfIndex_ExpectProperValues)
{
    int width = 15;
    int height = 15;
    int scale = 5;
    PheromoneGrid testGrid{width,height,scale};
    int goldX = 2;
    int goldY = 7;

    int index = 4;
    Vector3D location = testGrid.get_location(index);
    int x = location[0];
    int y = location[1];

    EXPECT_EQ(x, goldX);
    EXPECT_EQ(y, goldY);
}

TEST(PheromoneGridGetLocationFunction, AfterTakingTheLocationOfAnIndex_WhenGettingIndexOfCalculatedLocation_ExpectInitialIndex)
{
    int width = 122;
    int height = 155;
    int scale = 5;
    PheromoneGrid testGrid{width,height,scale};
    int initialIndex = 15;

    Vector3D location = testGrid.get_location(initialIndex);
    int x = location[0];
    int y = location[1];
    int calculatedIndex = testGrid.get_index(x,y);

    EXPECT_EQ(initialIndex, calculatedIndex);
}

TEST(AddHomePheromoneToPheromoneGrid, AfterAddingHomePheromoneToPheromoneGrid_WhenGettingPheromoneStrengthAtLocation_ExpectValue)
{
    PheromoneGrid testGrid{20,20,1};
    int x = 5;
    int y = 7;
    int index = testGrid.get_index(x,y);
    int goldValue = 5;

    testGrid.add_home_pheromone(x,y,goldValue);
    int pheromoneValue = testGrid.get_home_pheromones()[index];

    EXPECT_EQ(pheromoneValue, goldValue);
}

TEST(AddFoodPheromoneToPheromoneGrid, AfterAddingFoodPheromoneToPheromoneGrid_WhenGettingPheromoneStrengthAtLocation_ExpectValue)
{
    PheromoneGrid testGrid{20,20,1};
    int x = 5;
    int y = 7;
    int index = testGrid.get_index(x,y);
    int goldValue = 5;

    testGrid.add_food_pheromone(x,y,goldValue);
    int pheromoneValue = testGrid.get_food_pheromones()[index];

    EXPECT_EQ(pheromoneValue, goldValue);
}

TEST(AddHomePheromoneToPheromoneGrid, AfterAddingHomePheromoneToScaledPheromoneGrid_WhenGettingPheromoneStrengthAtLocation_ExpectProperValue)
{
    PheromoneGrid testGrid{25,25,5};
    int x = 5;
    int y = 7;
    int index = testGrid.get_index(x,y);
    int goldValue = 5;

    testGrid.add_home_pheromone(x,y,goldValue);
    int pheromoneValue = testGrid.get_home_pheromones()[index];

    EXPECT_EQ(pheromoneValue, goldValue);
}

TEST(AddFoodPheromoneToPheromoneGrid, AfterAddingFoodPheromoneToScaledPheromoneGrid_WhenGettingPheromoneStrengthAtLocation_ExpectProperValue)
{
    PheromoneGrid testGrid{27,25,5};
    int x = 5;
    int y = 7;
    int goldValue = 5;

    testGrid.add_food_pheromone(x,y,goldValue);
    int pheromoneValue = testGrid.get_food_pheromone(x,y);

    EXPECT_EQ(pheromoneValue, goldValue);
}

TEST(DecayAllPheromones, AfterDecayingPheromonesOnPheromoneGridWithNoPheromones_WhenGettingPheromoneValues_ExpectZeros)
{
    PheromoneGrid testGrid{5,5,1};
    int decayValue = 5; //defaulted to 1

    testGrid.decay_all_pheromones();

    for (int x = 0; x <= 5;x++)
    {
        for (int y = 0; y <= 5; y++)
        {
            int pheromoneValue = testGrid.get_food_pheromone(x,y);
            EXPECT_EQ(pheromoneValue, 0);
        }
    }
}

TEST(DecayAllPheromones, AfterDecayingPheromonesOnPheromoneGridWithFoodPheromones_WhenGettingPheromoneValues_ExpectProperValues)
{
    PheromoneGrid testGrid{5,5,1};
    testGrid.add_food_pheromone(1,1,1);
    testGrid.add_food_pheromone(2,2,10);
    int decayValue = testGrid.get_pheromone_decay_value();
    int goldValue1 = 0;
    int goldValue2 = 10 - decayValue;

    testGrid.decay_all_pheromones();
    int pheromoneValue1 = testGrid.get_food_pheromone(1,1);
    int pheromoneValue2 = testGrid.get_food_pheromone(2,2);

    EXPECT_EQ(pheromoneValue1, goldValue1);
    EXPECT_EQ(pheromoneValue2, goldValue2);
}

TEST(DecayAllPheromones, AfterDecayingPheromonesOnPheromoneGridWithHomePheromones_WhenGettingPheromoneValues_ExpectProperValues)
{
    PheromoneGrid testGrid{20,20,5};
    int decayValue = testGrid.get_pheromone_decay_value();
    testGrid.add_home_pheromone(1,1,4);
    testGrid.add_home_pheromone(10,10,10);
    int goldValue1{4-decayValue};
    int goldValue2{10-decayValue};

    testGrid.decay_all_pheromones();
    int pheromoneValue1 = testGrid.get_home_pheromone(1,1);
    int pheromoneValue2 = testGrid.get_home_pheromone(10,10);

    EXPECT_EQ(pheromoneValue1, goldValue1);
    EXPECT_EQ(pheromoneValue2, goldValue2);
}

TEST(ClearPheromones, AfterClearingPheromonesOnPheromoneGridWithHomePheromones_WhenGettingPheromoneValues_ExpectZeros)
{
    PheromoneGrid testGrid{20,20,5};
    testGrid.add_home_pheromone(1,1,4);
    testGrid.add_home_pheromone(10,10,10);

    testGrid.clear();
    int pheromoneValue1 = testGrid.get_home_pheromone(1,1);
    int pheromoneValue2 = testGrid.get_home_pheromone(10,10);

    EXPECT_EQ(pheromoneValue1, 0);
    EXPECT_EQ(pheromoneValue2, 0);
}

TEST(ClearPheromones, AfterClearingPheromonesOnPheromoneGridWithFoodPheromones_WhenGettingPheromoneValues_ExpectZeros)
{
    PheromoneGrid testGrid{20,20,5};
    testGrid.add_food_pheromone(1,1,4);
    testGrid.add_food_pheromone(10,10,10);

    testGrid.clear();
    int pheromoneValue1 = testGrid.get_food_pheromone(1,1);
    int pheromoneValue2 = testGrid.get_food_pheromone(10,10);

    EXPECT_EQ(pheromoneValue1, 0);
    EXPECT_EQ(pheromoneValue2, 0);
}

//############################################################
//ObstacleGrid Tests
//############################################################

TEST(ObstacleGridInitialization, GivenAnInitializedObstacleGrid_WhenGettingDimensions_ExpectProperDimensions)
{
    ObstacleGrid testGrid{100,100};
    int goldHeight{100};
    int goldWidth{100};

    EXPECT_EQ(testGrid.get_width(), goldWidth);
    EXPECT_EQ(testGrid.get_height(), goldHeight);
}

TEST(ObstacleGridInitialization, GivenAnUnevenObstacleGrid_WhenGettingDimensions_ExpectProperDimensions)
{
    ObstacleGrid testGrid{55,66};
    int goldWidth{55};
    int goldHeight{66};

    EXPECT_EQ(testGrid.get_width(), goldWidth);
    EXPECT_EQ(testGrid.get_height(), goldHeight);
}

TEST(ObstacleGridInitialization, GivenAnInitializedGrid_WhenGettingVectorLength_ExpectProperDimensions)
{
    ObstacleGrid testGrid{55,66};
    int goldWidth{55};
    int goldHeight{66};

    EXPECT_EQ(testGrid.get_width(), goldWidth);
    EXPECT_EQ(testGrid.get_height(), goldHeight);
}

TEST(GetIndex, GivenAnInitializedGrid_WhenGettingIndexOfOrigin_ExpectZero)
{
    ObstacleGrid testGrid{55,66};

    EXPECT_EQ(testGrid.get_index(0,0), 0);
}

TEST(GetIndex, GivenAnInitializedGrid_WhenGettingIndexOfPoint_ExpectProperPoint)
{
    ObstacleGrid testGrid{2,2};
    int goldIndex{4};

    EXPECT_EQ(testGrid.get_index(1,1), goldIndex);
}

TEST(GetIndex, GivenAnInitializedGrid_WhenGettingIndexOfMaxPoint_ExpectProperPoint)
{
    ObstacleGrid testGrid{2,2};
    int goldIndex{8};

    EXPECT_EQ(testGrid.get_index(2,2), goldIndex);
}

TEST(GetLocation, GivenAnInitializedGrid_WhenGettingLocationFromIndex_ExpectCorrectIndex)
{
    ObstacleGrid testGrid{2,2};
    int index{4};
    Vector3D goldLocation{1,1,0};

    EXPECT_EQ(testGrid.get_location(index), goldLocation);
}

TEST(AddAndCheckObstacle, GivenAnInitializedGridWithAnObstacleAdded_WhenCheckingForObstacleAtLocation_ExpectTrue)
{
    ObstacleGrid testGrid{100,100};

    testGrid.add_obstacle(10,5);

    EXPECT_EQ(testGrid.check_obstacle(10,5), true);
}

TEST(AddAndCheckObstacle, GivenAnInitializedGridWithNoObstaclesAdded_WhenCheckingForObstacleAtLocation_ExpectFalse)
{
    ObstacleGrid testGrid{100,100};

    EXPECT_EQ(testGrid.check_obstacle(10,5), false);
}

TEST(MultipleAddAndCheckObstacle, GivenAnInitializedGridWithTwoObstaclesAdded_WhenCheckingForObstacleAtLocations_ExpectProperValues)
{
    ObstacleGrid testGrid{100,100};

    testGrid.add_obstacle(20,5);
    testGrid.add_obstacle(0,0);

    EXPECT_EQ(testGrid.check_obstacle(20,5), true);
    EXPECT_EQ(testGrid.check_obstacle(0,0), true);
    EXPECT_EQ(testGrid.check_obstacle(3,3), false);
}

TEST(RemoveObstacle, GivenAnInitializedGridWithTwoObstaclesAdded_AfterRemovingObstaclesAtLocations_ExpectNoObstacles)
{
    ObstacleGrid testGrid{100,100};
    testGrid.add_obstacle(20,5);
    testGrid.add_obstacle(0,0);

    testGrid.remove_obstacle(20,5);
    testGrid.remove_obstacle(0,0);

    EXPECT_EQ(testGrid.check_obstacle(20,5), false);
    EXPECT_EQ(testGrid.check_obstacle(0,0), false);
}

TEST(getObstacleLocations, GivenAnInitializedGridWithTwoObstaclesAdded_WhenGettingObstacleLocations_ExpectVectorLengthOfTwo)
{
    ObstacleGrid testGrid{100,100};
    testGrid.add_obstacle(20,5);
    testGrid.add_obstacle(0,0);
    int goldLength{2};

    std::vector<Vector3D> obstacleLocations = testGrid.get_obstacle_locations();

    EXPECT_EQ(obstacleLocations.size(), goldLength);
}

TEST(getObstacleLocations, GivenAnInitializedGridWithAnObstacleAdded_WhenCheckingObstacleLocations_ExpectVectorLengthOfTwo)
{
    ObstacleGrid testGrid{100,100};
    testGrid.add_obstacle(11,7);
    int goldX{11};
    int goldY{7};

    std::vector<Vector3D> obstacleLocations = testGrid.get_obstacle_locations();

    EXPECT_EQ(obstacleLocations[0][0], goldX);
    EXPECT_EQ(obstacleLocations[0][1], goldY);
}

TEST(FillBorders, GivenAnInitializedGridWithBordersFilled_WhenCheckingIfObstaclesAtEdges_ExpectTrue)
{
    ObstacleGrid testGrid{2,2};

    testGrid.fill_borders();

    EXPECT_TRUE(testGrid.check_obstacle(0,0));
    EXPECT_TRUE(testGrid.check_obstacle(0,1));
    EXPECT_TRUE(testGrid.check_obstacle(0,2));
    EXPECT_TRUE(testGrid.check_obstacle(2,0));
    EXPECT_TRUE(testGrid.check_obstacle(2,1));
    EXPECT_TRUE(testGrid.check_obstacle(2,2));
    EXPECT_TRUE(testGrid.check_obstacle(1,0));
    EXPECT_TRUE(testGrid.check_obstacle(1,2));
}

TEST(FillBorders, GivenAnInitializedGridWithBordersFilled_WhenCheckingIfObstaclesInMiddle_ExpectFalse)
{
    ObstacleGrid testGrid{100,100};

    testGrid.fill_borders();

    EXPECT_TRUE(testGrid.check_obstacle(0,0));
    EXPECT_TRUE(testGrid.check_obstacle(100,0));
    EXPECT_FALSE(testGrid.check_obstacle(5,5));
}

class TestObstacles:public testing::Test
{
public:
    TestObstacles()
    {
        for (int x = 0; x < 1000; x++)
        {
            testObstacles.add_obstacle(x,500);
        }
        for (int y = 0; y < 1000; y++)
        {
            testObstacles.add_obstacle(500,y);
        }
    }

    int detectionRange = 25;

protected:
    ObstacleGrid testObstacles{1000,1000};
};


TEST_F(TestObstacles, GivenAnObstacleGrid_WhenCheckingIfObstacleInFrontOfLocationAndOrientation_ExpectTrue)
{
    Vector3D location{500.0 - detectionRange, 500.0 - detectionRange, 0};
    double orientation = 0;

    EXPECT_TRUE(testObstacles.check_for_obstacle_front(location,orientation,detectionRange));
}

TEST_F(TestObstacles, GivenAnObstacleGrid_WhenCheckingIfObstacleInFrontOfLocationJustPastWallAndFacingWall_ExpectTrue)
{
    Vector3D location{500.0 + detectionRange, 500.0 + detectionRange, 0};
    double orientation = M_PI;

    EXPECT_TRUE(testObstacles.check_for_obstacle_front(location,orientation,detectionRange));
}

TEST_F(TestObstacles, GivenAnObstacleGrid_WhenCheckingIfObstacleInFrontOfLocationFarFromObstacles_ExpectFalse)
{
    Vector3D location{499.0 - detectionRange, 499.0 - detectionRange, 0};
    double orientation = 0;

    EXPECT_FALSE(testObstacles.check_for_obstacle_front(location, orientation, detectionRange));
}

TEST_F(TestObstacles, GivenAnObstacleGrid_WhenCheckingIfObstacleInFrontOfLocationNearObstaclesButOrientationFacingAway_ExpectFalse)
{
    Vector3D location{499.1,499,0};
    double orientation = -M_PI;

    EXPECT_FALSE(testObstacles.check_for_obstacle_front(location,orientation,detectionRange));
}

TEST_F(TestObstacles, GivenAnObstacleGrid_WhenCheckingObstacleDistanceFromALocation_ExpectCorrectDistance)
{
    Vector3D location{499,499,0};
    double orientation = -3.14/6;
    int goldDistance{1};

    EXPECT_EQ(testObstacles.check_obstacle_distance(location, orientation, detectionRange), goldDistance);
}

TEST_F(TestObstacles, GivenAnObstacleGridAndALocationTwoAwayFromObstacle_WhenCheckingObstacleDistance_ExpectTwo)
{
    Vector3D location{498,498,0};
    double orientation = 0;
    int goldDistance{2};

    int calcDistance = testObstacles.check_obstacle_distance(location,orientation,detectionRange);

    EXPECT_EQ(calcDistance, goldDistance);
}

TEST(AddObstacleCircle, AfterAddingObstacleCircleToEmptyGrid_WhenCheckingIfObstaclesAtLocation_ExpectTrue)
{
    ObstacleGrid testGrid{100,100};
    int x{50};
    int y{40};
    int radius{10};

    testGrid.add_obstacle_circle(x,y,radius);

    EXPECT_TRUE(testGrid.check_obstacle(x+radius-1,y));
    EXPECT_TRUE(testGrid.check_obstacle(x,y+radius-1));
    EXPECT_TRUE(testGrid.check_obstacle(x,y));
}

TEST(AddObstacleCircle, AfterAddingObstacleCircleToEmptyGrid_WhenCheckingIfObstaclesFarFromAddLocation_ExpectFalse)
{
    ObstacleGrid testGrid{100,100};
    int x{50};
    int y{40};
    int radius{10};

    testGrid.add_obstacle_circle(x,y,radius);

    EXPECT_FALSE(testGrid.check_obstacle(x+radius+1,y));
    EXPECT_FALSE(testGrid.check_obstacle(x,y+radius+1));
}

TEST(AddObstacleLine, AfterDrawingObstacleLineOnAnEmptyInitializedGrid_WhenCheckingIfObstaclesInMiddleOfLine_ExpectTrue)
{
    ObstacleGrid testGrid{100,100};
    int x1{20};
    int y1{20};
    int x2{50};
    int y2{50};
    int thickness{5};

    testGrid.add_obstacle_line(x1,y1,x2,y2,thickness);

    EXPECT_TRUE(testGrid.check_obstacle(40,40));
    EXPECT_FALSE(testGrid.check_obstacle(5,5));
}

TEST(AddObstacleLine, AfterDrawingVerticalObstacleLineOnAnEmptyInitializedGrid_WhenCheckingIfObstaclesInMiddleOfLine_ExpectTrue)
{
    ObstacleGrid testGrid{100,100};
    int x1{20};
    int y1{20};
    int x2{20};
    int y2{50};
    int thickness{5};

    testGrid.add_obstacle_line(x1,y1,x2,y2,thickness);

    EXPECT_TRUE(testGrid.check_obstacle(20,40));
    EXPECT_FALSE(testGrid.check_obstacle(5,5));
}

TEST(SmoothObstacleGrid, AfterSmoothingAPointWithAnObstacleWithNoNearbyObstacles_WhenCheckingIfObstacleAtLocation_ExpectFalse)
{
    ObstacleGrid testGrid{100,100};
    testGrid.add_obstacle(50,50);

    testGrid.smooth();

    EXPECT_FALSE(testGrid.check_obstacle(50,50));
}

TEST(SmoothObstacleGrid, AfterSmoothingAPointWithAnObstacleWithNearbyObstacles_WhenCheckingIfObstacleAtLocation_ExpectFalse)
{
    ObstacleGrid testGrid{100,100};
    int radius = 50;
    testGrid.add_obstacle_circle(50,50,radius);

    testGrid.smooth();

    EXPECT_TRUE(testGrid.check_obstacle(50,50));
}

TEST(EraseObstacle, AfterErasingObstacles_WhenCheckingIfObstacleAtLocation_ExpectFalse)
{
    ObstacleGrid testGrid{100,100};
    int radius = 50;
    testGrid.add_obstacle_circle(50,50,radius);

    testGrid.erase(50,50,radius);

    EXPECT_FALSE(testGrid.check_obstacle(50,50));
    EXPECT_FALSE(testGrid.check_obstacle(50+radius,50));
    EXPECT_FALSE(testGrid.check_obstacle(50,50+radius));
}

TEST(ClearObstacleGrid, AfterClearingObstacles_WhenCheckingIfObstacleAtLocation_ExpectFalse)
{
    ObstacleGrid testGrid{100,100};
    int radius = 50;
    testGrid.add_obstacle_circle(50,50,radius);

    testGrid.clear();

    EXPECT_FALSE(testGrid.check_obstacle(50,50));
    EXPECT_FALSE(testGrid.check_obstacle(70,70));
}
