#ifndef BRICKBUSTERWINDOW_H
#define BRICKBUSTERWINDOW_H

#include <QMainWindow>
#include <iostream>
#include <QListWidget>
#include "myglwidget.h"
#include "webcam.h"

namespace Ui {
class BrickBusterWindow;
}

class BrickBusterWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit BrickBusterWindow(QWidget *parent = 0);
    ~BrickBusterWindow();

public slots:
    void afficheDir(int direction);

private:
    Ui::BrickBusterWindow *ui;
    Webcam *webcam_;
    MyGLWidget *glWidget_;
};

#endif // BRICKBUSTERWINDOW_H
