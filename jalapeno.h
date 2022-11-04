#ifndef JALAPENO_H
#define JALAPENO_H
#include "config.h"
#include "plant.h"
#include <QTimer>
#include <QThread>

class Jalapeno:public Plant{
public:
    Jalapeno(QWidget* parent = 0);
    int ReturnWaitingTime(){return WaitingTime;}
private:
    int attackingNumber = BOMB_DAMAGE_VALUE;  //伤害
    QMovie* Attackingmovie;
    QMovie* fireMovie;
    int WaitingTime = 1000;   //放置植物之后多少秒产生作用
public slots:
    void startAttacking();
};

#endif // JALAPENO_H
