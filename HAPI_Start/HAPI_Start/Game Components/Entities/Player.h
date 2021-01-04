#pragma once
#pragma once
#include "Alien.h"
#include "Fireball.h"

enum class ePowerUpType;

enum class ePowerUpState
{
	e_Small,
	e_Normal,
	e_FireThrower
};

class Player final : public Alien
{
public:
	explicit Player(const Vector2& startingPosition);

	void Update(float deltaTime) override;
	void Render(TextureManager& textureManager) override;
	void PlayAnimation(float deltaTime) override;
	void CheckEntityCollisions(Entity& other) override;

	void PowerUp(ePowerUpType pType);
	void PowerDown();
	void AddToScore(int points);
	void AddCoin();
	void Kill();
	void Reset(bool passLevel = false, const Vector2& startPosition = Vector2::CENTRE);

	ePowerUpState GetPowerUpState() const;
	int           GetScore() const;
	int           GetCoinCount() const;
	bool          GetIsDead() const;
	eDirection    GetMoveDirectionLimit() const;
	int           GetLivesRemaining() const;

	void SetCanShoot(bool canShoot);
	void SetShouldJump(bool shouldJump);
	void SetMoveDirectionLimit(eDirection direction);

private:
	int           m_numStates;
	bool          m_shouldJumpNextFrame;
	ePowerUpState m_currentPowerUpState;
	eDirection    m_moveDirectionLimit;
	int           m_score;
	int           m_coinCount;
	int           m_livesRemaining;
	float         m_immuneTime;
	bool          m_immune;
	bool          m_isDead;

	void           Move(float deltaTime) override;
	CollisionBoxes GenerateCollisionBoxes() override;
};
