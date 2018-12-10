#define _USE_MATH_DEFINES

#include "myglwidget.h"
#include <GL/glu.h>
#include <QApplication>
#include <QDesktopWidget>
#include <iostream>
#include <iomanip>

// Declarations des constantes
const unsigned int WIN_WIDTH  = 1920;
const unsigned int WIN_HEIGHT = 1080;

// Constructeur
MyGLWidget::MyGLWidget(QWidget * parent) : QGLWidget(parent)
{
    // Reglage de la taille/position
    //setFixedSize(WIN_WIDTH, WIN_HEIGHT);
    move(QApplication::desktop()->screen()->rect().center() - rect().center());

    connect(&animationTimer_,  &QTimer::timeout, [&] {
        updateGL();
    });

    animationTimer_.setInterval(10);
    animationTimer_.start();

    score_ = 0;
    level_ = 0;
    nbBalles_ = 3;

    height_ = height();
    width_ = width();
    textRatio_ = (height_*width_)/(1920*1080);
}


// Fonction d'initialisation
void MyGLWidget::initializeGL()
{
    glLoadIdentity();
    // Reglage de la couleur de fond
    glClearColor(0.5,0.5,0.5,1.0);
    // Activation du zbuffer
    glEnable(GL_DEPTH_TEST);

    printSpaceBar_ = true;

    movingR_ = false;
    movingL_ = false;

    nbBriques_ = 0;
    fp_ = false; // signal de fin de partie
    win_ = false;

    vBalle_ = 0.05;

    palet_ = new Palet();
    balle_ = new Balle();
    bg_ = new Background();

    for(int i = 0; i < 6; i++) { // colonnes
        for(int j = 0; j < 3; j++) { // lignes
            briques_.push_back(new Brique(i, j));
            nbBriques_ += 1;
        }
    }

    // Activation de la lumiere
    glEnable(GL_LIGHTING);
    // Reglage de la lampe
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
    //GLfloat colorAmbiant[] = {1.0,1.0,1.0,1.0};
    GLfloat ambientProperties[] = {0.8f, 0.8f, 0.08f, 1.0f};
    //GLfloat positionProperties[] = {0.0f, 0.0f, -2.0f, 0.0f};

    glLightfv(GL_LIGHT0, GL_AMBIENT, ambientProperties);
    //glLightfv( GL_LIGHT0, GL_POSITION, positionProperties);

    // On met une lampe ambiante ET diffuse pour avoir un bon éclairage
    //glLightfv(GL_LIGHT0, GL_AMBIENT_AND_DIFFUSE, colorAmbiant);
    //GLfloat pos[] = {0.0f,0.0f,-2.0f,0.0f};
    //glLightfv(GL_LIGHT0, GL_POSITION, pos);
}

void MyGLWidget::initLevel()
{
    printSpaceBar_ = true;

    movingR_ = false;
    movingL_ = false;

    nbBriques_ = 0;
    fp_ = false; // signal de fin de partie
    win_ = false;

    if(!briques_.empty()){
        for(uint i = 0; i < briques_.size(); i++)
            briques_.pop_back();
    }

    if(level_ > 0)
        vBalle_ += 0.02;
    else if(level_ == 0)
        vBalle_ = 0.05;

    int randNb = rand();

    for(int i = 0; i < 6; i++) { // colonnes
        for(int j = 0; j < 3; j++) { // lignes
            if(level_ == 0 || (level_ > 0 && randNb%2 == 0)) {
                briques_.push_back(new Brique(i, j));
                nbBriques_ += 1;
            }
            randNb = rand();
        }
    }

    // On fait cela pour qu'il n'y ait pas 0 brique
    if(nbBriques_ == 0) {
        briques_.push_back(new Brique(rand()%7, rand()%2));
        nbBriques_ = 1;
    }
}

// Fonction de redimensionnement
void MyGLWidget::resizeGL(int width, int height)
{
    // Definition du viewport (zone d'affichage)
    glViewport(0, 0, width, height);

    // Definition de la matrice de projection
    //glMatrixMode(GL_PROJECTION);
    //glLoadIdentity();
}


