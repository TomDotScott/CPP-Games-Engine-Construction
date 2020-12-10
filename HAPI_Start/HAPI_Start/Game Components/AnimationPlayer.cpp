#include "AnimationPlayer.h"
AnimationPlayer::AnimationPlayer(const std::vector<std::string>& frames, const bool looping, const float duration) :
	m_frames(frames),
	m_currentFrame(0),
	m_duration(duration),
	m_playTime(0.f),
	m_looping(looping),
	m_state(eAnimationState::e_Started)
{

}

void AnimationPlayer::Play(const float deltaTime)
{
	if (m_state == eAnimationState::e_Started)
	{
		m_playTime += deltaTime;
		if (m_playTime >= m_duration)
		{
			m_playTime = 0.0f;
			if (m_currentFrame < m_frames.size() - 1)
			{
				++m_currentFrame;
			} else
			{
				if (m_looping)
				{
					m_currentFrame = 0;
				} else
				{
					m_state = eAnimationState::e_Ended;
				}
			}
		}
	}
}

unsigned AnimationPlayer::GetCurrentFrame() const
{
	return m_currentFrame;
}

std::string AnimationPlayer::GetCurrentFrameIdentifier() const
{
	return m_frames[m_currentFrame];
}

eAnimationState AnimationPlayer::GetCurrentAnimationState() const
{
	return m_state;
}

void AnimationPlayer::ResetAnimation()
{
	m_state = eAnimationState::e_Started;
	m_currentFrame = 0;
}
