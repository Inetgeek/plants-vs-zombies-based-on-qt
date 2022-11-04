#include "zombie.h"
#include <iostream>

//构造函数
Zombie::Zombie(QWidget* parent):QLabel(parent){
    //生命值等参数初始化
    life = ZOMBIE_LIFE_BLOOD;
    row = -1;
    posX = 900;
    posY = 0;
    movingSpeed = ZOMBIE_MOVING_SPEED;
    isEatingPlant = false;
    damageValue = ZOMBIE_DAMAGE_VALUE;

    //僵尸吃东西声音设置
    zoobieEatingSound = new QMediaPlayer(this);
    zoobieEatingSound->setMedia(QUrl("qrc:/music/chompsoft.mp3"));
    zoobieEatingSound->setVolume(150);

    //计时器的设计
    zombieTimer = new QTimer(this);
    zombieTimer->setInterval(20);
    zombieTimer->start();

    //死亡图片
    normalDead = new QMovie(":/images/ZombieDie.gif");
    bombDead = new QMovie(":/images/BoomDie.gif");
}

//析构函数
Zombie::~Zombie(){

}

//获取僵尸血量
int Zombie::getZombieLife(){
    return life;
}

//获取僵尸所在行
int Zombie::getZombieRow(){
    return row;
}

//获取僵尸当前横坐标
double Zombie::getZombiePosX(){
    return posX;
}

//获取僵尸当前纵坐标
double Zombie::getZombiePosY(){
    return posY;
}

//获取僵尸移动速度
double Zombie::getZombieMovingSpeed(){
    return movingSpeed;
}

//获取僵尸是否正在吃植物
bool Zombie::getIsEatingPlant(){
    return isEatingPlant;
}

//获取僵尸攻击力
int Zombie::getZombieDamageValue(){
    return damageValue;
}

//僵尸减血
void Zombie::changeZombieLife(int hurt_blood){
    life = life - hurt_blood;
}

//改变僵尸速度
void Zombie::changeZombieMovingSpeed(double new_speed){
    movingSpeed = new_speed;
}

//改变僵尸是否吃植物的状态
void Zombie::changeZombieStatus(bool status){
    isEatingPlant = status;
}

//改变僵尸位置
void Zombie::slot_changeZombiePosition(){
    //如果没有在吃植物，那就移动
    if(!isEatingPlant){
        posX = posX - movingSpeed;
    }
    this->move(posX, posY);
}

//改变僵尸动作
void Zombie::slot_changeZombieAction(){
    //在吃植物时的动作
    if(isEatingPlant&&life > 0 && !isburn){
        zoobieEatingSound->play();
        this->setMovie(zombieEatingMovie);
        zombieEatingMovie->start();
    }
    else if(life > 0 && !isburn){
        //初始化为移动的动作
        this->setMovie(zombieMoveMovie);
        zombieMoveMovie->start();
    }
    else if(life <= 0 && !isburn){
        this->setMovie(normalDead);
        this->move(posX-50, posY);
        this->resize(170, 150);
        normalDead->start();
    }
    else{
        this->setMovie(bombDead);
        this->move(posX, posY);
        this->resize(170, 150);
        bombDead->start();
    }
}

//改变僵尸所在行
void Zombie::changeZombieRow(int r){
    row = r;
}

//改变僵尸纵坐标
void Zombie::changeZombiePosY(double y){
    posY = y;
}

//普通死亡槽函数
void Zombie::slot_normalDead(){
    this->setMovie(normalDead);
    zombieMoveMovie->stop();
    normalDead->start();
    this->show();
}

//爆炸死亡槽函数
void Zombie::slot_bombDead(){
    this->setMovie(bombDead);
    bombDead->start();
}
