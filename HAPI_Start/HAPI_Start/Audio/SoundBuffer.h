#pragma once
#include <AL/al.h>
#include <vector>
#include "../Design Patterns/Singleton.h"

class SoundBuffer : public Singleton<SoundBuffer>
{
private:
	friend class Singleton<SoundBuffer>;
public:
	// As the class is a singleton, we don't want it to be copyable or moveable;
	// only one instance of the class should exist
	SoundBuffer(const SoundBuffer& other)                = delete;
	SoundBuffer(SoundBuffer&& other) noexcept            = delete;
	SoundBuffer& operator=(const SoundBuffer& other)     = delete;
	SoundBuffer& operator=(SoundBuffer&& other) noexcept = delete;

	/**
	 * \brief Loads the named audio file into an OpenAL buffer object and returns the new buffer ID
	 * \param filename - The file location, relative to the working directory, of the sound effect file
	 * \return the new buffer ID
	 */
	ALuint AddSoundEffect(const char* filename);
	bool   RemoveSoundEffect(const ALuint& buffer);

private:
	std::vector<ALuint> m_soundEffectBuffers;

	SoundBuffer() = default;
	~SoundBuffer();
};
