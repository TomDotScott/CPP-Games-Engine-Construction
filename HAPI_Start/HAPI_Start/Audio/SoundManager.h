#pragma once
#include <unordered_map>

#include "SoundBuffer.h"
#include "SoundDevice.h"
#include "SoundSource.h"

class SoundManager : public Singleton<SoundManager>
{
	friend class Singleton<SoundManager>;
public:
	bool AddSoundEffect(const std::string& sfxName, const std::string& filename);
	void PlaySoundEffect(const std::string& sfxName);
private:
	std::unordered_map<std::string, ALuint> m_soundBuffers;

	const SoundDevice& m_soundDevice;
	
	SoundSource m_source;

	SoundManager();
	
};

