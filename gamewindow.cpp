#include "gamewindow.h"
#include "ui_gamewindow.h"
#include <QDebug>
#include <iostream>
#include <QTime>

GameWindow::GameWindow(QWidget* parent) :
    QWidget(parent),
    ui(new Ui::GameWindow){

    this->setWindowTitle("Plants vs Zombies");
    this->setWindowIcon(QIcon(":/images/Icon.png"));
    ui->setupUi(this);

    //随机数种子的生成
    srand(time(0));
    //游戏主计时器的初始化
    gameWindowTimer = new QTimer(this);
    gameWindowTimer->setInterval(20);     //每秒屏幕刷新50次
    gameWindowTimer->start();

    //游戏背景音乐的设置
    mediaPlayer_gameWindowBackground = new QMediaPlayer(this);
    mediaPlaylist_gameWindowBackground = new QMediaPlaylist(this);
    mediaPlaylist_gameWindowBackground->addMedia(QUrl("qrc:/music/UraniwaNi.mp3"));
    mediaPlaylist_gameWindowBackground->setCurrentIndex(1);
    mediaPlaylist_gameWindowBackground->setPlaybackMode(QMediaPlaylist::CurrentItemInLoop);
    mediaPlayer_gameWindowBackground->setPlaylist(mediaPlaylist_gameWindowBackground);
    mediaPlayer_gameWindowBackground->setVolume(150);
    mediaPlayer_gameWindowBackground->play();

    //点击按钮声音
    mediaPlayer_buttonClicked = new QMediaPlayer(this);
    mediaPlayer_buttonClicked->setMedia(QUrl("qrc:/music/buttonclick.mp3"));
    mediaPlayer_buttonClicked->setVolume(150);

    //子弹打到普通僵尸的音乐
    mediaPlayer_bulletHitZombie = new QMediaPlayer(this);
    mediaPlayer_bulletHitZombie->setMedia(QUrl("qrc:/music/splat1.mp3"));
    mediaPlayer_bulletHitZombie->setVolume(150);

    //子弹打到铁桶僵尸的音乐
    mediaPlayer_bulletHitbucketZombie = new QMediaPlayer(this);
    mediaPlayer_bulletHitbucketZombie->setMedia(QUrl("qrc:/music/shieldhit.mp3"));
    mediaPlayer_bulletHitbucketZombie->setVolume(150);

    //游戏胜利的音效
    mediaPlayer_gameWin = new QMediaPlayer(this);
    mediaPlayer_gameWin->setMedia(QUrl("qrc:/music/winmusic.mp3"));
    mediaPlayer_gameWin->setVolume(150);

    //游戏失败的音效
    mediaPlayer_gameLose = new QMediaPlayer(this);
    mediaPlayer_gameLose->setMedia(QUrl("qrc:/music/scream.mp3"));
    mediaPlayer_gameLose->setVolume(150);

    //警报声的音效
    mediaPlayer_siren = new QMediaPlayer(this);
    mediaPlayer_siren->setMedia(QUrl("qrc:/music/siren.mp3"));
    mediaPlayer_siren->setVolume(150);

    //铲子铲植物
    mediaPlayer_shovel = new QMediaPlayer(this);
    mediaPlayer_shovel->setMedia(QUrl("qrc:/music/shovel.mp3"));
    mediaPlayer_shovel->setVolume(150);

    //背景label相关设置
    myBackGroundLabel = new QLabel(ui->centralwidget);
    backGroundImg = new QImage(":/images/background1.png");
    myBackGroundLabel->setGeometry(0, 0, 900, 600);
    myBackGroundLabel->setScaledContents(true);
    myBackGroundLabel->setPixmap(QPixmap::fromImage(*backGroundImg));
    this->scrollBackgroundImage();    //开始场景混动

    //小推车的初始化
    for(int i = 0; i < 5; i++)
    {
        lawnCleanerArray[i] = nullptr;
    }

    gameWindowTimer->singleShot(5000, this, &GameWindow::slot_LawnCleanerInitial);

    //铲子拦相关设置
    shovelHolder = new QLabel(ui->centralwidget);
    shovelHolder->setGeometry(380, 10, 111, 41);
    shovelHolder->setScaledContents(true);
    shovelHolderImg = new QImage(":/images/ShovelBack.png");
    shovelHolder->setPixmap(QPixmap::fromImage(*shovelHolderImg));

    //卡片栏相关设置
    cardHolder = new QLabel(ui->centralwidget);
    cardHolder->setGeometry(10, 10, 73, 600);
    cardHolder->setScaledContents(true);
    cardHolderImg = new QImage(":/images/SeedChooser_Background.png");
    cardHolder->setPixmap(QPixmap::fromImage(*cardHolderImg));

    //豌豆射手卡片的设置
    normalPeaPushButton = new QPushButton(ui->centralwidget);
    normalPeaPushButton->setGeometry(15, 50, 65, 90);
    normalPeaPushButton->setIcon(QIcon(":/images/card_peashooter_move.png"));
    normalPeaPushButton->setIconSize(QSize(65, 90));

    //寒冰射手卡片的设置
    icePeaPushButton = new QPushButton(ui->centralwidget);
    icePeaPushButton->setGeometry(15, 145, 65, 90);
    icePeaPushButton->setIcon(QIcon(":/images/card_snowpea_move.png"));
    icePeaPushButton->setIconSize(QSize(65, 90));

    //坚果卡片的设置
    wallNutPushButton = new QPushButton(ui->centralwidget);
    wallNutPushButton->setGeometry(15, 240, 65, 90);
    wallNutPushButton->setIcon(QIcon(":/images/card_wallnut_move.png"));
    wallNutPushButton->setIconSize(QSize(65, 90));

    //樱桃炸弹卡片的设置
    cherryBombPushButton = new QPushButton(ui->centralwidget);
    cherryBombPushButton->setGeometry(15, 335, 65, 90);
    cherryBombPushButton->setIcon(QIcon(":/images/card_cherrybomb_move.png"));
    cherryBombPushButton->setIconSize(QSize(65, 90));

    //火爆辣椒卡片的设置
    jalapenoPushButton = new QPushButton(ui->centralwidget);
    jalapenoPushButton->setGeometry(15, 430, 65, 90);
    jalapenoPushButton->setIcon(QIcon(":/images/card_jalapeno.png"));
    jalapenoPushButton->setIconSize(QSize(65, 90));

    //铲子卡片的设置
    shovelPushButton = new QPushButton(ui->centralwidget);
    shovelPushButton->setGeometry(380, 10, 111, 41);
    shovelPushButton->setIcon(QIcon(":/images/Shovel.png"));
    shovelPushButton->setIconSize(QSize(111, 41));

    //去除边框
    shovelPushButton->setFlat(true);

    //二维bool数组初始化
    memset(isFieldPlantable, true, sizeof(isFieldPlantable));
    memset(hasFieldPlanted, 0, sizeof(hasFieldPlanted));

    //5个植物数组的初始化
    for(int i = 0; i < 5; i++){
        for(int j = 0 ; j < 9; j++){
            normalPeaArray[i][j] = nullptr;
        }
    }

    for(int i = 0; i < 5; i++) {
        for(int j = 0; j < 9; j++){
            icePeaArray[i][j] = nullptr;
        }
    }

    for(int i = 0; i < 5; i++){
        for(int j = 0; j < 9; j++){
            cherryBombArray[i][j] = nullptr;
        }
    }

    for(int i = 0;i < 5; i++){
        for(int j = 0; j < 9; j++){
            jalapenoArray[i][j] = nullptr;
        }
    }

    for(int i = 0; i < 5; i++){
        for(int j = 0; j < 9; j++){
            wallNutArray[i][j] = nullptr;
        }
    }

    //各个button相关的连接
    QObject::connect(normalPeaPushButton, &QPushButton::clicked, this, &GameWindow::slot_clickNormalPeaPushButton);
    QObject::connect(wallNutPushButton, &QPushButton::clicked, this, &GameWindow::slot_clickWallNutPushButton);
    QObject::connect(icePeaPushButton, &QPushButton::clicked, this, &GameWindow::slot_clickIcePeaPushButton);
    QObject::connect(cherryBombPushButton, &QPushButton::clicked, this, &GameWindow::slot_clickCherryBombPushButton);
    QObject::connect(jalapenoPushButton, &QPushButton::clicked, this, &GameWindow::slot_clickJalapenoPushButton);
    QObject::connect(shovelPushButton, &QPushButton::clicked, this, &GameWindow::slot_clickShovelPushButton);

    //与小车有关的连接
    QObject::connect(gameWindowTimer, &QTimer::timeout, this, &GameWindow::slot_LawnCleanerActivate);
    QObject::connect(gameWindowTimer, &QTimer::timeout, this, &GameWindow::slot_LawnCleanerAttack);
    QObject::connect(gameWindowTimer, &QTimer::timeout, this, &GameWindow::slot_deleteOutOfRangeLawnCleaner);

    //铁桶僵尸生产计时器及僵尸产生
    bucketZombieProduceTimer = new QTimer(this);
    bucketZombieProduceTimer->setInterval(bucketZombieProduceInterval);
    bucketZombieProduceTimer->start();
    QObject::connect(bucketZombieProduceTimer, &QTimer::timeout, this, &GameWindow::slot_produceBucketZombie);

    //普通僵尸生产计时器及僵尸产生
    normalZombieProduceTimer = new QTimer(this);
    normalZombieProduceTimer->setInterval(normalZombieProduceInterval);
    normalZombieProduceTimer->start();
    QObject::connect(normalZombieProduceTimer, &QTimer::timeout, this, &GameWindow::slot_produceNormalZombie);

    //路障僵尸生产计时器及僵尸产生
    coneheadZombieProduceTimer = new QTimer(this);
    coneheadZombieProduceTimer->setInterval(coneheadZombieProduceInterval);
    coneheadZombieProduceTimer->start();
    QObject::connect(coneheadZombieProduceTimer, &QTimer::timeout, this, &GameWindow::slot_produceConeZombie);

    //僵尸和植物交互的几个connect
    QObject::connect(gameWindowTimer, &QTimer::timeout, this, &GameWindow::slot_change_P_and_Z_state);    //改变僵尸和植物的状态
    QObject::connect(gameWindowTimer, &QTimer::timeout, this, &GameWindow::slot_deleteDeadPlant);         //删掉已经死的植物
    QObject::connect(gameWindowTimer, &QTimer::timeout, this, &GameWindow::slot_checkBulletHitZombie);    //检查子弹是否命中僵尸
    QObject::connect(gameWindowTimer, &QTimer::timeout, this, &GameWindow::slot_deleteDeadZombie);        //删除已经死掉的僵尸
    QObject::connect(gameWindowTimer, &QTimer::timeout, this, &GameWindow::slot_deleteOutOfRangeBullet);  //删除飞出屏幕外的子弹

    //游戏结束的几个label的设置
    //游戏胜利的设置
    gameWinLabel = new QLabel(ui->centralwidget);
    gameWinLabel->setScaledContents(true);
    gameWinImg = new QImage(":/images/trophy.png");
    gameWinLabel->setPixmap(QPixmap::fromImage(*gameWinImg));
    gameWinLabel->hide();
    QObject::connect(gameWindowTimer, &QTimer::timeout, this, &GameWindow::slot_checkGameWin);

    //游戏失败的设置
    gameLoseLabel = new QLabel(ui->centralwidget);
    gameLoseLabel->setScaledContents(true);
    gameLoseImg = new QImage(":/images/ZombiesWon.png");
    gameLoseLabel->setPixmap(QPixmap::fromImage(*gameLoseImg));
    gameLoseLabel->hide();
    QObject::connect(gameWindowTimer, &QTimer::timeout, this, &GameWindow::slot_checkGameLose);

    //植物重生策略
    QTimer* t = new QTimer(this);
    t->setInterval(20);
    t->start();
    QObject::connect(t, &QTimer::timeout, [&](){
        if(!ableToSetNormalPea && !ableToSetIcePea && !ableToSetWallNut && !ableToSetCherryBomb && !ableToSetJalapeno){
            int luck = rand() % 100;
            if(luck > 70){
                ableToSetNormalPea = true;
                normalPeaPushButton->setDisabled(false);
            }

            luck = rand() % 100;
            if(luck > 70){
                ableToSetIcePea = true;
                icePeaPushButton->setDisabled(false);
            }

            luck = rand() % 100;
            if(luck > 60){
                ableToSetWallNut = true;
                wallNutPushButton->setDisabled(false);
            }

            luck = rand() % 100;
            if(luck > 90){
                ableToSetCherryBomb = true;
                cherryBombPushButton->setDisabled(false);
            }

            luck = rand() % 100;
            if(luck > 90){
                ableToSetJalapeno = true;
                jalapenoPushButton->setDisabled(false);
            }
        }
    });


}

