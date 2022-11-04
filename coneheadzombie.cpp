#include "coneheadzombie.h"

coneheadZombie::coneheadZombie(QWidget* parent):Zombie(parent){
    changeZombieLife(-50);
    //僵尸吃东西声音设置
    zoobieEatingSound = new QMediaPlayer(this);
    zoobieEatingSound->setMedia(QUrl("qrc:/music/chompsoft.mp3"));
    zoobieEatingSound->setVolume(150);

    SetMovie();
    QObject::connect(zombieTimer, &QTimer::timeout, this, &Zombie::slot_changeZombiePosition);
    QObject::connect(zombieTimer, &QTimer::timeout, this, &Zombie::slot_changeZombieAction);
}

void coneheadZombie::SetMovie(){
    //僵尸只有两个基本动作
    zombieMoveMovie = new QMovie(":/images/ConeheadZombie.gif");
    zombieEatingMovie = new QMovie(":/images/ConeheadZombieAttack.gif");

    this->setMovie(zombieMoveMovie);//初始化为移动的动作
    zombieMoveMovie->start();

    this->move(getZombiePosX(), getZombiePosY());//初始化移动到初始化点
}
