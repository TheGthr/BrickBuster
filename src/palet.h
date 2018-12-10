#ifndef PALET_H
#define PALET_H

#include <QGLWidget>

class Palet
{
public:
    // Constructeur
    Palet(GLfloat taille = 3);

    // Destructeur
    virtual ~Palet();

    // Methode d'affichage
    void Display();

    // Getter & Setter
    float getPos();
    float getTaille();
    int getCalls();
    void setPos(float pos);
    void setCalls(int calls);

    // Déplacement du palet
    void left();
    void right();

private:
    int calls_;
    GLuint* textureID_;
    float pos_; // correspond au milieu du palet
    float taille_;
    float hauteur_;
};

#endif // PALET_H
