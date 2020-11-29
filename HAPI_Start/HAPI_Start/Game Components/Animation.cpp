#include "Animation.h"
Animation::Animation(std::vector<std::string>& frames, const bool looping, const float duration) :
	m_frames(frames),
	m_currentFrame(0),
	m_duration(duration),
	m_playTime(0.f),
	m_looping(looping),
	m_state(e_AnimationState::eStarted)
{

}

void Animation::Update(const float deltaTime)
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
