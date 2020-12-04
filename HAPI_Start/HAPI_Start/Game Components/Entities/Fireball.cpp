#include "Fireball.h"
Fireball::Fireball(const int entityID) :
	Entity(
		eEntityType::e_Fireball,
		entityID,
		Vector2(constants::k_spriteSheetCellWidth, constants::k_spriteSheetCellWidth)
	),
	m_activeState(false),
	m_jumpForce(1.5f)
{
	AddAnimation(animations::FIREBALL_SPIN, true, 50);
	AddAnimation(animations::FIREBALL_EXPLOSION);
}

void Fireball::Initialise(const Vector2 startPosition, const eDirection startingDirection)
{
	m_position = startPosition;
	m_velocity = (startingDirection == eDirection::e_Right ? Vector2::LEFT : Vector2::RIGHT) + Vector2::DOWN;
	m_velocity.x *= 0.75f;
	m_activeState = true;
}

void Fireball::Bounce()
{
	m_velocity.y = -m_jumpForce;
}

bool Fireball::GetActiveState() const
{
	return m_activeState;
}

void Fireball::Update(const float deltaTime)
{
	Move(deltaTime);
	PlayAnimation(deltaTime);
	m_currentCollisionBoxes = GenerateCollisionBoxes();
}

void Fireball::CheckEntityCollisions(Entity& other)
{

}

CollisionBoxes Fireball::GenerateCollisionBoxes()
{
	auto entityCollisionBox = BoundsRectangle({ 0, 0 }, m_size);
	entityCollisionBox.Translate(m_position);

	auto topBottomCollisionBox = BoundsRectangle({ 21, 20 }, { 43, 24 });
	topBottomCollisionBox.Translate(m_position);

	auto leftRightCollisionBox = BoundsRectangle({ 21, 24 }, { 32, 40 });
	leftRightCollisionBox.Translate(m_position);

	return{ entityCollisionBox,
		topBottomCollisionBox,
		leftRightCollisionBox,
		leftRightCollisionBox.Translate({21, 0}),
		topBottomCollisionBox.Translate({0, 20})
	};
}

void Fireball::Move(const float deltaTime)
{
	m_velocity.y += constants::k_gravity * (deltaTime / 1000.f);
	m_position = m_position + m_velocity * deltaTime;
}
