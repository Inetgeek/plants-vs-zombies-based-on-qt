#include "normalzombie.h"

normalZombie::normalZombie(QWidget* parent): Zombie(parent){
    //设立动作
    SetMovie();
    QObject::connect(zombieTimer, &QTimer::timeout, this, &Zombie::slot_changeZombiePosition);
    QObject::connect(zombieTimer, &QTimer::timeout, this, &Zombie::slot_changeZombieAction);
}

void normalZombie::SetMovie(){
    //僵尸只有两个基本动作
    zombieMoveMovie = new QMovie(":/images/Zombie.gif");
    zombieEatingMovie = new QMovie(":/images/ZombieAttack.gif");

    this->setMovie(zombieMoveMovie);//初始化为移动的动作
    zombieMoveMovie->start();

    this->move(getZombiePosX(), getZombiePosY());//初始化移动到初始化点
}
