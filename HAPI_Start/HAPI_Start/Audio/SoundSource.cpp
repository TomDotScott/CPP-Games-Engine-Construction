#include "SoundSource.h"

SoundSource::SoundSource() :
	m_source(),
	m_pitch(1.f),
	m_gain(1.f),
	m_position{0, 0, 0},
	m_velocity{0, 0, 0},
	m_loopSound(false),
	m_buffer(0)
{
	alGenSources(1, &m_source);
	alSourcef(m_source, AL_PITCH, m_pitch);
	alSourcef(m_source, AL_GAIN, m_gain);
	alSource3f(m_source, AL_POSITION, m_position[0], m_position[1], m_position[2]);
	alSource3f(m_source, AL_VELOCITY, m_velocity[0], m_velocity[1], m_velocity[2]);
	alSourcei(m_source, AL_LOOPING, m_loopSound);
	alSourcei(m_source, AL_BUFFER, m_buffer);
}

SoundSource::~SoundSource()
{
	alDeleteSources(1, &m_source);
}

void SoundSource::Play(const ALuint buffer)
{
	// Stop the current source...
	alSourceStop(m_source);

	if (buffer != m_buffer)
	{
		m_buffer = buffer;
		alSourcei(m_source, AL_BUFFER, static_cast<ALuint>(m_buffer));
	}

	alSourcePlay(m_source);
}
