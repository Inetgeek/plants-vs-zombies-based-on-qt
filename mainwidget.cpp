#include "mainwidget.h"
#include "ui_mainwidget.h"
#include "gamewindow.h"
#include <QPushButton>
#include <QMediaPlayer>

mainWidget::mainWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::mainWidget) {
    ui->setupUi(this);
    this->setWindowIcon(QIcon(":/images/Icon.png"));
    this->setWindowTitle("Plants vs Zombies");
    this->resize(900, 600);

    mediaPlayer_backgroundMusic = new QMediaPlayer(this);
    mediaPlaylist_backgroundMusic = new QMediaPlaylist(this);
    mediaPlaylist_backgroundMusic->addMedia(QUrl("qrc:/music/Mountains.mp3"));
    mediaPlaylist_backgroundMusic->setCurrentIndex(1);
    mediaPlaylist_backgroundMusic->setPlaybackMode(QMediaPlaylist::CurrentItemInLoop);
    mediaPlayer_backgroundMusic->setPlaylist(mediaPlaylist_backgroundMusic);
    mediaPlayer_backgroundMusic->setVolume(150);
    mediaPlayer_backgroundMusic->play();

    QMediaPlayer *startSound = new QMediaPlayer(this);
    startSound->setMedia(QUrl("qrc:/music/evillaugh.mp3"));
    startSound->setVolume(150);

    QPalette pal = this->palette();
    pal.setBrush(QPalette::Background, QBrush(QPixmap(":/images/MainMenu.png")));
    setPalette(pal);

    QPushButton* startBtn = new QPushButton(this);
    startBtn->resize(330, 140);
    startBtn->move(475, 100);
    startBtn->setStyleSheet(
        "QPushButton{background: transparent;}"
        "QPushButton{border-image: url(:/images/Adventure_0.png) no-repeat;}"
        "QPushButton:hover{border-image: url(:/images/Adventure_1.png) no-repeat;}"
         "QPushButton:pressed{border-image: url(:/images/Adventure_2.png) no-repeat;}"
    );

    //从地里出来的手的动画设置
    movie_hand = new QMovie(":/images/start.gif");
    ui->label->setMovie(movie_hand);
    ui->label->setScaledContents(true);

    connect(startBtn, &QPushButton::released, [=](){
        movie_hand->start();
        startSound->play();

    });

    QObject::connect(movie_hand, SIGNAL(finished()), mediaPlayer_backgroundMusic, SLOT(stop()));

    QObject::connect(movie_hand, &QMovie::finished, [=](){
        gameWin = new GameWindow;
        this->hide();
        gameWin->show();
    });

}

mainWidget::~mainWidget(){
    delete ui;
}

void mainWidget::myslot(){
    //隐藏这个窗口
    hide();
    //显示另外的窗口
    gameWin->show();
}
void mainWidget::subslot(){
    gameWin->hide();
    show();
}