GameWindow::~GameWindow(){
    delete ui;
}


//计算距离
double GameWindow::getdist(double x1, double y1, double x2, double y2){
    return sqrt((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2));
}

//删除小车
void GameWindow::slot_deleteOutOfRangeLawnCleaner(){
    for(int i = 0; i < 5; i++){
        if(lawnCleanerArray[i] != nullptr){
            if(lawnCleanerArray[i]->pos().x() >= 900){
                if(lawnCleanerArray[i]->isRunning == true){
                    delete lawnCleanerArray[i];
                    lawnCleanerArray[i] = nullptr;
                }
            }
        }
    }
}

//小车攻击
void GameWindow::slot_LawnCleanerAttack(){
    for(int i=0; i<5; i++){
        if(lawnCleanerArray[i] != nullptr){
            if(lawnCleanerArray[i]->isActivited == true && lawnCleanerArray[i]->isRunning == true){
                //遍历普通僵尸
                for( int z = 0; z < normalZombieList.size(); z++){
                    if(std::fabs(lawnCleanerArray[i]->pos().x() - normalZombieList[z]->getZombiePosX() )<= 10 && i == normalZombieList[z]->getZombieRow()){
                        normalZombieList[z]->changeZombieLife(1500);

                    }
                }

                //铁桶僵尸
                for( int bZ = 0; bZ < bucketZombieList.size() ; bZ++){
                    if(std::fabs(lawnCleanerArray[i]->pos().x() - bucketZombieList[bZ]->getZombiePosX() )<= 10 && i == bucketZombieList[bZ]->getZombieRow()){
                        bucketZombieList[bZ]->changeZombieLife(1500);
                    }
                }

                //路障僵尸
                for( int pvZ = 0; pvZ < coneZombieList.size() ; pvZ++){
                    if(std::fabs(lawnCleanerArray[i]->pos().x()-82-50 - coneZombieList[pvZ]->getZombiePosX() )<= 10 && i == coneZombieList[pvZ]->getZombieRow()){
                        coneZombieList[pvZ]->changeZombieLife(1500);
                    }
                }
            }
        }
    }
}

