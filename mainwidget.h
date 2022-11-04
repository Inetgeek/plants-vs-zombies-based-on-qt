#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include "gamewindow.h"

QT_BEGIN_NAMESPACE
namespace Ui {
    class mainWidget;
}
QT_END_NAMESPACE

class mainWidget : public QWidget{
    Q_OBJECT

public:
    mainWidget(QWidget* parent = nullptr);
    ~mainWidget();

private:
    Ui::mainWidget *ui;
    GameWindow* gameWin;
    QMovie* movie_hand;
    QMediaPlayer* mediaPlayer_backgroundMusic;//media控制
    QMediaPlaylist* mediaPlaylist_backgroundMusic;//连续播放的list
public slots:
    void myslot();
    void subslot();
};
#endif // MAINWIDGET_H
