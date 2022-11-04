#ifndef NORMALPEA_H
#define NORMALPEA_H

#include <QTimer>
#include <QList>

#include "bullet.h"
#include "plant.h"



class NormalPea:public Plant{
public:
    NormalPea(QWidget* parent = 0);
    bool ReturnAttackingstate(){return isAttacking;}
    QList<Bullet*> bulletList;      //子弹的一个List
    QTimer* shootingTriggerTimer;   //控制发射豌豆间隔
    int ReturnType(){return type;}
    bool isAttacking = false;     //是否正在攻击
private:
    int attackNumber = 20;       //每颗豌豆攻击力20
    int type = 0;    //表示0号子弹
public slots:
    void StartAttack();
    void slot_shootingBullet();
};

#endif // NORMALPEA_H
