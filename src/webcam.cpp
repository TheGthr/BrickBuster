#include "webcam.h"

using namespace cv;
using namespace std;

Webcam::Webcam(QWidget * parent) : QWidget(parent)
{
    // Définition taille de la frame
    frameWidth_ = 640;
    frameHeight_ = 480;
    // Définition taille de la zone d'interaction
    subImageWidth_ = 128;
    subImageHeight_ = 128;
    // Définition taille de l'image template
    templateWidth_ = 75;
    templateHeight_ = 75;

    // Définition des rectangles et de leur placement
    workingRect_ = new Rect((frameWidth_-subImageWidth_)/2,frameHeight_/2+(frameHeight_/2-subImageHeight_)/2,subImageWidth_,subImageHeight_);
    templateRect_ = new Rect((workingRect_->width-templateWidth_)/2,(workingRect_->height-templateHeight_)/2,templateWidth_,templateHeight_);
    workingCenter_ = new Point(workingRect_->x+subImageWidth_/2,workingRect_->y+subImageHeight_/2);

    // Définition des compteurs de frame pour suivre les mouvements de balayage
    vectorStop_ = {false, false, false, false, false};
    numberFrame_ = 0;
    numberFrameLastMove_ = 0;
    numberFrameLastStop_ = 0;

    // Ouverture de la webcam par défaut
    cap_ = new VideoCapture(0);
    // cout<<"Width :"<<cap.get(CV_CAP_PROP_FRAME_WIDTH)<<endl;
    // cout<<"Height :"<<cap.get(CV_CAP_PROP_FRAME_HEIGHT)<<endl;
    cap_->set(CV_CAP_PROP_FRAME_WIDTH,frameWidth_);
    cap_->set(CV_CAP_PROP_FRAME_HEIGHT,frameHeight_);

    // Vérification de l'ouverture de la webcam
    if(!cap_->isOpened())  // check if we succeeded
    {
        cerr<<"Error openning the default camera"<<endl;
    }
}

Webcam::~Webcam()
{

}

void Webcam::initTemplateImages(MyGLWidget* glw) {
    glw_ = glw;
    // Informations acquisition de l'image template
    cout<<"Put the template image inside the square and hit a key !"<<endl;
    while (waitKey(10)<0)
    {
        // Acquisition nouvelle frame
        if (cap_->read(frame1_))
        {
            // Flip vertical de l'image
            flip(frame1_,frame1_,1);
            // Copie du template rect
            templateImageTracking_=Mat(frame1_,*workingRect_).clone();
            // Dessin du rectangle d'acquisition au centre
            rectangle(frame1_,*workingRect_,Scalar(0,0,255),2,8,0);
            // Affichage de la frame
            imshow("WebCam", frame1_);
        }
    }

    // Récupération première frame
    *cap_ >> frame1_;
    // Effet miroir
    cv::flip(frame1_,frame1_,1);
    // Extraction du rectangle d'interaction et conversion en niveaux de gris
    cv::cvtColor(Mat(frame1_,*workingRect_),frameRect1_,COLOR_BGR2GRAY);
    //Mat(frame1,rect).copyTo(frameRect1);

    // Création de l'image résultat de match Template pour le balayage
    int result_cols =  frame1_.cols-templateWidth_  + 1;
    int result_rows = frame1_.rows-templateHeight_ + 1;
    resultImage_.create( result_cols, result_rows, CV_32FC1 );

    // Création de l'image résultat de match Template pour le tracking
    int result_colsTracking =  frame1_.cols-templateImageTracking_.cols  + 1;
    int result_rowsTracking = frame1_.rows-templateImageTracking_.rows + 1;
    resultImageTracking_.create( result_colsTracking, result_rowsTracking, CV_32FC1 );
}

