#ifndef CHERRYBOMB_H
#define CHERRYBOMB_H
#include "config.h"
#include "plant.h"
#include <QThread>

class CherryBomb:public Plant{
public:
    CherryBomb(QWidget* parent = 0);
    int ReturnWaitingTime(){return WaitingTime;}
    QMovie* bombfinish;
private:
    int attackingNumber = BOMB_DAMAGE_VALUE;
    int range = 377;           //设置爆炸的范围
    QMovie* bomb;            //放置爆炸时候的动态
    int WaitingTime = 1000;   //放置植物之后多少秒产生作用
public slots:
    void startAttacking();
};

#endif // CHERRYBOMB_H
