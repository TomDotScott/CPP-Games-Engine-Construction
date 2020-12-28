#include "SoundBuffer.h"
#include <sndfile.h>
#include <cinttypes>
#include <AL/alext.h>

#include "HAPI_lib.h"

// Loads the named audio file into an OpenAL buffer object,
// and returns the new buffer ID
ALuint SoundBuffer::AddSoundEffect(const char* filename)
{
	SF_INFO sfInfo;

	// Open the audio file and check that it's usable
	SNDFILE* soundFile = sf_open(filename, SFM_READ, &sfInfo);

	if (!soundFile)
	{
		HAPI.UserMessage("Could not open audio at: " + static_cast<std::string>(filename), "An error occured");
		return 0;
	}

	if (sfInfo.frames < 1 || sfInfo.frames > static_cast<sf_count_t>((INT_MAX / sizeof(short))) / sfInfo.channels)
	{
		HAPI.UserMessage("Bad sample count in: " + static_cast<std::string>(filename), "An error occured");
		sf_close(soundFile);
		return 0;
	}

	// Get the sound format and figure out the OpenAL format
	ALenum format = AL_NONE;
	switch (sfInfo.channels)
	{
	case 1:
		format = AL_FORMAT_MONO16;
		break;
	case 2:
		format = AL_FORMAT_STEREO16;
		break;
	case 3:
		if (sf_command(soundFile, SFC_WAVEX_GET_AMBISONIC, NULL, 0) == SF_AMBISONIC_B_FORMAT)
		{
			format = AL_FORMAT_BFORMAT2D_16;
		}
		break;
	case 4:
		if (sf_command(soundFile, SFC_WAVEX_GET_AMBISONIC, NULL, 0) == SF_AMBISONIC_B_FORMAT)
		{
			format = AL_FORMAT_BFORMAT3D_16;
		}
		break;
	default:
		HAPI.UserMessage("Unsupported channel count in: " + static_cast<std::string>(filename), "An error occured");
		sf_close(soundFile);
		return 0;
	}

	// Decode the audio file and add to the buffer
	auto* membuff = static_cast<short*>(malloc(static_cast<size_t>(sfInfo.frames * sfInfo.channels) * sizeof(short)));

	const sf_count_t frames = sf_readf_short(soundFile, membuff, sfInfo.frames);
	if(frames < 1)
	{
		free(membuff);
		sf_close(soundFile);
		HAPI.UserMessage("Failed to read samples in: " + static_cast<std::string>(filename), "An error occured");
		return 0;
	}

	const ALsizei bytes = static_cast<ALsizei>(frames * sfInfo.channels) * static_cast<ALsizei>(sizeof(short));

	// Buffer the audio data into a new buffer object
	ALuint buffer = 0;
	alGenBuffers(1, &buffer);
	alBufferData(buffer, format, membuff, bytes, sfInfo.samplerate);

	// free the data and close the file
	free(membuff);
	sf_close(soundFile);

	// Check if an error occured and clean up if so
	const ALenum err = alGetError();
	if(err != ALC_NO_ERROR)
	{
		HAPI.UserMessage("OpenAL Error: " + static_cast<std::string>(const_cast<char*>(alGetString(err))), "An error occured");
		if(buffer && alIsBuffer(buffer))
		{
			alDeleteBuffers(1, &buffer);
		}
		return 0;
	}

	// Finally, add to the list of known buffers
	m_soundEffectBuffers.emplace_back(buffer);
	
	return buffer;	
}

bool SoundBuffer::RemoveSoundEffect(const ALuint& buffer)
{
	auto it = m_soundEffectBuffers.begin();
	while(it != m_soundEffectBuffers.end())
	{
		if(*it == buffer)
		{
			// We have found the buffer so free it, and remove
			// from the list of known buffers
			alDeleteBuffers(1, &*it);

			it = m_soundEffectBuffers.erase(it);

			return true;
		}else
		{
			++it;
		}
	}
	// Buffer not found
	return false;
}

SoundBuffer::~SoundBuffer()
{
	// Clear all the buffers
	alDeleteBuffers(m_soundEffectBuffers.size(), static_cast<const ALuint*>(m_soundEffectBuffers.data()));
	
	// Erase the vector
	m_soundEffectBuffers.clear();
}