//小车激活
void GameWindow::slot_LawnCleanerActivate(){
    for(int i = 0; i < 5; i++){
        if(lawnCleanerArray[i] != nullptr){
            if(lawnCleanerArray[i]->isActivited == false && lawnCleanerArray[i]->isRunning == false){
                //普通僵尸
                for( int z = 0; z < normalZombieList.size() ; z++){
                    if(std::fabs(lawnCleanerArray[i]->pos().x() - normalZombieList[z]->getZombiePosX() )<= 10 && i == normalZombieList[z]->getZombieRow()){
                        lawnCleanerArray[i]->isActivited = true;
                        return;
                    }
                }

                //铁桶僵尸
                for( int bZ = 0; bZ < bucketZombieList.size() ; bZ++){
                    if(std::fabs(lawnCleanerArray[i]->pos().x() - bucketZombieList[bZ]->getZombiePosX() )<= 10 && i == bucketZombieList[bZ]->getZombieRow()){
                        lawnCleanerArray[i]->isActivited = true;
                        return;
                    }
                }

                //路障僵尸
                for( int pvZ = 0; pvZ < coneZombieList.size() ; pvZ++){
                    if(std::fabs(lawnCleanerArray[i]->pos().x() - coneZombieList[pvZ]->getZombiePosX() )<= 10 && i == coneZombieList[pvZ]->getZombieRow()){
                        lawnCleanerArray[i]->isActivited = true;
                        return;
                    }
                }
            }
        }
    }
}

//小推车初始化
void GameWindow::slot_LawnCleanerInitial(){
    //播放音效
    mediaPlayer_siren->play();
    //初始化
    for(int row = 0; row < 5; row++){
        lawnCleanerArray[row] = new LawnCleaner(ui->centralwidget);
        lawnCleanerArray[row]->hide();
        lawnCleanerArray[row]->row = row;
        lawnCleanerArray[row]->slot_enter();
    }
}


//让所有timer停止
void GameWindow::slot_stopAllTimer(){
    //游戏主窗口QTimer停止
    gameWindowTimer->stop();
    //射手timer的停止
    for(int row = 0; row < 5; row++){
        for(int col = 0; col < 9; col++){
            if(normalPeaArray[row][col] != nullptr){
                normalPeaArray[row][col]->shootingTriggerTimer->stop();
            }
            if(icePeaArray[row][col] != nullptr){
                icePeaArray[row][col]->shootingTriggerTimer->stop();
            }
        }
    }

    //各个僵尸的timer的停止
    //普通僵尸
    QList<normalZombie*>::iterator z;
    for(z = normalZombieList.begin(); z != normalZombieList.end(); z++){
        (*z)->zombieTimer->stop();
    }

    //铁桶僵尸
    QList<bucketheadZombie*>::iterator bZ;
    for(bZ = bucketZombieList.begin(); bZ != bucketZombieList.end(); bZ++){
        (*bZ)->zombieTimer->stop();
    }

    //路障僵尸
    QList<coneheadZombie*>::iterator pvZ;
    for(pvZ = coneZombieList.begin(); pvZ != coneZombieList.end(); pvZ++){
        (*pvZ)->zombieTimer->stop();
    }
}

//游戏胜利时动画与音效
void GameWindow::func_gameWin(){
    gameWinLabel->show();
    gameWinLabel->raise();
    QPropertyAnimation* tempAnimation;
    tempAnimation = new QPropertyAnimation(gameWinLabel, "geometry");
    tempAnimation->setDuration(5000);
    tempAnimation->setStartValue(QRect(450, 300, 0, 0));
    tempAnimation->setEndValue(QRect(250, 80, 500, 440));
    tempAnimation->start();
    mediaPlayer_gameWin->play();
}

void GameWindow::slot_checkGameWin(){
    if(normalZombieRound >= normalZombieRoundUpperLimit && bucketZombieRound >= bucketZombieRoundUpperLimit && coneheadZombieRound >= coneheadZombieRoundUpperLimit){
        if(normalZombieList.isEmpty() == true && bucketZombieList.isEmpty() == true && coneZombieList.isEmpty() == true){
            slot_stopAllTimer();
            func_gameWin();
            return;
        }
    }
}

//游戏失败时动画与音效
void GameWindow::func_gameLose(){
    gameLoseLabel->show();
    gameLoseLabel->raise();
    QPropertyAnimation* tempAnimation;
    tempAnimation = new QPropertyAnimation(gameLoseLabel,"geometry");
    tempAnimation->setDuration(5000);
    tempAnimation->setStartValue(QRect(450, 300, 0, 0));
    tempAnimation->setEndValue(QRect(250, 80, 500, 440));
    tempAnimation->start();
    mediaPlayer_gameLose->play();
}

//检查游戏是否失败
void GameWindow::slot_checkGameLose(){
    //普通僵尸
    for(int z = 0; z < normalZombieList.size(); z++){
        if(normalZombieList[z]->getZombiePosX() <= 10){
            slot_stopAllTimer();
            func_gameLose();
            return;
        }
    }
    //铁桶僵尸
    for(int z = 0; z < bucketZombieList.size(); z++){
        if(bucketZombieList[z]->getZombiePosX() <= 10){
            slot_stopAllTimer();
            func_gameLose();
            return;
        }
    }
    //路障僵尸
    for(int z = 0; z < coneZombieList.size(); z++){
        if(coneZombieList[z]->getZombiePosX() <= 10){
            slot_stopAllTimer();
            func_gameLose();
            return;
        }
    }
}

