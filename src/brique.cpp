#include "brique.h"

Brique::Brique(int posX, int posY)
{
    disp_ = true;
    taille_ = 2;
    hauteur_ = 0.666;
    posX_ = -6 + (posX + 0.5)*taille_;
    posY_ = 10 - (posY + 0.5)*hauteur_;
    couleur_ = rand()%255;

    // Chargement des textures
    QImage tBriques = QGLWidget::convertToGLFormat(QImage(":/tex/res/brick3.jpg"));

    textureID_ = new GLuint[1];
    glGenTextures(1, textureID_);

    // On charge la place des textures dans le vecteur m_TextureID
    // Pour pouvoir les identifier dans Planet par la suite
    glBindTexture(GL_TEXTURE_2D, textureID_[0]);
    glTexImage2D(GL_TEXTURE_2D, 0, 4, tBriques.width(),tBriques.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, tBriques.bits());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

Brique::~Brique()
{

}

void Brique::Display()
{
    if(disp_){
        glPushMatrix();

        glTranslatef(posX_, posY_, 0);
        float color[] = {1,1,1,1};
        glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE, color); // On définit la couleur courante
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, textureID_[0]);

        glBegin(GL_QUADS);
        // Devant
        //glColor3ub(couleur_*2%255,couleur_,couleur_*3%255);
        glTexCoord2i(0, 0); glVertex3f(-taille_/2, -hauteur_/2, 0);
        glTexCoord2i(1, 0); glVertex3f(taille_/2, -hauteur_/2, 0);
        glTexCoord2i(1, 1); glVertex3f(taille_/2, hauteur_/2, 0);
        glTexCoord2i(0, 1); glVertex3f(-taille_/2, hauteur_/2, 0);

        // Fond
        glVertex3f(-taille_/2, -hauteur_/2, -1);
        glVertex3f(taille_/2, -hauteur_/2, -1);
        glVertex3f(taille_/2, hauteur_/2, -1);
        glVertex3f(-taille_/2, hauteur_/2, -1);

        // Bas
        glVertex3f(-taille_/2, -hauteur_/2, 0);
        glVertex3f(-taille_/2, -hauteur_/2, -1);
        glVertex3f(taille_/2, -hauteur_/2, -1);
        glVertex3f(taille_/2, -hauteur_/2, 0);

        // Haut
        glVertex3f(-taille_/2, hauteur_/2, 0);
        glVertex3f(-taille_/2, hauteur_/2, -1);
        glVertex3f(taille_/2, hauteur_/2, -1);
        glVertex3f(taille_/2, hauteur_/2, 0);

        // Gauche
        glVertex3f(-taille_/2, hauteur_/2, -1);
        glVertex3f(-taille_/2, -hauteur_/2, -1);
        glVertex3f(-taille_/2, -hauteur_/2, 0);
        glVertex3f(-taille_/2, hauteur_/2, 0);

        // Droite
        glVertex3f(taille_/2, hauteur_/2, -1);
        glVertex3f(taille_/2, -hauteur_/2, -1);
        glVertex3f(taille_/2, -hauteur_/2, 0);
        glVertex3f(taille_/2, hauteur_/2, 0);
        glDisable(GL_TEXTURE_2D);
        glEnd();

        glPopMatrix();
    }
}

float* Brique::getPos()
{
    float* pos = new float[2];
    pos[0] = posX_;
    pos[1] = posY_;
    return pos;
}

void Brique::briqueTouched()
{
    disp_ = false;
    this->~Brique();
}

bool Brique::isTouched()
{
    return disp_;
}
