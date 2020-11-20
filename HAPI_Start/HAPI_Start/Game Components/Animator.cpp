#include "Animator.h"
void Animator::AddAnimation(Animation& animation) {
	m_animations.emplace_back(animation);
}

void Animator::Update(const float deltaTime) {
	m_animations[m_animationIndex].Update(deltaTime);
}

void Animator::SetAnimationIndex(const int animationIndex) {
	m_animationIndex = animationIndex;
}

std::string Animator::GetCurrentFrameIdentifier() {
	Animation& currentAnimation = m_animations[m_animationIndex];
	return currentAnimation.m_frames[currentAnimation.m_currentFrame];
}
