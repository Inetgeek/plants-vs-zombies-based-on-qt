#ifndef ICEPEA_H
#define ICEPEA_H
#include "plant.h"
#include <QList>
#include <QTimer>
#include "bullet.h"
#include "config.h"
/*
    逻辑为由于植物的不同设置相应的图片，并且在检测到该行出现僵尸的时候开启startAttack的槽函数并且对僵尸进行射击
*/
class IcePea:public Plant{
public:
    IcePea(QWidget* parent = 0);
    bool ReturnAttackingstate(){return isAttacking;}
    QList<Bullet*> bulletList;//子弹的一个List
    int ReturnType(){return type;}
    QTimer* shootingTriggerTimer;//控制发射豌豆间隔
    bool isAttacking = false;
private:
    int attackNumber = 20;
    int type = 1;
public slots:
    void startAttack();
    void slot_shootingBullet();
};

#endif // ICEPEA_H
