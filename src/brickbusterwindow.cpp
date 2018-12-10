#include "brickbusterwindow.h"
#include "ui_brickbusterwindow.h"

BrickBusterWindow::BrickBusterWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::BrickBusterWindow)
{
    //ui->setupUi(this);
    glWidget_ = new MyGLWidget();
    glWidget_->show();

    webcam_ = new Webcam();
    webcam_->initTemplateImages(glWidget_);
    webcam_->frameProcessing();
}

BrickBusterWindow::~BrickBusterWindow()
{
    delete ui;
}

void BrickBusterWindow::afficheDir(int direction)
{
    std::cout << "la direction : " << direction << endl;
}
