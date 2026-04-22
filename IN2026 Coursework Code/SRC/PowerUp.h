#pragma once


#ifndef __POWERUP_H__
#define __POWERUP_H__
#include "GameObject.h"


namespace PowerUpType{
    enum Value{ ExtraLife, Upgrade, Invincible};
}


class PowerUp : public GameObject{
    
public:
    PowerUp(PowerUpType::Value type) : GameObject("PowerUp"), mPowerUpType(type)
    {
        mScale = 0.5f;
    }
    ~PowerUp() {}
    
    PowerUpType::Value getPowerUpType() { return mPowerUpType; }
    void Update(int t) override { GameObject::Update(t); }

    
private:
    PowerUpType::Value mPowerUpType;
};
    
#endif

    
