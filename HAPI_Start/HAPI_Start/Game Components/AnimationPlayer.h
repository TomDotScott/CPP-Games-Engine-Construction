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
	AnimationPlayer(std::vector<std::string> frames, bool looping = true, float duration = 100.f);
	void Play(float deltaTime);
	unsigned GetCurrentFrame() const;
	std::string GetCurrentFrameIdentifier() const;
	eAnimationState GetCurrentAnimationState() const;
	void ResetAnimation();
	
private:
	std::vector<std::string> m_frames;
	unsigned m_currentFrame;
	float m_duration;
	float m_playTime;
	bool m_looping;
	eAnimationState m_state;

};