// Fonction d'affichage
void MyGLWidget::paintGL()
{
    height_ = height();
    width_ = width();
    textRatio_ = (height_*width_)/(1920*1080);

    glEnable(GL_DEPTH_TEST);
    // Reinitialisation des tampons
    glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);
    // Definition de la matrice modelview
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Perspective
    //gluPerspective(100, (GLfloat)width()/(GLfloat)height(), 0.1, 12);
    //gluLookAt(0, 4, 8, 0, 0, 0, 0, 1, 0);
    // Ortho
    glOrtho(-6, 6, -1, 10, -2, 2);


    // ----------------------------------------------------------------

    bg_->Display();
    collision();
    printSpaceBar();
    palet_->Display();
    balle_->Display();
    // Affichage des briques
    for(Brique* b : briques_) {
        b->Display();
    }

    if(movingL_) {
        if(palet_->getCalls() < 29)
            palet_->left();
        else
            movingL_ = false;
    } else if(movingR_) {
        if(palet_->getCalls() < 29)
            palet_->right();
        else
            movingR_ = false;
    }

    // ----------------------------------------------------------------

    glColor3ub(255, 255, 255);
    QFont *font = new QFont("Times", 15*textRatio_);

    // Affichage du score
    QString *score = new QString("Score : " + QString::number(score_));
    // Affichage du niveau
    QString *level = new QString("Niveau " + QString::number(level_ + 1));
    // Affichage du nb de balles
    QString *balles = new QString("balles : " + QString::number(nbBalles_));

    if(textRatio_ < 0.3) {
        font->setPointSizeF(15*textRatio_*4);
        renderText(30*textRatio_*4, height() - 50*textRatio_, *score, *font);
        renderText(width() - 150*textRatio_*4, height() - 50*textRatio_, *level, *font);
        renderText(width()/2 - 50*textRatio_*4, height() - 50*textRatio_, *balles, *font);
    } else if(textRatio_ < 0.8 && textRatio_ >= 0.3) {
        font->setPointSizeF(15*textRatio_*2);
        renderText(30*textRatio_*2, height() - 50*textRatio_, *score, *font);
        renderText(width() - 150*textRatio_*2, height() - 50*textRatio_, *level, *font);
        renderText(width()/2 - 50*textRatio_*2, height() - 50*textRatio_, *balles, *font);
    }  else {
        font->setPointSizeF(15*textRatio_);
        renderText(30*textRatio_, height() - 50*textRatio_, *score, *font);
        renderText(width() - 150*textRatio_, height() - 50*textRatio_, *level, *font);
        renderText(width()/2 - 50*textRatio_, height() - 50*textRatio_, *balles, *font);
    }
}

void MyGLWidget::collision()
{
    float centreBalleX = balle_->getPos()[0];
    float centreBalleY = balle_->getPos()[1];
    float bordBalleBasY = balle_->getPos()[1] - 0.2; // 0.2 = rayon de la balle
    float bordBalleHautY = balle_->getPos()[1] + 0.2; // 0.2 = rayon de la balle
    float bordBalleGaucheX = balle_->getPos()[0] - 0.2; // 0.2 = rayon de la balle
    float bordBalleDroitX = balle_->getPos()[0] + 0.2; // 0.2 = rayon de la balle
    float centrePalet = palet_->getPos();
    float gauchePalet = palet_->getPos() - palet_->getTaille()/2;
    float droitePalet = palet_->getPos() + palet_->getTaille()/2;
    float hautPalet = 0.5; // 0.5 = hauteur du palet

    // Collisions avec palet et murs
    if((bordBalleDroitX >= gauchePalet && bordBalleGaucheX <= droitePalet) && (bordBalleBasY <= hautPalet && bordBalleBasY > 0)) {
        balle_->setV((centreBalleX-centrePalet)/3*vBalle_, (1-abs(centreBalleX-centrePalet)/3)*vBalle_);
    } else if (bordBalleDroitX >= 6) { // mur droit
        balle_->setV(-balle_->getV()[0], balle_->getV()[1]);
    } else if (bordBalleGaucheX <= -6) { // mur gauche
        balle_->setV(-balle_->getV()[0], balle_->getV()[1]);
    } else if (bordBalleHautY >= 10) { // mur haut
        balle_->setV(balle_->getV()[0], -balle_->getV()[1]);
    } else if (bordBalleBasY <= -2) { // mur bas
        if(nbBalles_ > 1) {
            nbBalles_ -= 1;
            nouvelleBalle();
        } else {
            nbBalles_ -= 1;
            win_ = false;
            finPartie();
        }
    }

    // Collisions avec briques
    float MoitTailleBrique = 2/2;
    float MoitHautBrique = 0.666/2;
    if(!fp_){
        for(Brique *b : briques_) {
            if(b->isTouched()){
                float hautBrique = b->getPos()[1] + MoitHautBrique;
                float basBrique = b->getPos()[1] - MoitHautBrique;
                float droiteBrique = b->getPos()[0] + MoitTailleBrique;
                float gaucheBrique = b->getPos()[0] - MoitTailleBrique;
                if((bordBalleDroitX >= gaucheBrique) && (bordBalleGaucheX <= droiteBrique) && (bordBalleBasY <= hautBrique) && (bordBalleHautY >= basBrique)) {
                    if(centreBalleX <= (gaucheBrique)) { // si on tape par la gauche
                        balle_->setV(-balle_->getV()[0], balle_->getV()[1]);
                    } else if(centreBalleX >= (droiteBrique)) { // si on tape par la droite
                        balle_->setV(-balle_->getV()[0], balle_->getV()[1]);
                    } else if(centreBalleY <= (basBrique)) { // si on tape par le bas
                        balle_->setV(balle_->getV()[0], -balle_->getV()[1]);
                    } else if(centreBalleY >= (hautBrique)) { // si on tape par le haut
                        balle_->setV(balle_->getV()[0], -balle_->getV()[1]);
                    }
                    b->briqueTouched();
                    score_ += 50;
                    if(!b->isTouched()){
                        if(nbBriques_ == 1) {
                            nbBriques_ -= 1;
                            win_ = true;
                            finPartie();
                        } else {
                            nbBriques_ -= 1;
                        }
                    }
                }
            }
        }
    }
}