void Webcam::frameProcessing() {
    while (waitKey(100)<0) {
        numberFrame_++;

        // Récupération de la nouvelle frame
        *cap_ >> frame2_;
        // Flip de l'image
        cv::flip(frame2_,frame2_,1);

        vect_ = findResultVectPoint(frame2_, *workingRect_, frameRect1_, frameRect2_, *templateRect_, resultImage_);
        resultRectTracking_ = findResultrect(frame2_,templateImageTracking_,resultImageTracking_, subImageWidth_, subImageHeight_);

        // Dessin du rectangle de travail et du vecteur de translation
        rectangle(frame2_,*workingRect_,Scalar(0, 255, 0),2);
        // Dessin du rectangle de tracking
        rectangle(frame2_,resultRectTracking_,Scalar(0, 255, 255),2);

        // Dessin du vecteur de translation via le point localisé
        Point p(workingCenter_->x+vect_.x,workingCenter_->y+vect_.y);
        arrowedLine(frame2_,*workingCenter_,p,Scalar(255,255,255),2);

        if ((vect_.x > 15) & ((numberFrame_ - numberFrameLastMove_) > 10)) {
            numberFrameLastMove_ = numberFrame_;
            glw_->deplacePalet(1); // Droite
        }
        if ((vect_.x < -15) & ((numberFrame_ - numberFrameLastMove_) > 10)) {
            numberFrameLastMove_ = numberFrame_;
            glw_->deplacePalet(2); // Gauche
        }

        // On rajoute true ou false dans le tableau de contrôle du recouvrement
        vectorStop_.push_back(intersection(resultRectTracking_,*workingRect_) > 85);
        // On supprime le premier élément
        vectorStop_.erase(vectorStop_.begin());

        // On regarde si tous les éléments sont à true
        unsigned int acc = 0;
        for(unsigned int i = 0; i < vectorStop_.size(); i++) {
            if (vectorStop_[i]) {
                acc++;
            }
        }

        // Si oui, on remplace le compteur de frame et on envoie la consigne d'arrêt
        if ((acc == vectorStop_.size()) & (numberFrame_-numberFrameLastStop_ > 10)) {
            numberFrameLastStop_ = numberFrame_;
            glw_->deplacePalet(0); // Arret
        }

        // Affichage de la deuxième frame
        imshow("WebCam", frame2_);

        // Permutation des rectangles pour continuer le traitement
        swap(frameRect1_,frameRect2_);
    }
}

double Webcam::intersection(Rect rect1,Rect rect2) {
    double x_overlap = fmax(0, fmin(rect1.x+rect1.width, rect2.x+rect2.width) - fmax(rect1.x, rect2.x));
    double y_overlap = fmax(0, fmin(rect1.y+rect1.height, rect2.y+rect2.height) - fmax(rect1.y, rect2.y));
    double overlapAreaPercentage = 100*(x_overlap * y_overlap)/(rect1.width*rect1.height);
    // cout << setprecision(4) << overlapAreaPercentage << endl;
    return overlapAreaPercentage;
}

Point Webcam::findResultVectPoint(Mat &frame2, Rect workingRect, Mat &frameRect1, Mat &frameRect2, Rect templateRect, Mat resultImage) {
    // Extraction du rectangle d'interaction et conversion en niveaux de gris
    cv::cvtColor(Mat(frame2,workingRect),frameRect2,COLOR_BGR2GRAY);

    // Extraction de l'image template dans la première frame
    Mat templateImage(frameRect1,templateRect);
    // Matching entre le rectangle d'interaction de la frame 2 et de l'image template de la frame 1
    matchTemplate(frameRect2, templateImage, resultImage, TM_CCORR_NORMED );
    // Localisation du meilleur match avec minMaxLoc
    double minVal; double maxVal; Point minLoc; Point maxLoc;
    minMaxLoc(resultImage, &minVal, &maxVal, &minLoc, &maxLoc);
    // Création d'un nouveau point symbolisant le vecteur de déplacement (entre origine et matching rect)
    return Point(maxLoc.x-templateRect.x,maxLoc.y-templateRect.y);
}

Rect Webcam::findResultrect(Mat frame, Mat templateImageTracking, Mat resultImageTracking, int subImageWidth, int subImageHeight) {
    // Matching entre la frame et l'image template, et stockage dans l'image de résultat
    matchTemplate(frame, templateImageTracking, resultImageTracking, TM_CCORR_NORMED);

    // Localisation du maximum avec minMaxLoc
    double minVal; double maxVal; Point minLoc; Point maxLoc;
    minMaxLoc(resultImageTracking, &minVal, &maxVal, &minLoc, &maxLoc, Mat());
    // Sauvegarde de la position du rectangle matché
    return Rect(maxLoc.x,maxLoc.y,subImageWidth,subImageHeight);
}
