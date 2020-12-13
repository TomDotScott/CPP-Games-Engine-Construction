#pragma once
#include <AL/al.h>

class SoundSource 
{
public:
	SoundSource();
	~SoundSource();

	void Play(const ALuint buffer);

private:
	ALuint m_source;
	float m_pitch;
	float m_gain;
	float m_position[3];
	float m_velocity[3];
	bool m_loopSound;
	ALuint m_buffer;
};