void MyGLWidget::nouvelleBalle()
{
    balle_->~Balle();
    balle_ = new Balle();
    palet_->setPos(0);
}

void MyGLWidget::finPartie()
{
    balle_->setV(0, 0);
    fp_ = true;
    glColor3ub(255, 102, 0);
    QFont *font = new QFont("Times", 100*textRatio_, QFont::Bold);
    if(win_) {
        QString *won = new QString("Bravo !");
        if(textRatio_ < 0.2) {
            font->setPointSizeF(100*textRatio_*4);
            renderText(width()/2-300*textRatio_*4, height()/2*textRatio_*6, *won, *font);
        } else if(textRatio_ < 0.8 && textRatio_ >= 0.2) {
            font->setPointSizeF(100*textRatio_*2);
            renderText(width()/2-300*textRatio_*2, height()/2*textRatio_*2, *won, *font);
        } else {
            font->setPointSizeF(100*textRatio_);
            renderText(width()/2-300*textRatio_, height()/2*textRatio_, *won, *font);
        }

        QFont *font = new QFont("Times", 30*textRatio_, QFont::Bold);
        QString *enter = new QString("Appuyer sur Entree pour continuer");
        if(textRatio_ < 0.3) {
            font->setPointSizeF(30*textRatio_*2);
            renderText(width()/2-450*textRatio_*2, height()/2+300*textRatio_, *enter, *font);
        } else if(textRatio_ < 0.8 && textRatio_ >= 0.3) {
            font->setPointSizeF(30*textRatio_*1.5);
            renderText(width()/2-450*textRatio_*1.5, height()/2+300*textRatio_, *enter, *font);
        }  else {
            font->setPointSizeF(30*textRatio_);
            renderText(width()/2-450*textRatio_, height()/2+200*textRatio_, *enter, *font);
        }

    } else {
        QString *lost = new QString("Perdu !");
        std::cout << textRatio_;
        if(textRatio_ < 0.2) {
            font->setPointSizeF(100*textRatio_*4);
            renderText(width()/2-300*textRatio_*4, height()/2*textRatio_*6, *lost, *font);
        } else if(textRatio_ < 0.8 && textRatio_ >= 0.2) {
            font->setPointSizeF(100*textRatio_*2);
            renderText(width()/2-300*textRatio_*2, height()/2*textRatio_*2, *lost, *font);
        }  else {
            font->setPointSizeF(100*textRatio_);
            renderText(width()/2-300*textRatio_, height()/2*textRatio_, *lost, *font);
        }

        QFont *font = new QFont("Times", 30*textRatio_, QFont::Bold);
        QString *enter2 = new QString("Appuyer sur Entree pour recommencer");
        if(textRatio_ < 0.3) {
            font->setPointSizeF(30*textRatio_*2);
            renderText(width()/2-500*textRatio_*2, height()/2+300*textRatio_, *enter2, *font);
        } else if(textRatio_ < 0.8 && textRatio_ >= 0.3) {
            font->setPointSizeF(30*textRatio_*1.5);
            renderText(width()/2-500*textRatio_*1.5, height()/2+300*textRatio_, *enter2, *font);
        }  else {
            font->setPointSizeF(30*textRatio_);
            renderText(width()/2-500*textRatio_, height()/2+200*textRatio_, *enter2, *font);
        }
    }
    animationTimer_.stop();
}

