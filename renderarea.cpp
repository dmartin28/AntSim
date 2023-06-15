#include "renderarea.hpp"

RenderArea::RenderArea(QWidget *parent, World* world)
    : QWidget{parent}, worldPtr{world}
{
    setBackgroundRole(QPalette::Base);
    setAutoFillBackground(true);

    worldWidth = worldPtr->get_width();
    worldHeight = worldPtr->get_height();

    generate_obstacle_image_vector();
    initialize_pheromone_images();
}

void RenderArea::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);
    paint_obstacles(&painter);
    paint_pheromones(&painter);
    paint_ants(&painter);
    paint_food(&painter);
    paint_colony(&painter);
}

void RenderArea::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        QPoint clickPoint =  event->pos();
        int x = clickPoint.x();
        int y = clickPoint.y();

        if (currentAddObject == food)
        {
            worldPtr->add_food(x,y,100);
        }
        if (currentAddObject == obstacle)
        {
            add_obstacle(x,y,clickPoint);
        }
        if (currentAddObject == ant)
        {
            add_ant(x,y);
        }
        if (currentAddObject == erase)
        {
            start_erasing(x,y,clickPoint);
        }
        if (currentAddObject == colony)
        {
            worldPtr->add_colony(x,y);
        }
    }
}

void RenderArea::mouseMoveEvent(QMouseEvent *event)
{
    if (scribbling == true)
    {
        QPoint currentPoint = event->pos();
        int x1 = lastPoint.x();
        int y1 = lastPoint.y();
        int x2 = currentPoint.x();
        int y2 = currentPoint.y();
        lastPoint = currentPoint;

        if (currentAddObject == obstacle)
        {
            worldPtr->add_obstacle_line(x1,y1,x2,y2,brushRadius);
            generate_obstacle_image_vector();
        }
        if (currentAddObject == erase)
        {
            worldPtr->erase_line(x1,y1,x2,y2,brushRadius);
            generate_obstacle_image_vector();
        }
    }
}

void RenderArea::mouseReleaseEvent(QMouseEvent *event)
{
    scribbling = false;
}

void RenderArea::paint_ants(QPainter* painter)
{
    std::vector<Ant> ants = worldPtr->get_ants();
    for (int i = 0; i < ants.size(); i++)
    {
        int antX = ants[i].get_location()[0];
        int antY = ants[i].get_location()[1];
        double antOrientation = (180.0/M_PI)*ants[i].get_orientation() + 90;
        painter->translate(antX,antY);
        painter->rotate(antOrientation);
        QRect antRect{-antSize/2,-antSize/2,antSize,antSize};

        if (ants[i].has_food())
        {
            antWithFoodIcon.paint(painter, antRect);
        }
        else
        {
            antIcon.paint(painter, antRect);
        }

        painter->rotate(-antOrientation);
        painter->translate(-antX,-antY);
    }
}

void RenderArea::paint_food(QPainter *painter)
{
    painter->setBrush(Qt::green);
    std::vector<Food> foodVector = worldPtr->get_food_vector();

    for (int i = 0; i < foodVector.size(); i++)
    {
        int x = foodVector[i].get_location()[0];
        int y = foodVector[i].get_location()[1];
        int quantity = foodVector[i].get_quantity();
        QRect foodRect = get_food_rect(x,y,quantity);
        painter->drawEllipse(foodRect);
    }
}

QRect RenderArea::get_food_rect(const int& x, const int& y, const int& quantity)
{
    int width = minFoodSize*std::sqrt(quantity);
    return QRect(x-width/2, y - width/2, width, width);
}

void RenderArea::paint_colony(QPainter *painter)
{
    if (worldPtr->has_colony())
    {
        painter->setBrush(QBrush("brown"));
        Vector3D location = worldPtr->get_colony().get_location();
        int x = location[0];
        int y = location[1];
        QRect colonyRect{x-colonySize/2, y-colonySize/2, colonySize, colonySize};
        painter->drawEllipse(colonyRect);
    }
}

