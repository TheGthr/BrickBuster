#include "background.h"

Background::Background()
{
    // Chargement des textures
    QImage tBg = QGLWidget::convertToGLFormat(QImage(":/tex/res/bg2.jpg"));

    textureID_ = new GLuint[1];
    glGenTextures(1, textureID_);

    // On charge la place des textures dans le vecteur m_TextureID
    // Pour pouvoir les identifier dans Planet par la suite
    glBindTexture(GL_TEXTURE_2D, textureID_[0]);
    glTexImage2D(GL_TEXTURE_2D, 0, 4, tBg.width(),tBg.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, tBg.bits());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

Background::~Background()
{

}

void Background::Display()
{
    glPushMatrix();
    float color[] = {1,1,1,1};
    glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE, color); // On définit la couleur courante
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textureID_[0]);

    float y = -1.9f;
    glBegin(GL_QUADS);
    glTexCoord2i(0, 0); glVertex3f(-6, -1, y);
    glTexCoord2i(1, 0); glVertex3f(6, -1, y);
    glTexCoord2i(1, 1); glVertex3f(6, 10, y);
    glTexCoord2i(0, 1); glVertex3f(-6, 10, y);
    glDisable(GL_TEXTURE_2D);
    glEnd();
    glPopMatrix();
}
