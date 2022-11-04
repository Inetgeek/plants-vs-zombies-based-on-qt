#include "jalapeno.h"

Jalapeno::Jalapeno(QWidget* parent):Plant(parent){
    Attackingmovie = new QMovie(":/images/Jalapeno.gif");
    fireMovie = new QMovie(":/images/JalapenoExplode.gif");
    this->setMovie(Attackingmovie);
    Attackingmovie->start();
    this->show();//显示启动动画
    startAttacking();
}

void Jalapeno::startAttacking(){
    QTimer* a = new QTimer(this);
    a->setInterval(1000);
    a->start();
    connect(a, &QTimer::timeout, [=](){
        this->setMovie(fireMovie);
        this->resize(750, 90);
        this->move(120, this->y());
        fireMovie->start();
        this->show();
    });
}
