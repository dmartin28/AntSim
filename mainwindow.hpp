#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "world.hpp"
#include "renderarea.hpp"

#include <QMainWindow>
#include <QtCore>
#include <QPainter>
#include <QMouseEvent>

namespace Ui {class MainWindowForm;}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    virtual void paintEvent(QPaintEvent *);

public slots:

private slots:
    void on_generateMapPushButton_clicked();

    void on_addAntPushButton_clicked();

    void on_addObstaclePushButon_clicked();

    void on_addFoodPushButton_clicked();

    void on_erasePushButton_clicked();

    void on_addColonyPushButton_clicked();

    void on_clearMapPushButton_clicked();

private:
    Ui::MainWindowForm *mMainWindowUI;
    int currentAddObject{1};
    int worldWidth{800};
    int worldHeight{500};
    World world{worldWidth, worldHeight};
    RenderArea* renderArea;
};

#endif // MAINWINDOW_H