void MyGLWidget::nouvellePartie()
{
    score_ = 0;
    nbBalles_ = 3;
    level_ = 0;

    this->nouvelleBalle();
    this->initLevel();

    animationTimer_.setInterval(10);
    animationTimer_.start();
}

void MyGLWidget::niveauSuivant()
{
    level_ += 1;

    this->nouvelleBalle();
    this->initLevel();

    animationTimer_.setInterval(10);
    animationTimer_.start();
}

void MyGLWidget::printSpaceBar()
{
    if(printSpaceBar_){
        glColor3ub(255, 102, 0);
        QFont *font = new QFont("Times", 30*textRatio_, QFont::Bold);
        QString *space = new QString("Appuyer sur Barre d'Espace pour commencer");
        if(textRatio_ < 0.3) {
            font->setPointSizeF(30*textRatio_*2);
            renderText(width()/2-550*textRatio_*2, height()/2+300*textRatio_, *space, *font);
        } else if(textRatio_ < 0.8 && textRatio_ >= 0.3) {
            font->setPointSizeF(30*textRatio_*1.5);
            renderText(width()/2-550*textRatio_*1.5, height()/2+300*textRatio_, *space, *font);
        }  else {
            font->setPointSizeF(30*textRatio_);
            renderText(width()/2-550*textRatio_, height()/2+200*textRatio_, *space, *font);
        }
    }
}

void MyGLWidget::deplacePalet(int direction)
{
    switch (direction) {
    case 0:
    {
        palet_->setCalls(0);
        movingR_ = false;
        movingL_ = false;
        break;
    }
    case 1:
    {
        palet_->setCalls(0);
        movingR_ = true;
        break;
    }
    case 2:
    {
        palet_->setCalls(0);
        movingL_ = true;
        break;
    }
    default:
        break;
    }
}

// Fonction de gestion d'interactions clavier
void MyGLWidget::keyPressEvent(QKeyEvent * event)
{
    switch(event->key())
    {

    case Qt::Key_Right:
    {
        if(!fp_) {
            palet_->setCalls(0);
            movingR_ = true;
            // Acceptation de l'evenement et mise a jour de la scene
            event->accept();
            updateGL();
        }
        break;
    }

    case Qt::Key_Left:
    {
        if(!fp_) {
            palet_->setCalls(0);
            movingL_ = true;
            // Acceptation de l'evenement et mise a jour de la scene
            event->accept();
            updateGL();
        }
        break;
    }

    case Qt::Key_Down:
    {
        if(!fp_) {
            palet_->setCalls(0);
            movingL_ = false;
            movingR_ = false;
            // Acceptation de l'evenement et mise a jour de la scene
            event->accept();
            updateGL();
        }
        break;
    }

    case Qt::Key_Space:
    {
        if(!fp_ && balle_->getV()[0] == 0 && balle_->getV()[1] == 0) {
            balle_->setV(0, - vBalle_);
            printSpaceBar_ = false;
            // Acceptation de l'evenement et mise a jour de la scene
            event->accept();
            updateGL();
        }
        break;
    }

    case Qt::Key_Enter:
    case Qt::Key_Return:
    {
        if(fp_) {
            if(win_)
                niveauSuivant();
            else
                nouvellePartie();
        }
        break;
    }

    // Sortie de l'application
    case Qt::Key_Escape:
    {
        close();
        // Acceptation de l'evenement et mise a jour de la scene
        event->accept();
        updateGL();
        break;
    }

    // Cas par defaut
    default:
    {
        // Ignorer l'evenement
        event->ignore();
        return;
    }
    }
}
