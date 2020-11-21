#pragma once
#include <vector>

#include "Animation.h"

class Animator {
public:
	void AddAnimation(Animation& animation);
	void Update(float deltaTime);
	void SetAnimationIndex(int animationIndex);
	std::string GetCurrentFrameIdentifier();
	EAnimationState GetCurrentAnimationState() const;
	
private:
	std::vector<Animation> m_animations;
	int m_animationIndex{ 0 };
};
