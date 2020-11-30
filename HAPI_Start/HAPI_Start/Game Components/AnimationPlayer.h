#pragma once
#include <string>
#include <vector>

enum class e_AnimationState
{
	eStarted, ePaused, eEnded
};

class AnimationPlayer
{
public:
	AnimationPlayer(std::vector<std::string>& frames, bool looping = true, float duration = 100.f);
	void Play(float deltaTime);
	unsigned GetCurrentFrame() const;
	std::string GetCurrentFrameIdentifier() const;
	e_AnimationState GetCurrentAnimationState() const;
	
private:
	std::vector<std::string> m_frames;
	unsigned m_currentFrame;
	float m_duration;
	float m_playTime;
	bool m_looping;
	e_AnimationState m_state;

};
