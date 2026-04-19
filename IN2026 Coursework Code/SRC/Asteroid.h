#ifndef __ASTEROID_H__
#define __ASTEROID_H__

#include "GameObject.h"

class Asteroid : public GameObject
{
public:
	Asteroid(void);
	~Asteroid(void);
	
	enum class Asteroid_Size {big, small};
	
	void setAsteroidSize(Asteroid_Size size){asteroid_Size = size;}
	Asteroid_Size getAsteroidSize() const {return asteroid_Size;}

	bool CollisionTest(shared_ptr<GameObject> o);
	void OnCollision(const GameObjectList& objects);
	
	private:
	Asteroid_Size asteroid_Size;
};

#endif
