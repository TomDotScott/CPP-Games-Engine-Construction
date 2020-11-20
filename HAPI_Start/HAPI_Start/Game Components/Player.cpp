#include "Player.h"

#include "../Graphics/Graphics.h"

Player::Player(const Vector2 startingPosition) :
	Entity(Vector2(constants::k_spriteSheetCellWidth, constants::k_spriteSheetCellWidth + 12),
		startingPosition,
		{ Vector2::ZERO }),
	CURRENT_STATE(EPlayerState::eJumping),
	m_currentDirection(Vector2::ZERO),
	m_jumpForce(8.f),
	m_shouldJumpNextFrame(false) {

	// Create the animations
	// Idle Animation
	std::vector<std::string> idle{ "Player_Idle_Body_1", "Player_Idle_Body_2" };
	Animation idleAnim(idle, 1000.f);
	m_animator.AddAnimation(idleAnim);

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

void Player::Update(const float deltaTime) {
	Move(deltaTime);

	if (CURRENT_STATE == EPlayerState::eWalking) {
		m_velocity.y = 0;
		if (m_shouldJumpNextFrame) {
			Jump();
			m_shouldJumpNextFrame = false;
		}
	} else if (CURRENT_STATE == EPlayerState::eJumping) {
		m_velocity.y += constants::k_gravity * (deltaTime / 1000);
	}

	m_position = m_position + m_velocity;

	// Can't go off the top of the screen
	if (m_position.y < constants::k_spriteSheetCellWidth) {
		m_position.y = constants::k_spriteSheetCellWidth;
	}

	m_animator.SetAnimationIndex(static_cast<int>(CURRENT_STATE));
	m_animator.Update(deltaTime);
}

void Player::Render() {
	Graphics::GetInstance().DrawSprite(
		GetTopIdentifier(),
		{ static_cast<float>(constants::k_screenWidth) / 2.f, m_position.y - constants::k_spriteSheetCellWidth
		}
	);

	Graphics::GetInstance().DrawSprite(
		m_animator.GetCurrentFrameIdentifier(),
		{ static_cast<float>(constants::k_screenWidth) / 2.f, m_position.y
		}
	);
}

void Player::SetDirection(const Vector2 direction) {
	m_currentDirection = direction;
	if (m_currentDirection == Vector2::ZERO && CURRENT_STATE != EPlayerState::eJumping && !m_shouldJumpNextFrame) {
		CURRENT_STATE = EPlayerState::eIdle;
	}
}

void Player::SetShouldJump(const bool shouldJump) {
	m_shouldJumpNextFrame = shouldJump;
}

Vector2 Player::GetCurrentDirection() const {
	return m_currentDirection;
}

void Player::Move(const float deltaTime) {
	m_position = m_position + (m_currentDirection * deltaTime);
}

void Player::Jump() {
	CURRENT_STATE = EPlayerState::eJumping;
	m_velocity.y = -m_jumpForce;
}

// The animations are just for the body but the character is made up of
// 2 sprites, with one for the head. In order to render the appropriate top,
// we need to edit the string identifier for the sprite to remove the "Bottom"
std::string Player::GetTopIdentifier() {
	std::string currentFramePlayerTopIdentifier = m_animator.GetCurrentFrameIdentifier();

	// Store the frame no to add back to the end
	const auto frameNo = currentFramePlayerTopIdentifier[currentFramePlayerTopIdentifier.size() - 1];
	// Remove the "Body_x"
	currentFramePlayerTopIdentifier.erase(currentFramePlayerTopIdentifier.end() - 6, currentFramePlayerTopIdentifier.end());
	// Add "Top_x"
	currentFramePlayerTopIdentifier.append("Top_");
	return currentFramePlayerTopIdentifier += frameNo;
}
