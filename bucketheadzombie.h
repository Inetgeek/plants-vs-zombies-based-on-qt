#ifndef BUCKETHEADZOMBIE_H
#define BUCKETHEADZOMBIE_H

#include "zombie.h"


/**
 * 铁通僵尸类，继承于 僵尸父类， 区别主要在于 血量不同，血量最高
 */
class bucketheadZombie : public Zombie{
public:
    bucketheadZombie(QWidget* parent);
    void SetMovie();
};

#endif // BUCKETHEADZOMBIE_H