//删除飞出屏幕外的子弹
void GameWindow::slot_deleteOutOfRangeBullet(){
    for(int row = 0; row < 5; row++){
        for(int col = 0; col < 9; col++){
            //普通射手
            if(hasFieldPlanted[row][col] == 1){
                for(int b = 0; b < normalPeaArray[row][col]->bulletList.size(); b++){
                        if(normalPeaArray[row][col]->bulletList[b]->pos().x() >= 900){
                            delete normalPeaArray[row][col]->bulletList[b];
                            normalPeaArray[row][col]->bulletList.removeAt(b);//删除这个子弹
                        }
                }
            //寒冰射手
            if(hasFieldPlanted[row][col] == 2){
                for(int b = 0; b < icePeaArray[row][col]->bulletList.size(); b++){
                    if(icePeaArray[row][col]->bulletList[b]->pos().x() >= 900){
                        delete icePeaArray[row][col]->bulletList[b];
                        icePeaArray[row][col]->bulletList.removeAt(b);//删除这个子弹
                    }
                }
            }
        }
    }
}
}

//删除死亡僵尸
void GameWindow::slot_deleteDeadZombie(){
    //遍历普通僵尸
    for(int z = 0; z < normalZombieList.size(); z++){
        if(normalZombieList[z]->getZombieLife() <= 0||normalZombieList[z]->isburn){
             //修改这一行豌豆的状态
             for(int col = 0; col < 9; col++){
                 if(hasFieldPlanted[normalZombieList[z]->getZombieRow()][col] == 1){
                        normalPeaArray[normalZombieList[z]->getZombieRow()][col]->isAttacking = false;
                 }
                 if(hasFieldPlanted[normalZombieList[z]->getZombieRow()][col] == 2){
                        icePeaArray[normalZombieList[z]->getZombieRow()][col]->isAttacking = false;
                 }
             }
             if(normalZombieList[z]->normalDead->currentFrameNumber()==normalZombieList[z]->normalDead->frameCount()-1){
                 delete normalZombieList[z];
                 normalZombieList.removeAt(z);
             }
             else if(normalZombieList[z]->bombDead->currentFrameNumber()==normalZombieList[z]->bombDead->frameCount()-1){
                 delete normalZombieList[z];
                 normalZombieList.removeAt(z);
             }
        }
    }

    //遍历铁桶僵尸
    for(int z=0;z<bucketZombieList.size();z++)
    {
        if(bucketZombieList[z]->getZombieLife() <= 0||bucketZombieList[z]->isburn){
            //修改这一行豌豆的状态
             for(int col = 0; col < 9; col++){
                 if(hasFieldPlanted[bucketZombieList[z]->getZombieRow()][col] == 1){
                        normalPeaArray[bucketZombieList[z]->getZombieRow()][col]->isAttacking = false;
                 }
                 if(hasFieldPlanted[bucketZombieList[z]->getZombieRow()][col] == 2){
                        icePeaArray[bucketZombieList[z]->getZombieRow()][col]->isAttacking = false;
                 }
             }
             if(bucketZombieList[z]->normalDead->currentFrameNumber()==bucketZombieList[z]->normalDead->frameCount()-1){
                 delete bucketZombieList[z];
                 bucketZombieList.removeAt(z);
             }
             else if(bucketZombieList[z]->bombDead->currentFrameNumber()==bucketZombieList[z]->bombDead->frameCount()-1){
                 delete bucketZombieList[z];
                 bucketZombieList.removeAt(z);
             }

         }
    }
    //路障
    for(int z = 0; z < coneZombieList.size(); z++){
        if(coneZombieList[z]->getZombieLife() <= 0||coneZombieList[z]->isburn){
            //修改这一行豌豆的状态
            for(int col = 0; col < 9; col++){
                if(hasFieldPlanted[coneZombieList[z]->getZombieRow()][col] == 1){
                    normalPeaArray[coneZombieList[z]->getZombieRow()][col]->isAttacking = false;
                }
                if(hasFieldPlanted[coneZombieList[z]->getZombieRow()][col] == 2){
                    icePeaArray[coneZombieList[z]->getZombieRow()][col]->isAttacking = false;
                }
            }
            if(coneZombieList[z]->normalDead->currentFrameNumber()==coneZombieList[z]->normalDead->frameCount()-1){
                delete coneZombieList[z];
                coneZombieList.removeAt(z);
            }
            else if(coneZombieList[z]->bombDead->currentFrameNumber()==coneZombieList[z]->bombDead->frameCount()-1){
                delete coneZombieList[z];
                coneZombieList.removeAt(z);
            }
        }
    }
}


