#include "SoundManager.h"
#include "HAPI_lib.h"

bool SoundManager::CreateSoundEffect(const std::string& sfxName, const std::string& filename)
{
	if (HAPI.LoadSound(filename))
	{
		if (!(m_sfxBuffer.find(sfxName) == m_sfxBuffer.end()))
		{
			HAPI.UserMessage("A sound effect with the name " + sfxName + " already exists.", "Error Occured");
			return false;
		} else
		{
			m_sfxBuffer.insert(std::pair<std::string, std::string>(sfxName, filename));
			return true;
		}
	}else
	{
		HAPI.UserMessage("A the sound effect: " + filename + " could not be found.", "Error Occured");
		return false;
	}
}

void SoundManager::PlaySFX(const std::string& sfxName)
{
	HAPI.PlaySound(m_sfxBuffer[sfxName]);
}