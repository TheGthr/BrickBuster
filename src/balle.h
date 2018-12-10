#ifndef BALLE_H
#define BALLE_H

#include <QGLWidget>
#include <GL/glu.h>

class Balle
{
public:
    // Constructeur
    Balle();

    // Destructeur
    virtual ~Balle();

    // Methode d'affichage
    void Display();

    // Set la vitesse de la balle
    void setV(float vx, float vy);

    void setPos(float x, float y);

    // Retourne la position de la balle
    float* getPos();

    float* getV();

private:
    GLUquadric* quad_;
    float x_;
    float y_;
    float vx_;
    float vy_;
    float angle_;

    GLuint* textureID_;
};

#endif // BALLE_H
