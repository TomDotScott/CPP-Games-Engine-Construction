#include "Alien.h"
#include "../Graphics/TextureManager.h"

Alien::Alien(const int         entityID,
             const eEntityType type,
             const Vector2&    startingPosition,
             const Vector2     size,
             const eDirection  startDir) :
	Entity(type,
	       entityID,
	       size,
	       startDir,
	       startingPosition
	      ),
	m_movementSpeed(10.f),
	m_jumpForce(9.f),
	m_canShoot(false),
	m_shotCoolDown(constants::k_fireBallCoolDownTimer),
	m_currentAlienState(eAlienState::e_Jumping) {}

eAlienState Alien::GetCurrentAlienState() const
{
	return m_currentAlienState;
}

void Alien::SetAlienState(const eAlienState state)
{
	m_currentAlienState = state;
}

void Alien::Render(TextureManager& textureManager)
{
	textureManager.DrawSprite(
	                          GetTopIdentifier(),
	                          {
		                          static_cast<float>(constants::k_screenWidth) / 2.f,
		                          m_position.y - constants::k_spriteSheetCellSize
	                          },
	                          m_currentDirection == eDirection::e_Left
	                         );

	textureManager.DrawSprite(
	                          GetCurrentAnimationFrameIdentifier(),
	                          {static_cast<float>(constants::k_screenWidth) / 2.f, m_position.y},
	                          m_currentDirection == eDirection::e_Left
	                         );
}

void Alien::Render(TextureManager& textureManager, const float playerOffset)
{
	textureManager.DrawSprite(
	                          GetTopIdentifier(),
	                          {
		                          m_position.x + (static_cast<float>(constants::k_screenWidth) / 2.f) - playerOffset,
		                          m_position.y - constants::k_spriteSheetCellSize
	                          },
	                          m_currentDirection == eDirection::e_Left
	                         );

	textureManager.DrawSprite(
	                          GetCurrentAnimationFrameIdentifier(),
	                          {
		                          m_position.x + (static_cast<float>(constants::k_screenWidth) / 2.f) - playerOffset,
		                          m_position.y
	                          },
	                          m_currentDirection == eDirection::e_Left
	                         );
}

std::vector<Fireball>& Alien::GetFireBallPool()
{
	return m_fireballPool;
}

void Alien::Jump(const float jumpForce)
{
	m_currentAlienState = eAlienState::e_Jumping;
	m_velocity.y        = -jumpForce;
}

void Alien::Shoot()
{
	if (m_shotCoolDown >= constants::k_fireBallCoolDownTimer)
	{
		// Find the first inactive fireball and initialise it 
		for (auto& ball : m_fireballPool)
		{
			if (ball.GetActiveState() == false)
			{
				ball.Initialise(m_position, m_currentDirection);
				m_canShoot     = false;
				m_shotCoolDown = 0.f;
				PlaySFX("Fireball_Shoot");
				return;
			}
		}
	}
}

// The animations are just for the body but the character is made up of
// 2 sprites, with one for the head. In order to render the appropriate top,
// we need to edit the string identifier for the sprite to remove the "Bottom"
std::string Alien::GetTopIdentifier()
{
	std::string currentFramePlayerTopIdentifier = GetCurrentAnimationFrameIdentifier();

	// Store the frame no to add back to the end
	const auto frameNo = currentFramePlayerTopIdentifier[currentFramePlayerTopIdentifier.size() - 1];
	// Remove the "Body_x"
	currentFramePlayerTopIdentifier.erase(currentFramePlayerTopIdentifier.end() - 6,
	                                      currentFramePlayerTopIdentifier.end());
	// Add "Top_x"
	currentFramePlayerTopIdentifier.append("Top_");
	return currentFramePlayerTopIdentifier += frameNo;
}
