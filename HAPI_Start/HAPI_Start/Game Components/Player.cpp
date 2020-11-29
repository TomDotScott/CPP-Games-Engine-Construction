#include "Player.h"

#include "../Graphics/Graphics.h"

Player::Player(const Vector2 startingPosition) :
	Entity(e_EntityType::ePlayer,
		-1,
		Vector2(constants::k_spriteSheetCellWidth, constants::k_spriteSheetCellWidth + 13),
		e_Direction::eNone,
		startingPosition,
		{ Vector2::ZERO }),
	m_jumpForce(8.f),
	m_shouldJumpNextFrame(false),
	m_currentPlayerState(e_PlayerState::eJumping),
	m_moveDirectionLimit(e_Direction::eNone)
{

	m_entityType = e_EntityType::ePlayer;

	// Create the animations
	// Idle Animation
	std::vector<std::string> idle{ "Player_Idle_Body_1", "Player_Idle_Body_2" };
	AddAnimation(idle, true, 1000.f);

	// Walk Animation
	std::vector<std::string> walk{ "Player_Walk_Body_1", "Player_Walk_Body_2" };
	AddAnimation(walk);

	// Jump Animation
	std::vector<std::string> jump{ "Player_Jump_Body_1" };
	AddAnimation(jump);

	// Climb Animation
	std::vector<std::string> climb{ "Player_Climb_Body_1", "Player_Climb_Body_2" };
	AddAnimation(climb);
}

void Player::Update(const float deltaTime)
{
	Move(deltaTime);

	if (m_currentPlayerState == e_PlayerState::eWalking)
	{
		m_velocity.y = 0;
		if (m_shouldJumpNextFrame)
		{
			Jump(m_jumpForce);
			m_shouldJumpNextFrame = false;
		}
	} else if (m_currentPlayerState == e_PlayerState::eJumping)
	{
		m_velocity.y += constants::k_gravity * (deltaTime / 1000);
	}

	m_position = m_position + m_velocity;

	// Can't go off the top of the screen
	if (m_position.y < constants::k_spriteSheetCellWidth)
	{
		m_position.y = constants::k_spriteSheetCellWidth;
	}

	if (m_currentDirection == e_Direction::eNone && m_currentPlayerState != e_PlayerState::eJumping && !m_shouldJumpNextFrame)
	{
		m_currentPlayerState = e_PlayerState::eIdle;
	}

	m_animator.SetAnimationIndex(static_cast<int>(m_currentPlayerState));
	m_animator.Update(deltaTime);
	m_currentCollisionBoxes = GenerateCollisionBoxes();
}

void Player::Render()
{
	Graphics::GetInstance().DrawSprite(
		GetTopIdentifier(),
		{ static_cast<float>(constants::k_screenWidth) / 2.f, m_position.y - constants::k_spriteSheetCellWidth
		},
		true
	);

	Graphics::GetInstance().DrawSprite(
		m_animator.GetCurrentFrameIdentifier(),
		{ static_cast<float>(constants::k_screenWidth) / 2.f, m_position.y
		},
		true
	);
}

void Player::CheckEntityCollisions(Entity& other)
{
	const auto otherEntColBox = other.GetCurrentCollisionBoxes();
	// Check the global boxes
	if (m_currentCollisionBoxes.m_globalBounds.Overlapping(otherEntColBox.m_globalBounds))
	{
		// If touching the left or right...
		if (m_currentCollisionBoxes.m_leftCollisionBox.Overlapping(otherEntColBox.m_rightCollisionBox) ||
			m_currentCollisionBoxes.m_rightCollisionBox.Overlapping(otherEntColBox.m_leftCollisionBox))
		{
			// TODO: Kill the player

		}
		// If touching the bottom...
		if (GetCurrentCollisionBoxes().m_bottomCollisionBox.Overlapping(otherEntColBox.m_topCollisionBox) &&
			(other.GetEntityType() == e_EntityType::eSnail || other.GetEntityType() == e_EntityType::eSlime)
			)
		{
			// Jump
			Jump(m_jumpForce / 2);
		}
	}
}

e_PlayerState Player::GetCurrentPlayerState() const
{
	return m_currentPlayerState;
}

void Player::SetPlayerState(const e_PlayerState state)
{
	m_currentPlayerState = state;
}

void Player::SetShouldJump(const bool shouldJump)
{
	m_shouldJumpNextFrame = shouldJump;
}

e_Direction Player::GetMoveDirectionLimit() const
{
	return m_moveDirectionLimit;
}

void Player::SetMoveDirectionLimit(const e_Direction direction)
{
	m_moveDirectionLimit = direction;
}

void Player::Move(const float deltaTime)
{
	if (m_currentDirection == e_Direction::eRight && m_moveDirectionLimit != e_Direction::eRight)
	{
		m_position = m_position + (Vector2::RIGHT * deltaTime);
	} else if (m_currentDirection == e_Direction::eLeft && m_moveDirectionLimit != e_Direction::eLeft)
	{
		m_position = m_position + (Vector2::LEFT * deltaTime);
	}
}

void Player::Jump(const float jumpForce)
{
	m_currentPlayerState = e_PlayerState::eJumping;
	m_velocity.y = -jumpForce;
}

// The animations are just for the body but the character is made up of
// 2 sprites, with one for the head. In order to render the appropriate top,
// we need to edit the string identifier for the sprite to remove the "Bottom"
std::string Player::GetTopIdentifier()
{
	std::string currentFramePlayerTopIdentifier = m_animator.GetCurrentFrameIdentifier();

	// Store the frame no to add back to the end
	const auto frameNo = currentFramePlayerTopIdentifier[currentFramePlayerTopIdentifier.size() - 1];
	// Remove the "Body_x"
	currentFramePlayerTopIdentifier.erase(currentFramePlayerTopIdentifier.end() - 6, currentFramePlayerTopIdentifier.end());
	// Add "Top_x"
	currentFramePlayerTopIdentifier.append("Top_");
	return currentFramePlayerTopIdentifier += frameNo;
}

CollisionBoxes Player::GenerateCollisionBoxes()
{
	auto entityCollisionBox = BoundsRectangle({ 0, 0 }, m_size);
	entityCollisionBox.Translate(m_position);

	auto topBottomCollisionBox = BoundsRectangle({ 16, 0 }, { 46, 8 });
	topBottomCollisionBox.Translate(m_position);

	auto leftRightCollisionBox = BoundsRectangle({ 5, 8 }, { 31, 68 });
	leftRightCollisionBox.Translate(m_position);

	return{ entityCollisionBox,
		topBottomCollisionBox,
		leftRightCollisionBox,
		leftRightCollisionBox.Translate({26, 0}),
		topBottomCollisionBox.Translate({0, 68})
	};
}
