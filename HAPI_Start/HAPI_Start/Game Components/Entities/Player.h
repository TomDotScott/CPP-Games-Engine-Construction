#pragma once
#pragma once
#include "Entity.h"
#include "Fireball.h"

enum class ePlayerState
{
	e_Idle, e_Walking, e_Jumping, e_Climbing
};

enum class ePowerUpType
{
	e_FireThrower
};

enum class ePowerUpState
{
	e_Normal, e_FireThrower
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

	ePowerUpState GetPowerUpState() const;
	void PowerUp(ePowerUpType pType);
	void PowerDown();

	void AddToScore(int points);
	int GetScore() const;
	
	void AddCoin();
	int GetCoinCount() const;
	
	int GetLivesRemaining() const;

	bool GetIsDead() const;
	void Kill();
	
	void Reset();
	
	std::vector<Fireball>& GetFireBallPool();

private:
	float m_jumpForce;
	bool m_shouldJumpNextFrame;
	
	bool m_canShoot;
	float m_shotCoolDown;
	
	ePlayerState m_currentPlayerState;
	ePowerUpState m_currentPowerUpState;
	
	eDirection m_moveDirectionLimit;

	int m_score;
	int m_coinCount;
	int m_livesRemaining;

	float m_immuneTime;
	bool m_immune;
	bool m_isDead;
	
	std::vector<Fireball> m_fireballPool;
	
	void Move(float deltaTime) override;
	void Jump(float jumpForce);
	void Shoot();
	std::string GetTopIdentifier();
	CollisionBoxes GenerateCollisionBoxes() override;
};
