#ifndef PLANT_H
#define PLANT_H

#include <QObject>
#include <QLabel>
#include <QMovie>
#include <QMediaPlayer>
#include <QTimer>
#include <iostream>

class Plant : public QLabel{
    Q_OBJECT
public:
    Plant(QWidget* parent = 0);
    //所有植物的共同动画
    QMovie *plantSwingMovie;
    QWidget *pointer;
    int Returnlife(){return life;}
    int ReturnRow(){return row;}
    int ReturnCol(){return col;}

private:
    //所有植物的共有性质
    int life;
    int row;
    int col;//位置信息
    bool isBeingEaten{false};//记录是否正在被吃,初始是没有被吃的状态
    QMediaPlayer *mediaPlayer_plant;//种植植物的声音
public slots:
    void SetRow(int Row){row=Row;}
    void SetLife(int Life){life=Life;}
    void SetCol(int Col){col=Col;}
};

#endif // PLANT_H
