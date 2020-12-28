#pragma once
#include "Entity.h"
#include "Fireball.h"

enum class eAlienState
{
	e_Idle, e_Walking, e_Jumping, e_Climbing, e_Dead
};

class Alien : public Entity
{
public:
	Alien(const int entityID, eEntityType type, Vector2 startingPosition, Vector2 size, eDirection startDir);
	
	eAlienState GetCurrentAlienState() const;
	void SetAlienState(eAlienState state);

	virtual void Update(float deltaTime) override = 0;
	virtual void CheckEntityCollisions(Entity& other) override = 0;

	virtual void Render(TextureManager& textureManager) override;
	void Render(TextureManager& textureManager, float playerOffset) override;

	std::vector<Fireball>& GetFireBallPool();

protected:
	
	float m_movementSpeed;
	float m_jumpForce;
	
	bool m_canShoot;
	float m_shotCoolDown;

	eAlienState m_currentAlienState;

	std::vector<Fireball> m_fireballPool;

	virtual void Move(float deltaTime) override = 0;
	virtual void Jump(float jumpForce);
	virtual void Shoot();
	
	std::string GetTopIdentifier();
	
	virtual CollisionBoxes GenerateCollisionBoxes() override = 0;
};

