#ifndef WEBCAM_H
#define WEBCAM_H

#include "opencv2/video/tracking.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

#include "myglwidget.h"

#include <cstdio>
#include <iostream>
#include <QWidget>
#include <QObject>

using namespace cv;
using namespace std;

class Webcam : public QWidget
{
    Q_OBJECT

public:
    Webcam(QWidget * parent = nullptr);
    virtual ~Webcam();
    void initTemplateImages(MyGLWidget* glw);
    void frameProcessing();
    double intersection(Rect rect1,Rect rect2);
    Point findResultVectPoint(Mat &frame2, Rect workingRect, Mat &frameRect1, Mat &frameRect2, Rect templateRect, Mat resultImage);
    Rect findResultrect(Mat frame, Mat templateImageTracking, Mat resultImageTracking, int subImageWidth, int subImageHeight);

private:
    int frameWidth_;
    int frameHeight_;
    int subImageWidth_;
    int subImageHeight_;
    int templateWidth_;
    int templateHeight_;

    // Définition des rectangles et de leur placement
    Rect *workingRect_;
    Rect *templateRect_;
    Rect resultRectTracking_;
    Point *workingCenter_;
    Point vect_;

    // Définition des compteurs de frame pour suivre les mouvements de balayage
    vector<bool> vectorStop_ = {false, false, false, false, false};
    int numberFrame_ = 0;
    int numberFrameLastMove_ = 0;
    int numberFrameLastStop_ = 0;

    Mat frame1_;
    Mat frame2_;
    Mat frameRect1_;
    Mat frameRect2_;
    Mat resultImage_;
    Mat templateImageTracking_;
    Mat resultImageTracking_;

    VideoCapture *cap_;
    MyGLWidget* glw_;
};

#endif // WEBCAM_H
