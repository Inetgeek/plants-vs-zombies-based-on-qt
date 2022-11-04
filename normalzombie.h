/**
 * 普通僵尸，继承于 僵尸父类，和僵尸父类基本一致
 */

#ifndef NORMALZOMBIE_H
#define NORMALZOMBIE_H

#include "zombie.h"
#include "config.h"

class normalZombie : public Zombie{
public:
    normalZombie(QWidget* parent);
    void SetMovie();
};

#endif // NORMALZOMBIE_H
