#include <QApplication>
#include <ctime>
#include "myglwidget.h"
#include "webcam.h"
#include "brickbusterwindow.h"

int main(int argc, char *argv[])
{
    // Creation de l'application QT
    QApplication app(argc, argv);
    BrickBusterWindow w;
    //w.show();

    // Execution de l'application QT
    return app.exec();
}
