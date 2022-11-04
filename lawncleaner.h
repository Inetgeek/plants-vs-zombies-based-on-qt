#ifndef LAWNCLEANER_H
#define LAWNCLEANER_H

#include "QTimer"
#include "QLabel"
#include "QPropertyAnimation"
#include "QImage"
#include "QMediaPlayer"
#include "QPixmap"

class LawnCleaner:public QLabel{
    Q_OBJECT

public:

    QImage* image_lawnCleaner;
    explicit LawnCleaner(QWidget* parent = 0);
    int row{0};               //所在行数
    int damage{1500};
    bool isActivited{false};  //是否被激活
    bool isRunning{false};    //是否正在前进
    QPropertyAnimation* animation_enter;
    QPropertyAnimation* animation_movingForward;//前进动画
    QTimer* timer_lawnCleanerCheck;
    QMediaPlayer* mediaPLayer_lawnCleaneRunning;

public slots:
    void slot_enter();         //登场动画
    void slot_movingForward(); //前进动画
};

#endif // LAWNCLEANER_H
