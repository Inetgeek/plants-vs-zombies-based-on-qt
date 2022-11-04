#include "plant.h"

Plant::Plant(QWidget* parent):QLabel(parent){
    mediaPlayer_plant = new QMediaPlayer(this);
    /*加入音乐路径*/
    mediaPlayer_plant->setMedia(QUrl(""));
    mediaPlayer_plant->setVolume(150);
    mediaPlayer_plant->play();
}
