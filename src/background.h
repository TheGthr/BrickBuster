#ifndef BACKGROUND_H
#define BACKGROUND_H

#include <QGLWidget>

class Background
{
public:
    Background();
    virtual ~Background();
    void Display();

private:
    GLuint* textureID_;
};

#endif // BACKGROUND_H
