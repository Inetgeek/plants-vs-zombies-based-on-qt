#ifndef CONEHEADZOMBIE_H
#define CONEHEADZOMBIE_H

#include "zombie.h"

/**
 * 路障僵尸类，继承于僵尸父类，主要区别于血量，血量更高
 */
class coneheadZombie : public Zombie{
public:
    coneheadZombie(QWidget* parent);
    void SetMovie();
};

#endif // CONEHEADZOMBIE_H
