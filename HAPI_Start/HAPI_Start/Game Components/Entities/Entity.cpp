#include "Entity.h"

#include "../../Audio/SoundManager.h"
#include "../Graphics/TextureManager.h"

Entity::Entity(
	eEntityType type, const int ID, const Vector2 size, const eDirection direction, const Vector2 position,
	const Vector2 velocity, const Vector2 acceleration
) :
	m_entityID(ID),
	m_size(size),
	m_position(position),
	m_velocity(velocity),
	m_acceleration(acceleration),
	m_currentDirection(direction),
	m_currentEntityState(eEntityState::e_Alive),
	m_entityType(type)
{

}

CollisionBoxes Entity::GetCurrentCollisionBoxes()
{
	return GenerateCollisionBoxes();
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

eDirection Entity::GetCurrentDirection() const
{
	return m_currentDirection;
}

void Entity::SetDirection(const eDirection direction)
{
	m_currentDirection = direction;
	if (direction == eDirection::e_Right)
	{
		m_velocity.x = abs(m_velocity.x);
	} else
	{
		m_velocity.x = -abs(m_velocity.x);
	}
}

eEntityState Entity::GetCurrentEntityState() const
{
	return m_currentEntityState;
}
void Entity::SetEntityState(const eEntityState state)
{
	m_currentEntityState = state;
}

eEntityType Entity::GetEntityType() const
{
	return m_entityType;
}

int Entity::GetEntityID() const
{
	return m_entityID;
}

void Entity::AddAnimation(const std::vector<std::string>& animationFrameIdentifiers, const bool looping, const float frameLength)
{
	auto anim = AnimationPlayer(animationFrameIdentifiers, looping, frameLength);
	m_animations.emplace_back(anim);
}

void Entity::PlayAnimation(const float deltaTime)
{
	m_animations[m_animationIndex].Play(deltaTime);
}

void Entity::PlaySFX(const std::string& sfxName)
{
	SoundManager::GetInstance().PlaySoundEffect(sfxName);
}

void Entity::SetAnimationIndex(const int animationIndex)
{
	m_animationIndex = animationIndex;
}

std::string Entity::GetCurrentAnimationFrameIdentifier()
{
	return m_animations[m_animationIndex].GetCurrentFrameIdentifier();
}

eAnimationState Entity::GetCurrentAnimationState() const
{
	return m_animations[m_animationIndex].GetCurrentAnimationState();
}

void Entity::Render()
{
	TextureManager::GetInstance().DrawSprite(
		GetCurrentAnimationFrameIdentifier(),
		{ static_cast<float>(constants::k_screenWidth) / 2.f, m_position.y
		}
	);
}

void Entity::Render(const float playerOffset)
{
	TextureManager::GetInstance().DrawSprite(
		GetCurrentAnimationFrameIdentifier(),
		{
			m_position.x + (static_cast<float>(constants::k_screenWidth) / 2.f) - playerOffset,
			m_position.y
		}
	);
}