//检查子弹是否命中僵尸
void GameWindow::slot_checkBulletHitZombie(){
    for(int row = 0; row < 5; row++){
        for(int col = 0; col < 9; col++){
            //如果是普通豌豆
            if(hasFieldPlanted[row][col] == 1){
                if(normalPeaArray[row][col]->isAttacking == true){
                    for(int b = 0; b < normalPeaArray[row][col]->bulletList.size(); b++){
                        for(int z = 0; z < normalZombieList.size() ; z++){
                            if(row == normalZombieList[z]->getZombieRow() &&  normalPeaArray[row][col]->bulletList[b]->pos().x() > normalZombieList[z]->getZombiePosX() ){
                                mediaPlayer_bulletHitZombie->play();
                                normalZombieList[z]->changeZombieLife(normalPeaArray[row][col]->bulletList[b]->Returndamage());//对僵尸造成伤害
                                normalPeaArray[row][col]->bulletList[b]->bulletExplode();
                                normalPeaArray[row][col]->bulletList.removeAt(b);//删除这个子弹
                                --b;//保证子弹不会miss掉
                                break;//跳出
                            }
                        }

                    }

                    for(int b = 0; b < normalPeaArray[row][col]->bulletList.size(); b++){
                        for(int z = 0; z < bucketZombieList.size() ; z++){
                            if(row == bucketZombieList[z]->getZombieRow() &&  normalPeaArray[row][col]->bulletList[b]->pos().x() >bucketZombieList[z]->getZombiePosX() ){
                                mediaPlayer_bulletHitZombie->play();
                                bucketZombieList[z]->changeZombieLife(normalPeaArray[row][col]->bulletList[b]->Returndamage());//对僵尸造成伤害
                                normalPeaArray[row][col]->bulletList[b]->bulletExplode();
                                normalPeaArray[row][col]->bulletList.removeAt(b);//删除这个子弹
                                --b;//保证子弹不会miss掉
                                break;//跳出
                            }
                        }
                    }

                    for(int b = 0; b < normalPeaArray[row][col]->bulletList.size(); b++){
                        for(int z = 0; z < coneZombieList.size() ; z++){
                            if(row == coneZombieList[z]->getZombieRow() &&  normalPeaArray[row][col]->bulletList[b]->pos().x() > coneZombieList[z]->getZombiePosX() ){
                                mediaPlayer_bulletHitZombie->play();
                                coneZombieList[z]->changeZombieLife(normalPeaArray[row][col]->bulletList[b]->Returndamage());//对僵尸造成伤害
                                normalPeaArray[row][col]->bulletList[b]->bulletExplode();
                                normalPeaArray[row][col]->bulletList.removeAt(b);//删除这个子弹
                                --b;//保证子弹不会miss掉
                                break;//跳出
                            }
                        }

                    }

                }
            }
            //如果是寒冰豌豆
            if(hasFieldPlanted[row][col] == 2){
                if(icePeaArray[row][col]->isAttacking == true){
                    for(int b = 0; b < icePeaArray[row][col]->bulletList.size(); b++){
                        for(int z = 0; z < normalZombieList.size() ; z++){
                            if(row == normalZombieList[z]->getZombieRow() &&  icePeaArray[row][col]->bulletList[b]->pos().x() > normalZombieList[z]->getZombiePosX() ){
                                    mediaPlayer_bulletHitZombie->play();
                                    normalZombieList[z]->changeZombieLife(icePeaArray[row][col]->bulletList[b]->Returndamage());//对僵尸造成伤害
                                    normalZombieList[z]->changeZombieMovingSpeed(0.15);
                                    icePeaArray[row][col]->bulletList[b]->bulletExplode();
                                    icePeaArray[row][col]->bulletList.removeAt(b);//删除这个子弹
                                    --b;//保证子弹不会miss掉
                                    break;//跳出
                            }
                        }

                    }

                    for(int b = 0; b < icePeaArray[row][col]->bulletList.size(); b++){
                        for(int z = 0; z < bucketZombieList.size() ; z++){
                            if(row == bucketZombieList[z]->getZombieRow() &&  icePeaArray[row][col]->bulletList[b]->pos().x() >bucketZombieList[z]->getZombiePosX() ){
                                mediaPlayer_bulletHitZombie->play();
                                bucketZombieList[z]->changeZombieLife(icePeaArray[row][col]->bulletList[b]->Returndamage());//对僵尸造成伤害
                                bucketZombieList[z]->changeZombieMovingSpeed(0.15);
                                icePeaArray[row][col]->bulletList[b]->bulletExplode();
                                icePeaArray[row][col]->bulletList.removeAt(b);//删除这个子弹
                                --b;//保证子弹不会miss掉
                                break;//跳出
                            }
                        }
                    }

                    for(int b = 0; b < icePeaArray[row][col]->bulletList.size(); b++){
                        for(int z = 0; z < coneZombieList.size() ; z++){
                            if(row == coneZombieList[z]->getZombieRow() &&  icePeaArray[row][col]->bulletList[b]->pos().x() > coneZombieList[z]->getZombiePosX() ){
                                mediaPlayer_bulletHitZombie->play();
                                coneZombieList[z]->changeZombieLife(icePeaArray[row][col]->bulletList[b]->Returndamage());//对僵尸造成伤害
                                coneZombieList[z]->changeZombieMovingSpeed(0.15);
                                icePeaArray[row][col]->bulletList[b]->bulletExplode();
                                icePeaArray[row][col]->bulletList.removeAt(b);//删除这个子弹
                                --b;//保证子弹不会miss掉
                                break;//跳出
                            }
                        }

                    }

                }
            }
        }
    }
}

//删除死亡的植物
void GameWindow::slot_deleteDeadPlant(){
    for(int row = 0; row < 5;row++){
        for(int col = 0; col < 9; col++){
            if(hasFieldPlanted[row][col]>0){
                switch(hasFieldPlanted[row][col]){
                    case 1:
                        if(normalPeaArray[row][col]->Returnlife() <= 0){
                            for(int z = 0; z < normalZombieList.size() ; z++){
                                if(std::fabs( 120 + col*82 + 55 - normalZombieList[z]->getZombiePosX()) <= 10  && row == normalZombieList[z]->getZombieRow()){
                                    normalZombieList[z]->changeZombieStatus(false);//修改僵尸的状态
                                }
                            }

                            for(int z = 0; z < bucketZombieList.size() ; z++){
                                if(std::fabs( 120 + col*82 + 55 - bucketZombieList[z]->getZombiePosX()) <= 10  && row == bucketZombieList[z]->getZombieRow()){
                                    bucketZombieList[z]->changeZombieStatus(false);//修改僵尸的状态
                                }
                            }

                            for(int z = 0; z < coneZombieList.size() ; z++){
                                if(std::fabs( 120 + col*82 + 55 - coneZombieList[z]->getZombiePosX()) <= 10  && row == coneZombieList[z]->getZombieRow()){
                                    coneZombieList[z]->changeZombieStatus(false);//修改僵尸的状态
                                }
                            }
                            delete normalPeaArray[row][col];
                            normalPeaArray[row][col] = nullptr;//删除一株向日葵并让其指向空指针
                            hasFieldPlanted[row][col] = 0;
                            isFieldPlantable[row][col] = true;//这块地又可以种植物了
                        }
                        break;
                    case 2:
                        if(icePeaArray[row][col]->Returnlife() <= 0){
                            for(int z = 0; z < normalZombieList.size() ; z++){
                                if(std::fabs( 120 + col*82 + 55 - normalZombieList[z]->getZombiePosX()) <= 10  && row == normalZombieList[z]->getZombieRow()){
                                  normalZombieList[z]->changeZombieStatus(false);//修改僵尸的状态
                             }
                         }

                            for(int z = 0; z < bucketZombieList.size() ; z++){
                                if(std::fabs( 120 + col*82 + 55 - bucketZombieList[z]->getZombiePosX()) <= 10  && row == bucketZombieList[z]->getZombieRow()){
                                    bucketZombieList[z]->changeZombieStatus(false);//修改僵尸的状态
                                }
                            }

                            for(int z = 0; z < coneZombieList.size() ; z++){
                                if(std::fabs( 120 + col*82 + 55 - coneZombieList[z]->getZombiePosX()) <= 10  && row == coneZombieList[z]->getZombieRow()){
                                   coneZombieList[z]->changeZombieStatus(false);//修改僵尸的状态
                                }
                            }
                            delete icePeaArray[row][col];
                            icePeaArray[row][col] = nullptr;//删除一株向日葵并让其指向空指针
                            hasFieldPlanted[row][col] = 0;
                            isFieldPlantable[row][col] = true;//这块地又可以种植物了
                        }
                        break;
                    case 3:
                        if(wallNutArray[row][col]->Returnlife() <= 0){
                            for(int z = 0; z < normalZombieList.size() ; z++){
                                if(std::fabs( 120 + col*82 + 55 - normalZombieList[z]->getZombiePosX()) <= 10  && row == normalZombieList[z]->getZombieRow()){
                                    normalZombieList[z]->changeZombieStatus(false);//修改僵尸的状态
                                }
                            }

                            for(int z = 0; z < bucketZombieList.size(); z++){
                                if(std::fabs( 120 + col*82 + 55 - bucketZombieList[z]->getZombiePosX()) <= 10  && row == bucketZombieList[z]->getZombieRow()){
                                 bucketZombieList[z]->changeZombieStatus(false);//修改僵尸的状态
                                }
                            }

                            for(int z = 0; z < coneZombieList.size() ; z++){
                                if(std::fabs( 120 + col*82 + 55 - coneZombieList[z]->getZombiePosX()) <= 10  && row == coneZombieList[z]->getZombieRow()){
                                    coneZombieList[z]->changeZombieStatus(false);
                                }
                            }
                            delete wallNutArray[row][col];
                            wallNutArray[row][col] = nullptr;
                            hasFieldPlanted[row][col] = 0;
                            isFieldPlantable[row][col] = true;//这块地又可以种植物了
                        }
                        break;
                    default:
                        break;
                }
            }
        }
    }
}



