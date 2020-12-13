#pragma once
#include <AL/al.h>
#include <string>
#include <vector>

class SoundBuffer
{
public:
	static SoundBuffer* GetInstance();

	ALuint AddSoundEffect(const char* filename);
	bool RemoveSoundEffect(const ALuint& buffer);

private:
	std::vector<ALuint> m_soundEffectBuffers;

	SoundBuffer() = default;
	~SoundBuffer();
};

