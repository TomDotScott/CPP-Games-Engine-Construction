#pragma once
#include <string>
#include <vector>

enum class EAnimationState {
	eStarted, eEnded, ePaused 
};

class Animation {
	friend class Animator;
public:
	explicit Animation(std::vector<std::string>& frames, bool looping = true, float duration = 100.f);
private:
	std::vector<std::string> m_frames;
	unsigned m_currentFrame;
	float m_duration;
	float m_playTime;
	bool m_looping;
	EAnimationState m_state;

	void Update(float deltaTime);
};
