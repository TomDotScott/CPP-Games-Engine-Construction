#pragma once
#pragma once
#include "Alien.h"
#include "Fireball.h"

enum class ePowerUpType;

enum class ePowerUpState
{
	e_Small, e_Normal, e_FireThrower
};

class Player final : public Alien
{
public:
	explicit Player(Vector2 startingPosition);

	void Update(float deltaTime) override;
	void Render(TextureManager& textureManager) override;
	
	ePowerUpState GetPowerUpState() const;
	void PowerUp(ePowerUpType pType);
	void PowerDown();

	void AddToScore(int points);
	int GetScore() const;

	void AddCoin();
	int GetCoinCount() const;

	int GetLivesRemaining() const;

	void SetCanShoot(bool canShoot);
	
	bool GetIsDead() const;
	void Kill();

	void Reset(bool passLevel = false);

	void PlayAnimation(float deltaTime) override;
	void SetShouldJump(bool shouldJump);

	eDirection GetMoveDirectionLimit() const;
	void SetMoveDirectionLimit(eDirection direction);

	CollisionBoxes GenerateCollisionBoxes() override;

	void CheckEntityCollisions(Entity& other) override;
	
private:
	int m_numStates;
	bool m_shouldJumpNextFrame;

	ePowerUpState m_currentPowerUpState;

	eDirection m_moveDirectionLimit;

	int m_score;
	int m_coinCount;
	int m_livesRemaining;

	float m_immuneTime;
	bool m_immune;
	bool m_isDead;

	void Move(float deltaTime) override;
};
