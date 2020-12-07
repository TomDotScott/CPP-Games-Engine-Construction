#pragma once
#pragma once
#include "Entity.h"
#include "Fireball.h"

enum class ePlayerState
{
	e_Idle, e_Walking, e_Jumping, e_Climbing
};

class Player final : public Entity
{
public:
	explicit Player(Vector2 startingPosition);

	void Update(float deltaTime) override;
	void Render() override;
	void CheckEntityCollisions(Entity& other) override;
	void SetShouldJump(bool shouldJump);

	void SetCanShoot(bool canShoot);

	eDirection GetMoveDirectionLimit() const;
	void SetMoveDirectionLimit(eDirection direction);

	ePlayerState GetCurrentPlayerState() const;
	void SetPlayerState(ePlayerState state);

	std::vector<Fireball>& GetFireBallPool();

private:
	float m_jumpForce;
	bool m_shouldJumpNextFrame;
	bool m_canShoot;
	float m_shotCoolDown;
	ePlayerState m_currentPlayerState;
	eDirection m_moveDirectionLimit;

	std::vector<Fireball> m_fireballPool;
	
	void Move(float deltaTime) override;
	void Jump(float jumpForce);
	void Shoot();
	std::string GetTopIdentifier();
	CollisionBoxes GenerateCollisionBoxes() override;
};
