#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include "config.h"

#include "normalzombie.h"
#include "bucketheadzombie.h"
#include "coneheadzombie.h"

#include "icepea.h"
#include "cherrybomb.h"
#include "jalapeno.h"
#include "wallnut.h"
#include "normalpea.h"

#include "lawncleaner.h"
#include "shovel.h"

#include <QWidget>
#include <QEvent>
#include <QVector>
#include <QWaitCondition>
#include <QLineEdit>
#include <QPushButton>
#include <QImage>
#include <QTimer>
#include <QMediaPlayer>
#include <QMediaPlaylist>

#include <cmath>
#include <ctime>

namespace Ui {
    class GameWindow;
}
/**
 * 游戏界面，用于实现主要逻辑
 */
class GameWindow : public QWidget{
    Q_OBJECT

public:
    void sleep();
    explicit GameWindow(QWidget* parent = nullptr);
    ~GameWindow();
    void scrollBackgroundImage();                        //滚动背景图
    void mousePressEvent(QMouseEvent* event);            //鼠标点击事件

private:
    QMediaPlayer* mediaPlayer_gameWindowBackground;      //背景音乐
    QMediaPlaylist* mediaPlaylist_gameWindowBackground;
    QMediaPlayer* mediaPlayer_bulletHitZombie;           //子弹打到僵尸
    QMediaPlayer* mediaPlayer_bulletHitbucketZombie;     //子弹打到铁桶僵尸
    QMediaPlayer* mediaPlayer_gameWin;                   //游戏胜利的音效
    QMediaPlayer* mediaPlayer_gameLose;                  //游戏失败的音效
    QMediaPlayer* mediaPlayer_siren;                     //警报声
    QMediaPlayer* mediaPlayer_buttonClicked;             //点击按钮声
    QMediaPlayer* mediaPlayer_shovel;                    //铲子铲植物

    NormalPea* normalPeaArray[5][9];               //存储豌豆射手的Array
    WallNut* wallNutArray[5][9];                   //存储坚果墙的Array
    IcePea* icePeaArray[5][9];                     //存储寒冰射手的Array
    CherryBomb* cherryBombArray[5][9];             //存储樱桃炸弹的Array
    Jalapeno* jalapenoArray[5][9];                 //存储火爆辣椒的Array
    LawnCleaner* lawnCleanerArray[5];              //存储lawncleaner的array

    QList<normalZombie*> normalZombieList;        //存储普通僵尸的List
    QList<bucketheadZombie*> bucketZombieList;    //存储铁桶僵尸的List
    QList<coneheadZombie*> coneZombieList;        //存储路障跳僵尸的List

    int normalZombieRound = ROUND_NUMBER;         //普通僵尸轮数计数器
    int bucketZombieRound = ROUND_NUMBER;         //铁桶僵尸轮数计数器
    int coneheadZombieRound = ROUND_NUMBER;       //路障跳僵尸轮数计数器

    int normalZombieRoundUpperLimit = MAX_ZOMBIE_NUMBER;                //普通僵尸只数上限
    int bucketZombieRoundUpperLimit = MAX_ZOMBIE_NUMBER;          //铁桶僵尸只数上限
    int coneheadZombieRoundUpperLimit = MAX_ZOMBIE_NUMBER;        //路障僵尸只数上限

    int numOfBImgRightMove{0};  //背景图滚动距离
    int numOfBImgLeftMove{0};   //背景图滚动距离

    int normalZombieProduceInterval{15000};   //普通僵尸产生间隔
    int bucketZombieProduceInterval{15000};   //铁桶僵尸产生间隔
    int coneheadZombieProduceInterval{15000}; //路障跳僵尸产生间隔

    QLabel* myBackGroundLabel;  //背景label

    QLabel* shovelHolder;       //铲子后面的label

    QLabel* cardHolder;         //卡片栏label

