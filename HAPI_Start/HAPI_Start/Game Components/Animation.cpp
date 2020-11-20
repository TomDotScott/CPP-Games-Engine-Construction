#include "Animation.h"
Animation::Animation(std::vector<std::string>& frames) :
	m_frames(frames),
	m_currentFrame(0),
	m_duration(100.f),
	m_playTime(0.f),
	m_state(EAnimationState::eStarted) {
}

Animation::Animation(std::vector<std::string>& frames, const float duration) :
	m_frames(frames),
	m_currentFrame(0),
	m_duration(duration),
	m_playTime(0.f),
	m_state(EAnimationState::eStarted) {

}

void Animation::Update(const float deltaTime) {
	if (m_state == EAnimationState::eStarted) {
		m_playTime += deltaTime;
		if (m_playTime >= m_duration) {
			m_playTime = 0.0f;
			if (m_currentFrame < m_frames.size() - 1) {
				++m_currentFrame;
			} else {
				m_currentFrame = 0;
			}
		}
	}
}
