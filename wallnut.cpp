#include "wallnut.h"

WallNut::WallNut(QWidget* parent):Plant(parent)
{
    //植物共有性质的赋值
    SetLife(PLANT_LIFE_BLOOD * 5);

    //动画
    plantSwingMovie = new QMovie(":/images/WallNut.gif");
    this->setMovie(plantSwingMovie);
    plantSwingMovie->start();
    //显示坚果动画
    this->show();
}
