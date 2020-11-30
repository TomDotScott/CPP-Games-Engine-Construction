#include "Entity.h"
#include "../Graphics/Graphics.h"

Entity::Entity(
	e_EntityType type, const int ID, const Vector2 size, const e_Direction direction, const Vector2 position,
	const Vector2 velocity, const Vector2 acceleration
) :
	m_entityID(ID),
	m_size(size),
	m_position(position),
	m_velocity(velocity),
	m_acceleration(acceleration),
	m_currentDirection(direction),
	m_currentEntityState(e_EntityState::eAlive),
	m_entityType(e_EntityType::eNone),
	m_currentCollisionBoxes()
{

}

CollisionBoxes Entity::GetCurrentCollisionBoxes() const
{
	return m_currentCollisionBoxes;
}

Vector2 Entity::GetPosition() const
{
	return m_position;
}

void Entity::SetPosition(const Vector2 newPos)
{
	m_position = newPos;
}

Vector2 Entity::GetVelocity() const
{
	return m_velocity;
}

void Entity::SetVelocity(const Vector2 newVel)
{
	m_velocity = newVel;
}

e_Direction Entity::GetCurrentDirection() const
{
	return m_currentDirection;
}

void Entity::SetDirection(const e_Direction direction)
{
	m_currentDirection = direction;
	if (direction == e_Direction::eRight)
	{
		m_velocity.x = abs(m_velocity.x);
	} else
	{
		m_velocity.x = -abs(m_velocity.x);
	}
}

e_EntityState Entity::GetCurrentEntityState() const
{
	return m_currentEntityState;
}
void Entity::SetEntityState(const e_EntityState state)
{
	m_currentEntityState = state;
}

e_EntityType Entity::GetEntityType() const
{
	return m_entityType;
}

int Entity::GetEntityID() const
{
	return m_entityID;
}

void Entity::AddAnimation(std::vector<std::string>& animationFrameIdentifiers, const bool looping, const float frameLength)
{
	auto anim = AnimationPlayer(animationFrameIdentifiers, looping, frameLength);
	m_animations.emplace_back(anim);
}

void Entity::SetAnimationIndex(const int animationIndex)
{
	m_animationIndex = animationIndex;
}

std::string Entity::GetCurrentAnimationFrameIdentifier()
{
	return m_animations[m_animationIndex].GetCurrentFrameIdentifier();
}

e_AnimationState Entity::GetCurrentAnimationState() const
{
	return m_animations[m_animationIndex].GetCurrentAnimationState();
}

void Entity::Render()
{
	Graphics::GetInstance().DrawSprite(
		GetCurrentAnimationFrameIdentifier(),
		{ static_cast<float>(constants::k_screenWidth) / 2.f, m_position.y
		}
	);
}

void Entity::Render(const float playerOffset)
{
	Graphics::GetInstance().DrawSprite(
		GetCurrentAnimationFrameIdentifier(),
		{
			m_position.x + (static_cast<float>(constants::k_screenWidth) / 2.f) - playerOffset,
			m_position.y
		}
	);
}