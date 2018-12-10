#include "balle.h"

Balle::Balle()
{
    // Position initiale : milieu de la zone de jeu, on appuie sur espace et la balle tombe
    quad_ = gluNewQuadric();
    x_ = 0.1; // On ne centre pas pour ne pas taper 2 briques directement
    y_ = 5;
    vx_ = 0;
    vy_ = 0;
    angle_ = 0;

    // Chargement des textures
    QImage tBriques = QGLWidget::convertToGLFormat(QImage(":/tex/res/ball2.jpg"));

    textureID_ = new GLuint[1];
    glGenTextures(1, textureID_);

    glBindTexture(GL_TEXTURE_2D, textureID_[0]);
    glTexImage2D(GL_TEXTURE_2D, 0, 4, tBriques.width(), tBriques.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, tBriques.bits() );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

Balle::~Balle()
{
    // Destruction de la quadrique
    gluDeleteQuadric(quad_);
}

void Balle::Display()
{
    glPushMatrix();

    x_ += vx_;
    y_ += vy_;
    angle_ += 5;
    glTranslatef(x_, y_, 0);
    glRotatef(angle_,1.0f,1.0f,1.0f);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textureID_[0]);
    gluQuadricTexture(quad_, GL_TRUE);

    glColor3ub(255,255,255);
    gluSphere(quad_, 0.2, 70, 70);

    glDisable(GL_TEXTURE_2D);
    glPopMatrix();
}

void Balle::setV(float vx, float vy)
{
    vx_ = vx;
    vy_ = vy;
}

void Balle::setPos(float x, float y)
{
    x_ = x;
    y_ = y;
}

float* Balle::getPos()
{
    float *pos = new float[2];
    pos[0] = x_;
    pos[1] = y_;
    return pos;
}

float* Balle::getV()
{
    float *v = new float[2];
    v[0] = vx_;
    v[1] = vy_;
    return v;
}
