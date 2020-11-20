#pragma once
#include <string>
#include <vector>

enum class EAnimationState {
	eStarted, eEnded, ePaused 
};

class Animation {
	friend class Animator;
public:
	explicit Animation(std::vector<std::string>& frames);
	explicit Animation(std::vector<std::string>& frames, float duration);
private:
	std::vector<std::string> m_frames;
	unsigned m_currentFrame;
	float m_duration;
	float m_playTime;
	EAnimationState m_state;

	void Update(float deltaTime);
};
