#ifndef BRIQUE_H
#define BRIQUE_H

#include <QGLWidget>

class Brique
{
public:
    // Constructeur
    Brique(int posX, int posY);

    // Destructeur
    virtual ~Brique();

    // Methode d'affichage
    void Display();
    float *getPos();

    // Permet de déclarer quelle brique est touchée
    void briqueTouched();

    // Permet de vérifier si la brique est touchée
    bool isTouched();

private:
    bool disp_;
    int couleur_;
    float posX_; // correspond au milieu de la brique
    float posY_; // correspond au milieu de la brique
    float taille_;
    float hauteur_;

    GLuint* textureID_;
};

#endif // BRIQUE_H
