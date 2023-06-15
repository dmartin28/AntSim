#include "mainwindow.hpp"
#include "ui_mainwindowform.h"

MainWindow::MainWindow(QWidget *parent) :QMainWindow{parent}, mMainWindowUI{new Ui::MainWindowForm}
{
    mMainWindowUI->setupUi(this);
    resize(1100, worldHeight+50);

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    timer->start(10);

    renderArea = new RenderArea(this, &world);
    QGridLayout *gridLayout = new QGridLayout(this->mMainWindowUI->graphicsFrame);
    gridLayout->addWidget(renderArea,0,0);
}

MainWindow::~MainWindow()
{
    delete mMainWindowUI;
}

void MainWindow::paintEvent(QPaintEvent *)
{
    world.update();
    renderArea->update();
}

void MainWindow::on_generateMapPushButton_clicked()
{
    world.clear_all();
    world.generate_random_caves();
    renderArea->generate_obstacle_image_vector();
}

void MainWindow::on_addAntPushButton_clicked()
{
    currentAddObject = 3;
    renderArea->set_currentAddObject(currentAddObject);
}

void MainWindow::on_addObstaclePushButon_clicked()
{
    currentAddObject = 2;
    renderArea->set_currentAddObject(currentAddObject);
}

void MainWindow::on_addFoodPushButton_clicked()
{
    currentAddObject = 1;
    renderArea->set_currentAddObject(currentAddObject);
}

void MainWindow::on_erasePushButton_clicked()
{
    currentAddObject = 4;
    renderArea->set_currentAddObject(currentAddObject);
}

void MainWindow::on_addColonyPushButton_clicked()
{
    currentAddObject = 5;
    renderArea->set_currentAddObject(currentAddObject);
}

void MainWindow::on_clearMapPushButton_clicked()
{
    world.clear_all();
    renderArea->generate_obstacle_image_vector();
}

