#pragma once
#include <AL/al.h>

class SoundSource
{
public:
	SoundSource();

	// Abide to the rule of 5, since we have a explicitly defined destructor
	SoundSource(const SoundSource& other)                = default;
	SoundSource(SoundSource&& other) noexcept            = default;
	SoundSource& operator=(const SoundSource& other)     = default;
	SoundSource& operator=(SoundSource&& other) noexcept = default;
	~SoundSource();

	void Play(ALuint buffer);

private:
	ALuint m_source;
	float  m_pitch;
	float  m_gain;
	float  m_position[3];
	float  m_velocity[3];
	bool   m_loopSound;
	ALuint m_buffer;
};
