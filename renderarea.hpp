#ifndef RENDERAREA_HPP
#define RENDERAREA_HPP

#include "world.hpp"

#include <QObject>
#include <QWidget>
#include <QIcon>
#include <qpainter.h>
#include <QDebug>
#include <QImage>
#include <QMouseEvent>

class RenderArea : public QWidget
{
    Q_OBJECT
public:
    explicit RenderArea(QWidget *parent = nullptr, World* world = nullptr);

    void paint_ants(QPainter* painter);
    void paint_food(QPainter* painter);
    void paint_colony(QPainter* painter);
    void paint_pheromones(QPainter* painter);
    void paint_obstacles(QPainter* painter);
    QRect get_food_rect(const int& x, const int& y, const int& quantity);

    void generate_obstacle_image_vector();
    void initialize_pheromone_images();
    void generate_home_pheromones_image_vector();
    void generate_food_pheromones_image_vector();
    void set_rgba_value(int* pixel, unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha);

    void set_currentAddObject(const int& object);
    void add_obstacle(const int& x, const int& y, const QPoint& clickPoint);
    void start_erasing(const int& x, const int& y, const QPoint& clickPoint);
    void add_ant(const int& x, const int& y);

signals:

protected:

    void paintEvent(QPaintEvent* event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

    QTimer *timer{nullptr};
    QIcon antIcon = QIcon(":myicons/ant image2.png");
    QIcon antWithFoodIcon = QIcon(":myicons/ant with food image.png");
    std::vector<int> homeImageInts;
    std::vector<int> foodImageInts;
    std::vector<int> obstacleImageInts;

    enum AddObject { food = 1, obstacle = 2, ant = 3, erase = 4, colony = 5};
    AddObject currentAddObject{food};

    bool scribbling = false;
    QPoint lastPoint;

    int antSize{10};
    int minFoodSize{3};
    int colonySize{30};
    int worldWidth;
    int worldHeight;
    int pheromoneAlphaScale{30};
    int brushRadius{15};

    World* worldPtr;

    QImage antImage = QImage(":myicons/ant image2.png");
};

#endif // RENDERAREA_HPP
