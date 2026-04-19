#include <stdlib.h>
#include "GameUtil.h"
#include "Asteroid.h"
#include "BoundingShape.h"

Asteroid::Asteroid(void) : GameObject("Asteroid")

{
	//changes the asteroids spawn point to be in the corners
	mAngle = rand() % 360;
	mRotation = 0; // rand() % 90;

	int spawn = rand() % 4;
	//left wall, right wall, bottom wall, top wall
	if (spawn == 1)
	{
		mPosition.x = -100.0;
		mPosition.y = rand() % 200 - 100;
	}
	else if (spawn == 2)
	{
		mPosition.x = 100.0;
		mPosition.y = rand() % 200 - 100;
	}
	else if (spawn == 3)
	{
		mPosition.x = rand() % 200 - 100;
		mPosition.y = -100.0;
	}
	else
	{
		mPosition.x = rand() % 200 - 100;
		mPosition.y = 100.0;
	}
	
	mVelocity.x = 10.0 * cos(DEG2RAD*mAngle);
	mVelocity.y = 10.0 * sin(DEG2RAD*mAngle);
	mVelocity.z = 0.0;
}

Asteroid::~Asteroid(void)
{
}

bool Asteroid::CollisionTest(shared_ptr<GameObject> o)
{
	if (mBoundingShape.get() == NULL) return false;
	if (o->GetBoundingShape().get() == NULL) return false;
	return mBoundingShape->CollisionTest(o->GetBoundingShape());
}
//bounce logic ffor the asteroids
void Asteroid::OnCollision(const GameObjectList& objects)
{
	for (auto i = objects.begin();  i != objects.end(); ++i)
	{
		shared_ptr<GameObject> other = *i;
		
		if (other->GetType() == GameObjectType("Asteroid"))
		{
			//gets  current position and velocity of both the colliding asteroids.
			GLVector3f position1 = GetPosition();
			GLVector3f position2 = other->GetPosition();
			GLVector3f velocity1 = GetVelocity();
			GLVector3f velocity2 = other->GetVelocity();
			
			float posix = position1.x - position2.x;
			float posiy = position1.y - position2.y;
			float dist = sqrt((posix * posix) + (posiy * posiy));
			if (dist == 0.0f) dist = 0.01f;
			
			float nx = posix/ dist;
			float ny = posiy / dist;
			float vn = (((velocity1.x - velocity2.x) * nx) + ((velocity1.y - velocity2.y) * ny));
			
			//makes sure the asteroids are actually moving towards each other.
			if (vn <0.0f)
			{
				velocity1.x -= vn * nx;
				velocity1.y -= vn * ny;
				velocity2.x += vn * nx;
				velocity2.y += vn * ny;
				
				//sets the velocity fo rboth asteroids to move them away.
				SetVelocity(velocity1);
				other->SetVelocity(velocity2);
			} 
			
		}
		else{
			mWorld->FlagForRemoval(GetThisPtr());
			
		}
	}
}
	

	

