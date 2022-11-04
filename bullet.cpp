#include "bullet.h"

Bullet::Bullet(int type,QWidget *parent):QLabel(parent){
    //设置子弹类型
    if(type == 0){
        bulletpicture = new QImage(":/images/bullet.png");
        //设置子弹爆炸图片
        movie_bulletExplosion = new QMovie(":/images/PeaBulletHit.gif");
        IfIce = false;
    }
    else{
        bulletpicture = new QImage(":/images/PB-10.gif");  //寒冰子弹的图片
        movie_bulletExplosion = new QMovie(":/images/IcePeaBulletHit.gif");
        IfIce =true;
    }
    this->setPixmap(QPixmap::fromImage(*bulletpicture));
    this->show();
}

void Bullet::bulletExplode(){
    this->setMovie(movie_bulletExplosion);
    movie_bulletExplosion->start();
    connect(movie_bulletExplosion, &QMovie::finished, this, &Bullet::slot_deleteBullet);
}

void Bullet::slot_deleteBullet(){
    delete this;
}

int Bullet::Returndamage(){
    return damage;
}
