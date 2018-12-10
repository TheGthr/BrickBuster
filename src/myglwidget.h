#ifndef MYGLWIDGET_H
#define MYGLWIDGET_H

#include <QGLWidget>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QKeyEvent>
#include <QTimer>
#include <random>
#include "palet.h"
#include "balle.h"
#include "brique.h"
#include "background.h"


// Classe dediee a l'affichage d'une scene OpenGL
class MyGLWidget : public QGLWidget
{
    Q_OBJECT

public:

    // Constructeur
    MyGLWidget(QWidget * parent = nullptr);

protected:

    // Fonction d'initialisation
    void initializeGL();

    // Génération du niveau suivant si gagné, niveau 1 si perdu
    void initLevel();

    // Fonction de redimensionnement
    void resizeGL(int width, int height);

    // Fonction d'affichage
    void paintGL();
	
    // Fonction de gestion d'interactions clavier
    void keyPressEvent(QKeyEvent * event);

    // Gestion des collisions
    void collision();

    void nouvelleBalle();
    void finPartie();

    // Recommence une partie depuis le début
    void nouvellePartie();

    // Accède au niveau suivant
    void niveauSuivant();

    void printSpaceBar();

public slots:
    void deplacePalet(int direction);

private:
    std::vector<Brique *> briques_;
    Brique *brique_;
    Palet *palet_;
    Balle *balle_;
    Background *bg_;

    int nbBriques_;
    bool fp_;
    bool win_;

    int score_;
    int level_;
    int nbBalles_;

    float vBalle_;

    bool movingR_;
    bool movingL_;

    bool printSpaceBar_;
    double textRatio_;
    double height_;
    double width_;

    // Timer d'animation
    QTimer animationTimer_;
};

#endif // MYGLWIDGET_H
