#include "AnimationPlayer.h"
AnimationPlayer::AnimationPlayer(std::vector<std::string>& frames, const bool looping, const float duration) :
	m_frames(frames),
	m_currentFrame(0),
	m_duration(duration),
	m_playTime(0.f),
	m_looping(looping),
	m_state(e_AnimationState::eStarted)
{

}

void AnimationPlayer::Play(const float deltaTime)
{
	if (m_state == e_AnimationState::eStarted)
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
					m_state = e_AnimationState::eEnded;
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

e_AnimationState AnimationPlayer::GetCurrentAnimationState() const
{
	return m_state;
}