    QLabel* gameWinLabel;     //游戏胜利时显示用的label
    QImage* gameWinImg;       //游戏胜利时的图像
    QLabel* gameLoseLabel;    //游戏失败时显示用的label
    QImage* gameLoseImg;      //游戏失败时的图像

    //植物卡片的按钮
    QPushButton* normalPeaPushButton;
    QPushButton* wallNutPushButton;
    QPushButton* icePeaPushButton;
    QPushButton* cherryBombPushButton;
    QPushButton* jalapenoPushButton;

    //铲子卡片
    QPushButton* shovelPushButton;

    QImage* backGroundImg;
    QImage* shovelHolderImg;  //铲子后面的label对应的图片

    QImage* cardHolderImg;    //卡片栏label的图片

    bool isFieldPlantable[5][9];   //每块地能否种植物的bool数组
    int hasFieldPlanted[5][9];     //0是没种植物，1是豌豆射手，2是寒冰射手，3是坚果墙，4是樱桃炸弹，5是火爆辣椒
    bool isSettingNormalPea{false};
    bool ableToSetNormalPea{false};      //防止鼠标随便点就种植物
    bool isSettingWallNut{false};
    bool ableToSetWallNut{false};  //防止鼠标随便点就种植物
    bool isSettingIcePea{false};
    bool ableToSetIcePea{false};  //防止鼠标随便点就种植物
    bool isSettingCherryBomb{false};
    bool ableToSetCherryBomb{false};  //防止鼠标随便点就种植物
    bool isSettingJalapeno{false};
    bool ableToSetJalapeno{false};  //防止鼠标随便点就种植物
    bool isToShovelPlants{false};
    bool ableToShovelPlants{false};//防止鼠标随便点就铲植物
    bool isPeaCooling{false};
    bool isWallNutCooling{false};
    bool isSunFlowerCooling{false};//记录植物是否处于冷却时间里

    double getdist(double x1,double y1,double x2,double y2);  //计算两个物体之间的距离

    QTimer* gameWindowTimer;  //游戏主窗口计时器

    //和僵尸有关的QTimer
    QTimer* normalZombieProduceTimer;
    QTimer* bucketZombieProduceTimer;
    QTimer* coneheadZombieProduceTimer;

    void func_gameLose();       //游戏失败的时候显示动画，结束计时器
    void func_gameWin();        //游戏胜利的时候显示东华，结束计时器

    void sendsignals();         //激发自定义信号

    Ui::GameWindow* ui;

public slots:
    void slot_showBackgroundImage();

    //植物卡片按钮
    void slot_clickNormalPeaPushButton();
    void slot_clickWallNutPushButton();
    void slot_clickIcePeaPushButton();
    void slot_clickCherryBombPushButton();
    void slot_clickJalapenoPushButton();

    //铲子按钮
    void slot_clickShovelPushButton();

    //与僵尸有关的slot
    void slot_produceNormalZombie();    //生成普通僵尸
    void slot_produceBucketZombie();    //生成铁桶僵尸
    void slot_produceConeZombie();      //生产路障僵尸
    void slot_change_P_and_Z_state();   //每一帧修改植物和僵尸的状态
    void slot_deleteDeadPlant();       //删除死亡植物
    void slot_checkBulletHitZombie();  //检查子弹是否命中
    void slot_deleteDeadZombie();      //删除死亡僵尸
    void slot_deleteOutOfRangeBullet();//删除飞出屏幕外的子弹

    //与游戏进程有关的slot
    void slot_checkGameLose();  //检查游戏是否结束
    void slot_checkGameWin();   //检查游戏是否胜利
    void slot_stopAllTimer();   //停止所有计时器

    void slot_LawnCleanerInitial();            //小车初始化
    void slot_LawnCleanerActivate();           //小车激活
    void slot_LawnCleanerAttack();             //小车攻击
    void slot_deleteOutOfRangeLawnCleaner();   //删除小车

signals:
    void mysignal();  //自定义信号
};

#endif // GAMEWINDOW_H
