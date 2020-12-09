#pragma once
#include <string>
#include <unordered_map>

#include "../Design Patterns/Singleton.h"

class SoundManager : public Singleton<SoundManager>
{
	friend class Singleton<SoundManager>;
public:
	bool CreateSoundEffect(const std::string& sfxName, const std::string& filename);
	void PlaySFX(const std::string& sfxName);
	void StopSound();


private:
	std::unordered_map<std::string, std::string> m_sfxBuffer;
};

