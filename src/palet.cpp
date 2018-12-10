#include "palet.h"

Palet::Palet(float taille)
{
    calls_ = 0;

    pos_ = 0;
    taille_ = taille;
    hauteur_ = 0.5;

    // Chargement des textures
    QImage tPal = QGLWidget::convertToGLFormat(QImage(":/tex/res/pal.JPG"));

    textureID_ = new GLuint[1];
    glGenTextures(1, textureID_);

    // On charge la place des textures dans le vecteur m_TextureID
    glBindTexture(GL_TEXTURE_2D, textureID_[0]);
    glTexImage2D(GL_TEXTURE_2D, 0, 4, tPal.width(),tPal.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, tPal.bits());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

Palet::~Palet()
{

}


void Palet::Display()
{
    glPushMatrix();

    glTranslatef(pos_, 0, 0);
    float color[] = {1,1,1,1};
    glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE, color); // On définit la couleur courante
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textureID_[0]);

    glBegin(GL_QUADS);
    // Devant
    glTexCoord2i(0, 0); glVertex3f(-taille_/2, 0, 0);
    glTexCoord2i(1, 0); glVertex3f(taille_/2, 0, 0);
    glTexCoord2i(1, 1); glVertex3f(taille_/2, hauteur_, 0);
    glTexCoord2i(0, 1); glVertex3f(-taille_/2, hauteur_, 0);

    // Fond
    glColor3ub(255,0,255); glVertex3f(-taille_/2, 0, -1);
    glVertex3f(taille_/2, 0, -1);
    glVertex3f(taille_/2, hauteur_, -1);
    glVertex3f(-taille_/2, hauteur_, -1);

    // Bas
    glColor3ub(255,0,0); glVertex3f(-taille_/2, 0, 0);
    glVertex3f(-taille_/2, 0, -1);
    glVertex3f(taille_/2, 0, -1);
    glVertex3f(taille_/2, 0, 0);

    // Haut
    glColor3ub(0,255,255); glVertex3f(-taille_/2, hauteur_, 0);
    glVertex3f(-taille_/2, hauteur_, -1);
    glVertex3f(taille_/2, hauteur_, -1);
    glVertex3f(taille_/2, hauteur_, 0);

    // Gauche
    glColor3ub(0,255,0); glVertex3f(-taille_/2, hauteur_, -1);
    glVertex3f(-taille_/2, 0, -1);
    glVertex3f(-taille_/2, 0, 0);
    glVertex3f(-taille_/2, hauteur_, 0);

    // Droite
    glColor3ub(255,255,0); glVertex3f(taille_/2, hauteur_, -1);
    glVertex3f(taille_/2, 0, -1);
    glVertex3f(taille_/2, 0, 0);
    glVertex3f(taille_/2, hauteur_, 0);
    glEnd();

    glPopMatrix();
}

void Palet::left()
{
    calls_ += 1;
    if(calls_ < 30 && calls_ > 0) {
        if(this->getPos() != 0)
            this->setPos(getPos()-0.06);
        else
            this->setPos(-0.06);
    } else if(calls_ == 30) {
        calls_ = 0;
    }

}

void Palet::right()
{
    calls_ += 1;
    if(calls_ < 30 && calls_ > 0) {
        if(this->getPos() != 0)
            this->setPos(getPos()+0.06);
        else
            this->setPos(+0.06);
    } else if(calls_ == 30) {
        calls_ = 0;
    }

}

float Palet::getPos()
{
    return pos_;
}

float Palet::getTaille()
{
    return taille_;
}

int Palet::getCalls()
{
    return calls_;
}

void Palet::setPos(float pos)
{
    if(!(pos + taille_/2 >= 6 || pos - taille_/2 <= -6))
        pos_ = pos;
}

void Palet::setCalls(int calls)
{
    calls_ = calls;
}
