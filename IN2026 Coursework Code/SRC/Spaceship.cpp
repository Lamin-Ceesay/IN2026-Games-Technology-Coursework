#include "GameUtil.h"
#include "GameWorld.h"
#include "Bullet.h"
#include "Spaceship.h"

#include "Asteroid.h"
#include "BoundingSphere.h"
#include "PowerUp.h"

using namespace std;

// PUBLIC INSTANCE CONSTRUCTORS ///////////////////////////////////////////////

/**  Default constructor. */
Spaceship::Spaceship()
	: GameObject("Spaceship"), mThrust(0)
{
}

/** Construct a spaceship with given position, velocity, acceleration, angle, and rotation. */
Spaceship::Spaceship(GLVector3f p, GLVector3f v, GLVector3f a, GLfloat h, GLfloat r)
	: GameObject("Spaceship", p, v, a, h, r), mThrust(0)
{
}

/** Copy constructor. */
Spaceship::Spaceship(const Spaceship& s)
	: GameObject(s), mThrust(0)
{
}

/** Destructor. */
Spaceship::~Spaceship(void)
{
}

// PUBLIC INSTANCE METHODS ////////////////////////////////////////////////////

/** Update this spaceship. */
void Spaceship::Update(int t)
{
	if (mInvincible) {
		mInvinicibilityTimer += t;
		if (mInvinicibilityTimer >= 2000) {
			mInvincible = false;
			mInvinicibilityTimer = 0;
		}
	}
	
	if (mBetterBrakes && mThrust <= 0) {
		mVelocity *= 0.98;
	}
	// Call parent update function
	GameObject::Update(t);
}

/** Render this spaceship. */
void Spaceship::Render(void)
{
	if (mSpaceshipShape.get() != NULL) mSpaceshipShape->Render();

	// If ship is thrusting
	if ((mThrust > 0) && (mThrusterShape.get() != NULL)) {
		mThrusterShape->Render();
	}
	if (mInvincible && (rand() % 5 == 0)) return;

	GameObject::Render();
}

/** Fire the rockets. */
void Spaceship::Thrust(float t)
{
	mThrust = t;
	// Increase acceleration in the direction of ship
	mAcceleration.x = mThrust*cos(DEG2RAD*mAngle);
	mAcceleration.y = mThrust*sin(DEG2RAD*mAngle);
}

/** Set the rotation. */
void Spaceship::Rotate(float r)
{
	if (mEasierTurning && r != 0) {
		mRotation = r * 2.0f;}
	else{
		mRotation = r;
	}
}

/** Shoot a bullet. */
void Spaceship::Shoot(void)
{
	// Check the world exists
	if (!mWorld) return;
	// Construct a unit length vector in the direction the spaceship is headed
	GLVector3f spaceship_heading(cos(DEG2RAD*mAngle), sin(DEG2RAD*mAngle), 0);
	spaceship_heading.normalize();
	// Calculate the point at the node of the spaceship from position and heading
	GLVector3f bullet_position = mPosition + (spaceship_heading * 4);
	// Calculate how fast the bullet should travel
	float bullet_speed = 30;
	// Construct a vector for the bullet's velocity
	GLVector3f bullet_velocity = mVelocity + spaceship_heading * bullet_speed;
	// Construct a new bullet
	shared_ptr<GameObject> bullet
		(new Bullet(bullet_position, bullet_velocity, mAcceleration, mAngle, 0, 2000));
	bullet->SetBoundingShape(make_shared<BoundingSphere>(bullet->GetThisPtr(), 2.0f));
	bullet->SetShape(mBulletShape);
	// Add the new bullet to the game world
	mWorld->AddObject(bullet);

}

bool Spaceship::CollisionTest(shared_ptr<GameObject> o)
{
	if (o->GetType() == GameObjectType("Asteroid") || o->GetType() == GameObjectType("PowerUp"))
	{
		if (mBoundingShape.get() == NULL || o->GetBoundingShape().get() == NULL) return false;
		return mBoundingShape->CollisionTest(o->GetBoundingShape());
	}
	return false;
}

void Spaceship::OnCollision(const GameObjectList &objects)
{

	
	
	// If the ship is invincible, ignore collisions
	if (mInvincible) return;
	for (auto i = objects.begin(); i != objects.end(); i++)
	{
		shared_ptr<GameObject> obj = *i;

		if (obj->GetType() == GameObjectType("PowerUp"))
		{
			shared_ptr<PowerUp> powerup = static_pointer_cast<PowerUp>(obj);
			if (powerup->getPowerUpType() == PowerUpType::ExtraLife)
			{
				mWorld->FlagForRemoval(obj);
			}
			else if (powerup->getPowerUpType() == PowerUpType::Upgrade)
			{
				EnableBetterBrakes(true);
				EnableEasierTurning(true);
				mWorld->FlagForRemoval(obj);
			}
			else if (powerup->getPowerUpType() == PowerUpType::Invincible)
			{
				SetInvincibility(true);
				mWorld->FlagForRemoval(obj);
				
			}
			continue;
		
		}
		
		if (mInvincible) continue;
		
		if (obj->GetType()  == GameObjectType("Asteroid"))
		{
			shared_ptr<Asteroid> asteroid = static_pointer_cast<Asteroid>(obj);
			if (asteroid->getAsteroidSize() == Asteroid::Asteroid_Size::big)
			{
				if (!IsInvincible())
				mWorld->FlagForRemoval(GetThisPtr());
				break;
			}
		}
	}
}