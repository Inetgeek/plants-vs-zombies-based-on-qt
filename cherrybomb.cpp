#include "cherrybomb.h"

CherryBomb::CherryBomb(QWidget* parent):Plant(parent){
    bomb = new QMovie(":/images/CherryBomb.gif");
    bombfinish = new QMovie(":/images/Boom.png");
    this->setMovie(bomb);
    bomb->start();

    this->show();//显示启动动画
    startAttacking();
}

void CherryBomb::startAttacking()
{
    QTimer* a = new QTimer(this);
    a->setInterval(1000);
    a->start();
    connect(a, &QTimer::timeout, [=](){
        this->setMovie(bombfinish);
        bombfinish->start();
        this->show();
    });
}
