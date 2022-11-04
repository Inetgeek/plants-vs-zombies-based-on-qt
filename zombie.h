/**
 * 僵尸父类
 */

#ifndef ZOMBIE_H
#define ZOMBIE_H

#include <QLabel>
#include <QTimer>
#include <QMovie>
#include <QMediaPlayer>

#include "config.h"

class Zombie : public QLabel
{
private:
    int life;            //僵尸血量
    int row;             //僵尸所在行
    double posX;         //僵尸当前所在横坐标
    double posY;         //僵尸当前所在纵坐标
    double movingSpeed;  //僵尸移动速度
    bool isEatingPlant;  //记录僵尸是否正在吃植物
    int damageValue;     //僵尸攻击力





public:
    Zombie(QWidget* parent = 0);                     //构造函数
    ~Zombie();                                       //析构函数
    QMovie *normalDead;                              //普通死亡图片
    QMovie *bombDead;                                //爆炸死亡图片
    QTimer* zombieTimer;                             //每个zoobie都有自己的timer
    QMovie* zombieMoveMovie;                         //zoobie走路时的动作
    QMovie* zombieEatingMovie;                       //zoobie吃东西时的动作
    bool isburn=false;                               //记录是否已经出生
    QMediaPlayer* zoobieEatingSound;                 //僵尸吃植物的音效
    int getZombieLife();                             //获取僵尸血量
    int getZombieRow();                              //获取僵尸所在行
    double getZombiePosX();                          //获取僵尸当前横坐标
    double getZombiePosY();                          //获取僵尸当前纵坐标
    double getZombieMovingSpeed();                   //获取僵尸移动速度
    bool getIsEatingPlant();                         //获取僵尸是否正在吃植物
    int getZombieDamageValue();                      //获取僵尸攻击力
    virtual void SetMovie() = 0;                     //虚函数，用于设立僵尸的movie
    //发送普通死亡信号
    void sendnormal(){
        connect(normalDead, &QMovie::finished, [=](){
            emit normalkill();
        });
    }
    //发送爆炸死亡信号
    void sendbomb(){
        connect(normalDead, &QMovie::finished, [=](){
            emit bombkill();
        });
    }


public slots:
    void changeZombieLife(int hurt_blood);           //僵尸减血
    void changeZombieMovingSpeed(double new_speed);  //改变僵尸速度
    void changeZombieStatus(bool status);            //改变僵尸是否吃植物的状态
    void changeZombieRow(int r);                     //改变僵尸所在行
    void changeZombiePosY(double y);                 //改变僵尸纵坐标
    void slot_changeZombiePosition();                //改变僵尸位置
    void slot_changeZombieAction();                  //改变僵尸动作
    void slot_normalDead();                          //正常死亡槽函数
    void slot_bombDead();                            //爆炸死亡槽函数
signals:
    void normalkill();                               //正常死亡信号
    void bombkill();                                 //爆炸死亡信号

};

#endif // ZOMBIE_H
