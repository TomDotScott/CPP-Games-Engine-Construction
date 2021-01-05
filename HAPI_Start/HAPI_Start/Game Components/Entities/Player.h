#pragma once
#pragma once
#include "Alien.h"
#include "Fireball.h"

enum class ePowerUpType;

/**
 * \brief The powerupstate determines the size
 * and powers of the player
 */
enum class ePowerUpState
{
	e_Small,
	e_Normal,
	e_FireThrower
};

/**
 * \brief The player is the alien entity that is able to be
 * controlled by the user of the demo. It collects coins
 * and, when it touches a powerup, gains different abilities
 */
class Player final : public Alien
{
public:
	explicit Player(const Vector2& startingPosition);

	void Update(float deltaTime) override;
	void Render(TextureManager& textureManager) override;
	void PlayAnimation(float deltaTime) override;
	void CheckEntityCollisions(Entity& other) override;

	/**
	 * \brief Applies a given powerup to the player
	 * \param pType the type of powerup to apply
	 */
	void PowerUp(ePowerUpType pType);
	/**
	 * \brief Powers down the player. If the player is small
	 * and this function gets called, the player dies
	 */
	void PowerDown();
	/**
	 * \brief Adds a given amount of points to the player's score
	 * \param points The amount of points to add
	 */
	void AddToScore(int points);
	/**
	 * \brief Adds a coin to the player's coin total
	 */
	void AddCoin();
	/**
	 * \brief Kills the player, setting its state to eDead
	 * and subtracting a life from the player
	 */
	void Kill();
	/**
	 * \brief Resets the player
	 * \param passLevel True if the player shouldn't reset his powerup state
	 * \param startPosition The position that the player should be reset to
	 */
	void Reset(bool passLevel = false, const Vector2& startPosition = Vector2::CENTRE);

	ePowerUpState GetPowerUpState() const;
	int           GetScore() const;
	int           GetCoinCount() const;
	bool          GetIsDead() const;
	eDirection    GetMoveDirectionLimit() const;
	int           GetLivesRemaining() const;

	/**
	 * \brief Sets whether the player should shoot a fireball in the current frame
	 * \param canShoot True if the player should shoot
	 */
	void SetCanShoot(bool canShoot);
	/**
	 * \brief Sets whether the player should jump in the current frame
	 * \param shouldJump True if the player should jump
	 */
	void SetShouldJump(bool shouldJump);
	/**
	 * \brief Sets the directional limit of the player 
	 * \param direction The direction that should be limited
	 */
	void SetMoveDirectionLimit(eDirection direction);

private:
	int           m_numStates;
	bool          m_shouldJumpNextFrame;
	ePowerUpState m_currentPowerUpState;
	eDirection    m_moveDirectionLimit;
	// The current game score
	int           m_score;
	// The player's coin counter
	int           m_coinCount;
	int           m_livesRemaining;
	// When the player gets hit by an enemy, he is granted temporary immunity
	float         m_immuneTime;
	// Whether the player is immune or not
	bool          m_immune;
	bool          m_isDead;

	void           Move(float deltaTime) override;
	CollisionBoxes GenerateCollisionBoxes() override;
};