//修改植物和僵尸的状态
void GameWindow::slot_change_P_and_Z_state(){
    for(int row = 0; row < 5; row++){
        for(int col = 0; col < 9; col++){
            if(hasFieldPlanted[row][col] > 0){
                //对普通僵尸遍历
                for(int z = 0; z < normalZombieList.size() ; z++){
                    if(normalZombieList[z]->getZombieRow() == row){
                        if(hasFieldPlanted[row][col] == 1)
                            normalPeaArray[row][col]->isAttacking = true;//让这一行的豌豆射手攻击
                        else if(hasFieldPlanted[row][col] == 2)
                            icePeaArray[row][col]->isAttacking = true;
                    }

                    if(std::fabs( 120 + col*82 + 55 - normalZombieList[z]->getZombiePosX()) <= 10  && row == normalZombieList[z]->getZombieRow()){
                        normalZombieList[z]->changeZombieStatus(true);//修改僵尸的状态
                        switch(hasFieldPlanted[row][col]){
                            case 1:
                                normalPeaArray[row][col]->SetLife(normalPeaArray[row][col]->Returnlife()-normalZombieList[z]->getZombieDamageValue());
                                break;
                            case 2:
                                icePeaArray[row][col]->SetLife(icePeaArray[row][col]->Returnlife()-normalZombieList[z]->getZombieDamageValue());
                                break;
                            case 3:
                                wallNutArray[row][col]->SetLife(wallNutArray[row][col]->Returnlife()-normalZombieList[z]->getZombieDamageValue());
                                break;
                            default:
                                break;
                        }
                    }
                }

                //对铁桶僵尸遍历
                for(int z = 0; z < bucketZombieList.size(); z++){
                    if(bucketZombieList[z]->getZombieRow() == row){
                        if(hasFieldPlanted[row][col] == 1)
                            normalPeaArray[row][col]->isAttacking = true;//让这一行的豌豆射手攻击
                        else if(hasFieldPlanted[row][col] == 2)
                            icePeaArray[row][col]->isAttacking = true;
                    }

                    if(std::fabs( 120 + col*82 + 55 - bucketZombieList[z]->getZombiePosX()) <= 10  && row == bucketZombieList[z]->getZombieRow()){
                        bucketZombieList[z]->changeZombieStatus(true);//修改僵尸的状态
                        switch(hasFieldPlanted[row][col]){
                            case 1:
                                normalPeaArray[row][col]->SetLife(normalPeaArray[row][col]->Returnlife()-bucketZombieList[z]->getZombieDamageValue());
                                break;
                            case 2:
                                icePeaArray[row][col]->SetLife(icePeaArray[row][col]->Returnlife()-bucketZombieList[z]->getZombieDamageValue());
                                break;
                            case 3:
                                wallNutArray[row][col]->SetLife(wallNutArray[row][col]->Returnlife()-bucketZombieList[z]->getZombieDamageValue());
                                break;
                            default:
                                break;
                        }
                    }
                }
                //对路障僵尸遍历
                for(int z = 0; z < coneZombieList.size(); z++){
                    if(coneZombieList[z]->getZombieRow() == row){
                        if(hasFieldPlanted[row][col] == 1)
                            normalPeaArray[row][col]->isAttacking = true;//让这一行的豌豆射手攻击
                        else if(hasFieldPlanted[row][col] == 2)
                            icePeaArray[row][col]->isAttacking = true;
                    }

                    if(std::fabs( 120 + col*82 + 55 - coneZombieList[z]->getZombiePosX()) <= 10  && row == coneZombieList[z]->getZombieRow()){
                        coneZombieList[z]->changeZombieStatus(true);//修改僵尸的状态
                        switch(hasFieldPlanted[row][col]){
                            case 1:
                                normalPeaArray[row][col]->SetLife(normalPeaArray[row][col]->Returnlife()-coneZombieList[z]->getZombieDamageValue());
                                break;
                            case 2:
                                icePeaArray[row][col]->SetLife(icePeaArray[row][col]->Returnlife()-coneZombieList[z]->getZombieDamageValue());
                                break;
                            case 3:
                                wallNutArray[row][col]->SetLife(wallNutArray[row][col]->Returnlife()-coneZombieList[z]->getZombieDamageValue());
                                break;
                            default:
                                break;
                        }
                    }
                }
            }
        }
    }
}

//生产普通僵尸
void GameWindow::slot_produceNormalZombie(){
    if(normalZombieRound > 0 && normalZombieRound <= normalZombieRoundUpperLimit){
        normalZombie *tempnormalZombie = new normalZombie(ui->centralwidget);
        tempnormalZombie->changeZombieRow(rand()%5);
        tempnormalZombie->changeZombiePosY(60.0 + tempnormalZombie->getZombieRow()*100.0-30);
        tempnormalZombie->move(tempnormalZombie->getZombiePosX(),tempnormalZombie->getZombiePosY());
        tempnormalZombie->show();
        normalZombieList << tempnormalZombie;
    }
    normalZombieRound++;
}
//生产铁桶僵尸
void GameWindow::slot_produceBucketZombie(){
    if(bucketZombieRound > 0 && bucketZombieRound <= bucketZombieRoundUpperLimit){
        bucketheadZombie *tempBucketZombie = new bucketheadZombie(ui->centralwidget);
        tempBucketZombie->changeZombieRow(rand()%5);
        tempBucketZombie->changeZombiePosY(60.0 + tempBucketZombie->getZombieRow()*100.0-30);
        tempBucketZombie->move(tempBucketZombie->getZombiePosX(),tempBucketZombie->getZombiePosY());
        tempBucketZombie->show();
        bucketZombieList << tempBucketZombie;
    }
    bucketZombieRound++;
}
//生产路障僵尸
void GameWindow::slot_produceConeZombie(){
    if(coneheadZombieRound > 0 && coneheadZombieRound <= coneheadZombieRoundUpperLimit){
        coneheadZombie *tempConeZombie = new coneheadZombie(ui->centralwidget);
        tempConeZombie->changeZombieRow(rand()%5);
        tempConeZombie->changeZombiePosY(60.0 + tempConeZombie->getZombieRow()*100.0-30);
        tempConeZombie->move(tempConeZombie->getZombiePosX(),tempConeZombie->getZombiePosY());
        tempConeZombie->show();
        coneZombieList << tempConeZombie;
    }
    coneheadZombieRound++;
}

