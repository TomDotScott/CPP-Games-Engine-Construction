#pragma once
#include <AL/al.h>
#include <string>
#include <vector>
#include "../Design Patterns/Singleton.h"

class SoundBuffer : public Singleton<SoundBuffer>
{
	friend class Singleton<SoundBuffer>;
public:

	ALuint AddSoundEffect(const char* filename);
	bool RemoveSoundEffect(const ALuint& buffer);

private:
	std::vector<ALuint> m_soundEffectBuffers;

	SoundBuffer() = default;
	~SoundBuffer();
};

