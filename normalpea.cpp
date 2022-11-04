#include "normalpea.h"

NormalPea::NormalPea(QWidget* parent):Plant(parent){
    pointer = parent;
    //所有植物类的共有信息
    SetLife(PLANT_LIFE_BLOOD);
    //动画设置
    plantSwingMovie = new QMovie(":/images/Peashooter.gif");
    this->setMovie(plantSwingMovie);
    plantSwingMovie->start();
    //显示豌豆动画
    this->show();
    //发射时间器设置
    shootingTriggerTimer = new QTimer(this);
    shootingTriggerTimer->setInterval(1400);
    shootingTriggerTimer->start();
    //每1.4秒发射一个子弹
    QObject::connect(shootingTriggerTimer, &QTimer::timeout, this, &NormalPea::slot_shootingBullet);
}

void NormalPea::slot_shootingBullet(){
    if(ReturnAttackingstate() == true){
        Bullet* tempBullet = new Bullet(ReturnType(), pointer);
        tempBullet->bulletAnimation = new QPropertyAnimation(tempBullet, "geometry");
        tempBullet->bulletAnimation->setDuration(3000);
        tempBullet->bulletAnimation->setStartValue(QRect(120 + 82*ReturnCol() + 80, 80 + ReturnRow()*100 + 10, 36, 34));
        tempBullet->bulletAnimation->setEndValue(QRect(120 + 82*ReturnCol() + 80 + 900, 80 + ReturnRow()*100 + 10, 36, 34));
        tempBullet->bulletAnimation->start();
        tempBullet->show();
        bulletList << tempBullet;
    }
}