void GameWindow::mousePressEvent(QMouseEvent *event){
    if(event->button() == Qt::LeftButton){
        int col = (event->pos().x() - 120) / 82;
        int row = (event->pos().y() - 80) / 100;
        if(col < 0 || col >= 9 || row < 0 || row >= 5)//必须点到土地上
            return;
        else{
            if(isFieldPlantable[row][col] == true && hasFieldPlanted[row][col] == 0){
                if(isSettingNormalPea == true && ableToSetNormalPea == true){
                    //几何位置设计
                    normalPeaArray[row][col] = new NormalPea(ui->centralwidget);
                    normalPeaArray[row][col]->setGeometry(120 + 82 * col, 80 + 100*row,82,100);
                    normalPeaArray[row][col]->SetRow(row);
                    normalPeaArray[row][col]->SetCol(col);//修改普通射手内部参数
                    isSettingNormalPea = false;
                    ableToSetNormalPea = false;//修改种植植物状态的参数
                    isFieldPlantable[row][col] = false;
                    hasFieldPlanted[row][col] = 1;//修改关于土地的参数
                    shovelPushButton->setDisabled(false);//解放铲子按钮
                    this->setCursor(Qt::ArrowCursor);//把鼠标形状改回来
                    normalPeaPushButton->setDisabled(true);
                }

                if(isSettingIcePea == true && ableToSetIcePea == true){
                    //几何位置设计
                    icePeaArray[row][col] = new IcePea(ui->centralwidget);
                    icePeaArray[row][col]->setGeometry(120 + 82 * col, 80 + 100*row,82,100);

                    icePeaArray[row][col]->SetRow(row);
                    icePeaArray[row][col]->SetCol(col);//修改向日葵内部参数

                    isSettingIcePea = false;
                    ableToSetIcePea = false;//修改种植植物状态的参数

                    isFieldPlantable[row][col] = false;
                    hasFieldPlanted[row][col] = 2;//修改关于土地的参数

                    shovelPushButton->setDisabled(false);//解放铲子按钮

                    this->setCursor(Qt::ArrowCursor);//把鼠标形状改回来

                    icePeaPushButton->setDisabled(true);
                }
                if(isSettingWallNut == true && ableToSetWallNut == true){
                    wallNutArray[row][col] = new WallNut(ui->centralwidget);
                    wallNutArray[row][col]->setGeometry(120 + 82 * col, 80 + 100*row,82,100);

                    wallNutArray[row][col]->SetRow(row);
                    wallNutArray[row][col]->SetCol(col);//修改向日葵内部参数

                    isSettingWallNut = false;
                    ableToSetWallNut = false;//修改种植植物状态的参数

                    isFieldPlantable[row][col] = false;
                    hasFieldPlanted[row][col] = 3;//修改关于土地的参数

                    shovelPushButton->setDisabled(false);//解放铲子按钮

                    this->setCursor(Qt::ArrowCursor);//把鼠标形状改回来

                    wallNutPushButton->setDisabled(true);
                }

                if(isSettingCherryBomb == true && ableToSetCherryBomb == true){
                    cherryBombArray[row][col] = new CherryBomb(ui->centralwidget);
                    cherryBombArray[row][col]->setGeometry(120 + 82 * col, 80 + 100*row,82,100);

                    cherryBombArray[row][col]->SetRow(row);
                    cherryBombArray[row][col]->SetCol(col);//修改向日葵内部参数

                    isSettingCherryBomb = false;
                    ableToSetCherryBomb = false;//修改种植植物状态的参数

                    isFieldPlantable[row][col] = false;
                    hasFieldPlanted[row][col] = 4;//修改关于土地的参数


                    shovelPushButton->setDisabled(false);//解放铲子按钮

                    this->setCursor(Qt::ArrowCursor);//把鼠标形状改回来

                    cherryBombPushButton->setDisabled(true);
                    cherryBombArray[row][col]->startAttacking();
                    QTimer *waitingtime=new QTimer(this);
                    waitingtime->setInterval(2000);
                    waitingtime->start();
                    for(int i = 0; i < normalZombieList.size(); i++){
                        if(getdist(cherryBombArray[row][col]->x(), cherryBombArray[row][col]->y(), normalZombieList[i]->getZombiePosX(), normalZombieList[i]->getZombiePosY())<150){
                            normalZombieList[i]->isburn=true;
                        }
                    }
                    for(int i = 0; i < bucketZombieList.size(); i++){
                        if(getdist(cherryBombArray[row][col]->x(), cherryBombArray[row][col]->y(), bucketZombieList[i]->getZombiePosX(), bucketZombieList[i]->getZombiePosY())<150){
                            bucketZombieList[i]->isburn=true;
                        }
                    }
                    for(int i = 0; i < coneZombieList.size(); i++){
                        if(getdist(cherryBombArray[row][col]->x(), cherryBombArray[row][col]->y(), coneZombieList[i]->getZombiePosX(), coneZombieList[i]->getZombiePosY())<150){
                            coneZombieList[i]->isburn=true;
                        }
                    }
                    connect(waitingtime, &QTimer::timeout, [=](){
                        delete cherryBombArray[row][col];
                        cherryBombArray[row][col]=nullptr;
                    });
                    isFieldPlantable[row][col] = true;
                    hasFieldPlanted[row][col] = 0;//修改关于土地的参数
                }
                if(isSettingJalapeno == true && ableToSetJalapeno == true){
                    jalapenoArray[row][col] = new Jalapeno(ui->centralwidget);
                    jalapenoArray[row][col]->setGeometry(120 + 82 * col, 80 + 100*row,82,100);

                    jalapenoArray[row][col]->SetRow(row);
                    jalapenoArray[row][col]->SetCol(col);//修改向日葵内部参数

                    isSettingJalapeno = false;
                    ableToSetJalapeno = false;//修改种植植物状态的参数

                    isFieldPlantable[row][col] = false;
                    hasFieldPlanted[row][col] = 5;//修改关于土地的参数

                    shovelPushButton->setDisabled(false);//解放铲子按钮

                    this->setCursor(Qt::ArrowCursor);//把鼠标形状改回来

                    jalapenoPushButton->setDisabled(true);

                    jalapenoArray[row][col]->startAttacking();
                    QTimer* waitingtime=new QTimer(this);
                    waitingtime->setInterval(1900);
                    waitingtime->start();
                    for(int i = 0; i < normalZombieList.size(); i++){
                        if(jalapenoArray[row][col]->ReturnRow() == normalZombieList[i]->getZombieRow()){
                            normalZombieList[i]->isburn=true;
                        }
                    }
                    for(int i = 0; i < bucketZombieList.size(); i++){
                        if(jalapenoArray[row][col]->ReturnRow() == bucketZombieList[i]->getZombieRow()){
                            bucketZombieList[i]->isburn=true;
                        }
                    }
                    for(int i = 0; i < coneZombieList.size(); i++){
                        if(jalapenoArray[row][col]->ReturnRow() == coneZombieList[i]->getZombieRow()){
                            coneZombieList[i]->isburn=true;
                        }
                    }
                    connect(waitingtime, &QTimer::timeout, [=](){
                        delete jalapenoArray[row][col];
                        jalapenoArray[row][col]=nullptr;
                    });
                    isFieldPlantable[row][col] = true;
                    hasFieldPlanted[row][col] = 0;//修改关于土地的参数
                }
            }

            else if(isFieldPlantable[row][col] == false && hasFieldPlanted[row][col] > 0){
                if(isToShovelPlants == true && ableToShovelPlants == true){
                    //播放声音
                    mediaPlayer_shovel->play();
                    isFieldPlantable[row][col] = true;
                    hasFieldPlanted[row][col] = 0;//修改土地状态
                    if(normalPeaArray[row][col] != nullptr){
                        delete normalPeaArray[row][col];
                        normalPeaArray[row][col] = nullptr;//删除一株豌豆并让其指向空指针
                    }
                    if(icePeaArray[row][col] != nullptr){
                        delete icePeaArray[row][col];
                        icePeaArray[row][col] = nullptr;//删除一株向日葵并让其指向空指针
                    }
                    if(wallNutArray[row][col] != nullptr){
                        delete wallNutArray[row][col];
                        wallNutArray[row][col] = nullptr;//删除一颗坚果并让其指向空指针
                    }
                }
            }
        }

    }
    else
        return;
}


