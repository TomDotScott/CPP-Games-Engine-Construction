#pragma once
#include <string>
#include <vector>

enum class eAnimationState
{
	e_Started, e_Paused, e_Ended
};

class AnimationPlayer
{
public:
	explicit        AnimationPlayer(std::vector<std::string> frames, bool looping = true, float duration = 100.f);
	void            Play(float deltaTime);
	void            ResetAnimation();
	
	unsigned        GetCurrentFrame() const;
	std::string     GetCurrentFrameIdentifier() const;
	eAnimationState GetCurrentAnimationState() const;

private:
	std::vector<std::string> m_frames;
	unsigned m_currentFrame;
	float m_duration;
	float m_playTime;
	bool m_looping;
	eAnimationState m_state;
	
};