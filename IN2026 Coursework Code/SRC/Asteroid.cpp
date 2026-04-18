#include <stdlib.h>
#include "GameUtil.h"
#include "Asteroid.h"
#include "BoundingShape.h"

Asteroid::Asteroid(void) : GameObject("Asteroid")
{
	mAngle = rand() % 360;
	mRotation = 0; // rand() % 90;
	mPosition.x = rand() / 2;
	mPosition.y = rand() / 2;
	mPosition.z = 0.0;
	mVelocity.x = 10.0 * cos(DEG2RAD*mAngle);
	mVelocity.y = 10.0 * sin(DEG2RAD*mAngle);
	mVelocity.z = 0.0;
}

Asteroid::~Asteroid(void)
{
}

bool Asteroid::CollisionTest(shared_ptr<GameObject> o)
{
	//if (GetType() == o->GetType()) return false;
	if (mBoundingShape.get() == NULL) return false;
	if (o->GetBoundingShape().get() == NULL) return false;
	return mBoundingShape->CollisionTest(o->GetBoundingShape());
}

void Asteroid::OnCollision(const GameObjectList& objects)
{
	for (auto i = objects.begin();  i != objects.end(); ++i)
	{
		shared_ptr<GameObject> other = *i;
		
		if (other->GetType() == GameObjectType("Asteroid"))
		{
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
			
			if (vn <0.0f)
			{
				velocity1.x -= vn * nx;
				velocity1.y -= vn * ny;
				velocity2.x += vn * nx;
				velocity2.y += vn * ny;
				
				
				SetVelocity(velocity1);
				other->SetVelocity(velocity2);
			} 
			
		}
		else{
			mWorld->FlagForRemoval(GetThisPtr());
			
		}
	}
}
	

	