void GameWindow::slot_clickNormalPeaPushButton(){
    //播放点击的音效
    mediaPlayer_buttonClicked->play();

    if(isSettingNormalPea == false){
        isSettingNormalPea = true;
        ableToSetNormalPea = true;//修改种植状态

        shovelPushButton->setDisabled(true);//静默铲子按钮

        this->setCursor(QPixmap(":/images/Peashooter_0.png"));//改鼠标形状
    }
    else{
        isSettingNormalPea = false;
        ableToSetNormalPea = false;//修改种植状态

        shovelPushButton->setDisabled(false);//解放铲子按钮

        this->setCursor(Qt::ArrowCursor);//改鼠标形状
    }
}


//寒冰射手按钮触发
void GameWindow::slot_clickIcePeaPushButton(){
    //播放点击的音效
    mediaPlayer_buttonClicked->play();

    if(isSettingIcePea == false){
        isSettingIcePea = true;
        ableToSetIcePea = true;//修改种植状态

        shovelPushButton->setDisabled(true);//静默铲子按钮

        this->setCursor(QPixmap(":/images/SnowPea_0.png"));//改鼠标形状

    }
    else{
        isSettingIcePea = false;
        ableToSetIcePea = false;//修改种植状态

        shovelPushButton->setDisabled(false);//解放铲子按钮

        this->setCursor(Qt::ArrowCursor);//改鼠标形状
    }
}

//坚果墙按钮触发
void GameWindow::slot_clickWallNutPushButton(){
    //播放点击的音效
    mediaPlayer_buttonClicked->play();

    if(isSettingWallNut == false){
        isSettingWallNut = true;
        ableToSetWallNut = true;//修改种植状态

        shovelPushButton->setDisabled(true);//静默铲子按钮

        this->setCursor(QPixmap(":/images/WallNut0.png"));//改鼠标形状

    }
    else{
        isSettingWallNut = false;
        ableToSetWallNut = false;//修改种植状态

        shovelPushButton->setDisabled(false);//解放铲子按钮

        this->setCursor(Qt::ArrowCursor);//改鼠标形状
    }
}


void GameWindow::slot_showBackgroundImage(){
    if(numOfBImgRightMove<50){
        QImage* tempImage = new QImage(":/images/background1.png");
        *backGroundImg =  tempImage->copy(10*numOfBImgRightMove, 0, 900, 600) ;
        myBackGroundLabel->setPixmap(QPixmap::fromImage(*backGroundImg));
        numOfBImgRightMove++;
    }
    else if(numOfBImgLeftMove < 38){
        QImage *tempImage = new QImage(":/images/background1.png");
        *backGroundImg =  tempImage->copy(500-10*numOfBImgLeftMove, 0, 900, 600) ;
        myBackGroundLabel->setPixmap(QPixmap::fromImage(*backGroundImg));
        numOfBImgLeftMove++;
    }
    else
        return;
}

void GameWindow::scrollBackgroundImage(){
    QTimer* tempTimer = new QTimer(this);
    tempTimer->setInterval(40);
    tempTimer->start();
    connect(tempTimer, &QTimer::timeout, this, &GameWindow::slot_showBackgroundImage);
}

//点击铲子时触发的事件
void GameWindow::slot_clickShovelPushButton(){
    //播放点击的音效
    mediaPlayer_buttonClicked->play();

    if(isToShovelPlants == false){
        isToShovelPlants = true;
        ableToShovelPlants = true;//修改铲子状态
        this->setCursor(QPixmap(":/images/Shovel.png"));//改鼠标形状
    }
    else{
        isToShovelPlants = false;
        ableToShovelPlants = false;//修改铲子状态
        this->setCursor(Qt::ArrowCursor);//改鼠标形状
    }
}


//樱桃炸弹触发按钮
void GameWindow::slot_clickCherryBombPushButton(){
    //播放点击的音效
    mediaPlayer_buttonClicked->play();
    if(isSettingCherryBomb == false){
        isSettingCherryBomb = true;
        ableToSetCherryBomb = true;//修改种植状态
        shovelPushButton->setDisabled(true);//静默铲子按钮
        this->setCursor(QPixmap(":/images/CherryBomb_0.png"));//改鼠标形状
    }
    else{
        isSettingCherryBomb = false;
        ableToSetCherryBomb = false;//修改种植状态
        shovelPushButton->setDisabled(false);//解放铲子按钮
        this->setCursor(Qt::ArrowCursor);//改鼠标形状
    }
}

//火爆辣椒触发按钮
void GameWindow::slot_clickJalapenoPushButton(){
    //播放点击的音效
    mediaPlayer_buttonClicked->play();
    if(isSettingJalapeno == false){
        isSettingJalapeno = true;
        ableToSetJalapeno = true;//修改种植状态
        shovelPushButton->setDisabled(true);//静默铲子按钮
        this->setCursor(QPixmap(":/images/Jalapeno_0.png"));//改鼠标形状
    }
    else{
        isSettingJalapeno = false;
        ableToSetJalapeno = false;//修改种植状态
        shovelPushButton->setDisabled(false);//解放铲子按钮
        this->setCursor(Qt::ArrowCursor);//改鼠标形状
    }
}

void GameWindow::sendsignals(){
    emit mysignal();
}
