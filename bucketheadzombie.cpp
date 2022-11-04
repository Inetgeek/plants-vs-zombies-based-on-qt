#include "bucketheadzombie.h"

bucketheadZombie::bucketheadZombie(QWidget* parent):Zombie(parent){
    changeZombieLife(-100);
    //僵尸吃东西声音设置
    zoobieEatingSound = new QMediaPlayer(this);
    zoobieEatingSound->setMedia(QUrl("qrc:/music/chompsoft.mp3"));
    zoobieEatingSound->setVolume(150);
    //设立动作
    SetMovie();
    QObject::connect(zombieTimer, &QTimer::timeout, this, &Zombie::slot_changeZombiePosition);
    QObject::connect(zombieTimer, &QTimer::timeout, this, &Zombie::slot_changeZombieAction);
}

void bucketheadZombie::SetMovie()
{
    //僵尸只有两个基本动作
    zombieMoveMovie = new QMovie(":/images/BucketheadZombie.gif");
    zombieEatingMovie = new QMovie(":/images/BucketheadZombieAttack.gif");

    this->setMovie(zombieMoveMovie);//初始化为移动的动作
    zombieMoveMovie->start();

    this->move(getZombiePosX(), getZombiePosY());//初始化移动到初始化点
}
