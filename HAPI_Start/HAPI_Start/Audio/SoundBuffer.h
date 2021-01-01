#pragma once
#include <AL/al.h>
#include <vector>
#include "../Design Patterns/Singleton.h"

class SoundBuffer : public Singleton<SoundBuffer>
{
private:
	friend class Singleton<SoundBuffer>;
public:
	SoundBuffer(const SoundBuffer& other)                = delete;
	SoundBuffer(SoundBuffer&& other) noexcept            = delete;
	SoundBuffer& operator=(const SoundBuffer& other)     = delete;
	SoundBuffer& operator=(SoundBuffer&& other) noexcept = delete;

	ALuint AddSoundEffect(const char* filename);
	bool   RemoveSoundEffect(const ALuint& buffer);

private:
	std::vector<ALuint> m_soundEffectBuffers;

	SoundBuffer() = default;
	~SoundBuffer();
};
