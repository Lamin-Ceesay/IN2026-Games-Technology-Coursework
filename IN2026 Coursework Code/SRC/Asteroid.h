#ifndef __ASTEROID_H__
#define __ASTEROID_H__

#include "GameObject.h"
#include "GameUtil.h"

class Asteroid : public GameObject
{
public:
	Asteroid(void);
	~Asteroid(void);
	
	enum class Asteroid_Size {big, small};
	
	void setAsteroidSize(Asteroid_Size size){asteroid_Size = size;}
	Asteroid_Size getAsteroidSize(){return asteroid_Size;}

	bool CollisionTest(shared_ptr<GameObject> o) override;
	void OnCollision(const GameObjectList& objects) override;
	
	void setHitByBullet(bool hit) { mHitByBullet = hit; }
	bool getHitByBullet() { return mHitByBullet; }
	
	private:
	Asteroid_Size asteroid_Size;
	bool mHitByBullet = false;
};

#endif