void RenderArea::paint_pheromones(QPainter *painter)
{
    PheromoneGrid pheromones = worldPtr->get_pheromones();

    int width= pheromones.get_grid_width();
    int height = pheromones.get_grid_height();
    int intSize = sizeof(int);
    int numberOfBytesPerWidth{width*intSize};

    generate_home_pheromones_image_vector();
    generate_food_pheromones_image_vector();

    const uchar* homeImageData = reinterpret_cast<const unsigned char*>(homeImageInts.data());
    const uchar* foodImageData = reinterpret_cast<const unsigned char*>(foodImageInts.data());

    QImage homeImage{homeImageData,width,height,numberOfBytesPerWidth,QImage::Format_ARGB32};
    QImage foodImage{foodImageData,width,height,numberOfBytesPerWidth,QImage::Format_ARGB32};

    painter->drawImage(QRect{0,0,worldWidth,worldHeight}, foodImage);
    painter->drawImage(QRect{0,0,worldWidth,worldHeight}, homeImage);
}

void RenderArea::paint_obstacles(QPainter* painter)
{
    int width= worldWidth+1;
    int height = worldHeight+1;
    int intSize = sizeof(int);
    int numberOfBytesPerWidth{width*intSize};

    const uchar* obstacleImageData = reinterpret_cast<const unsigned char*>(obstacleImageInts.data());
    QImage obstacleImage = QImage(obstacleImageData,width,height,numberOfBytesPerWidth,QImage::Format_ARGB32);
    painter->drawImage(QRect{0,0,worldWidth,worldHeight}, obstacleImage);
}

void RenderArea::generate_obstacle_image_vector()
{
    std::vector<bool> obstacleVector = worldPtr->get_obstacle_vector();
    obstacleImageInts = std::vector<int>(obstacleVector.size());
    fill(obstacleImageInts.begin(), obstacleImageInts.end(), 0);

    for (int i = 0; i<obstacleVector.size(); i++)
    {
        if (obstacleVector[i] == true)
        {
            set_rgba_value(obstacleImageInts.data()+i,0,0,0,255);
        }
    }
}

void RenderArea::initialize_pheromone_images()
{
    PheromoneGrid pheromones = worldPtr->get_pheromones();
    int pheromonesLength = pheromones.get_food_pheromones().size();
    homeImageInts = std::vector<int>(pheromonesLength,0);
    foodImageInts = std::vector<int>(pheromonesLength,0);
}

void RenderArea::generate_home_pheromones_image_vector()
{
    PheromoneGrid pheromones = worldPtr->get_pheromones();
    std::vector<int> homeAlphas = pheromones.get_home_pheromones();

    for (int i=0; i < homeAlphas.size(); i++)
    {
        int alphaValue = homeAlphas[i]/pheromoneAlphaScale;
        if (alphaValue>255)
        {
            alphaValue = 255;
        }
        set_rgba_value(homeImageInts.data()+i,255,0,0,alphaValue);
    }
}

void RenderArea::generate_food_pheromones_image_vector()
{
    PheromoneGrid pheromones = worldPtr->get_pheromones();
    std::vector<int> foodAlphas = pheromones.get_food_pheromones();

    for (int i=0; i < foodAlphas.size(); i++)
    {
        int alphaValue = foodAlphas[i]/pheromoneAlphaScale;
        if (alphaValue>255)
        {
            alphaValue = 255;
        }
        set_rgba_value(foodImageInts.data()+i,0,0,255,alphaValue);
    }
}

void RenderArea::set_rgba_value(int* pixel, unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha)
{
    unsigned char* array = reinterpret_cast<unsigned char*>(pixel);
    array[3]=alpha;
    array[2]=blue;
    array[1]=green;
    array[0]=red;
}

void RenderArea::set_currentAddObject(const int& object)
{
    if (object == 1)
    {
        currentAddObject = food;
    }
    if (object == 2)
    {
        currentAddObject = obstacle;
    }
    if (object == 3)
    {
        currentAddObject = ant;
    }
    if (object == 4)
    {
        currentAddObject = erase;
    }
    if (object == 5)
    {
        currentAddObject = colony;
    }
}

void RenderArea::add_obstacle(const int &x, const int &y, const QPoint &clickPoint)
{
    scribbling = true;
    lastPoint = clickPoint;
    worldPtr->add_obstacle(x,y,brushRadius);
    generate_obstacle_image_vector();
}

void RenderArea::start_erasing(const int &x, const int &y, const QPoint &clickPoint)
{
    scribbling = true;
    lastPoint = clickPoint;
    worldPtr->erase(x,y,brushRadius);
    generate_obstacle_image_vector();
}

void RenderArea::add_ant(const int &x, const int &y)
{
    double orientation = generate_random_double(0,2*3.14);
    Vector3D location(x,y,0);
    worldPtr->add_ant(location, orientation);
}

