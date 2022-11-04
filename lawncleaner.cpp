#include "lawncleaner.h"

LawnCleaner::LawnCleaner(QWidget* parent):QLabel(parent){
    //图像设置
    image_lawnCleaner = new QImage(":/images/LawnCleaner.png");
    this->setPixmap(QPixmap::fromImage(*image_lawnCleaner));
    this->setGeometry(0, 0, 70, 57);
    this->setScaledContents(true);

    //计时器设置
    timer_lawnCleanerCheck = new QTimer(this);
    timer_lawnCleanerCheck->setInterval(20);
    timer_lawnCleanerCheck->start();
    QObject::connect(timer_lawnCleanerCheck, SIGNAL(timeout()), this, SLOT(slot_movingForward()));

    //音效设置
    mediaPLayer_lawnCleaneRunning = new QMediaPlayer(this);
    mediaPLayer_lawnCleaneRunning->setMedia(QUrl("qrc:/music/zamboni1.mp3"));
    mediaPLayer_lawnCleaneRunning->setVolume(150);
}

void LawnCleaner::slot_enter(){
    animation_enter = new QPropertyAnimation(this,"geometry");
    animation_enter->setDuration(500);
    animation_enter->setStartValue(QRect(-70, 80+row*100 + 20, 70, 57));
    animation_enter->setEndValue(QRect(60, 80 + row * 100 + 20, 70, 57));
    this->show();
    animation_enter->start();
}

//前进动画
void LawnCleaner::slot_movingForward(){
    if(isActivited == true && isRunning == false){
        isRunning = true;
        mediaPLayer_lawnCleaneRunning->play();
        animation_movingForward = new QPropertyAnimation(this, "geometry");
        animation_movingForward->setDuration(5500);
        animation_movingForward->setStartValue(QRect(70, 80 + row * 100 + 20, 70, 57));
        animation_movingForward->setEndValue(QRect(900, 80 + row * 100 + 20, 70, 57));
        animation_movingForward->start();
    }
}
