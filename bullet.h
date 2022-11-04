#ifndef BULLET_H
#define BULLET_H
#include <QLabel>
#include <QImage>
#include <QString>
#include <QMovie>
#include <QPropertyAnimation>

#include "config.h"


/*
    逻辑设置不同的植物的子弹图片，通过位置信息打印每一个子弹的图像，如果与僵尸相遇，运行slot_deleteBullet的槽函数,
    该槽函数用于设置爆炸的时候的图像，并且对僵尸减少相应的血量
*/

class Bullet:public QLabel{
public:
    Bullet(int type,QWidget* parent = 0);        //设置子弹类型
    bool ReturnBulletType(){return IfIce;}     //返回子弹类型，判断对僵尸是否减速
    QPropertyAnimation* bulletAnimation;       //设置子弹飞行动画
    void bulletExplode();                      //子弹爆炸的函数
    int Returndamage();
private:
    int row;    //行
    double position_x;
    double position_y;
    double speed;
    QImage* bulletpicture;                    //设置子弹的图片
    QMovie* movie_bulletExplosion;
    int damage = BULLET_DAMAGE_VALUE;
    bool IfIce;
public slots:
    void slot_deleteBullet();
};

#endif // BULLET_H
