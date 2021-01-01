#include "Music.h"
#include <cassert>
#include <AL/alext.h>
#include "HAPI_lib.h"

Music::Music(const std::string& filename) :
	m_source(),
	m_buffers{},
	m_musicFileInfo(),
	m_memBuff(nullptr),
	m_musicFormat()
{
	alGenSources(1, &m_source);
	alGenBuffers(constants::k_numBuffers, m_buffers);

	m_musicFile = sf_open(filename.c_str(), SFM_READ, &m_musicFileInfo);
	if (!m_musicFile)
	{
		HAPI.UserMessage("Could not open music file at : " + filename + " check the file path", "An error occurred");
	}

	assert(m_musicFile);

	// Using the file info, figure out the OpenAL format
	switch (m_musicFileInfo.channels)
	{
		case 1:
			m_musicFormat = AL_FORMAT_MONO16;
			break;
		case 2:
			m_musicFormat = AL_FORMAT_STEREO16;
			break;
		case 3:
			if (sf_command(m_musicFile, SFC_WAVEX_GET_AMBISONIC, nullptr, 0) == SF_AMBISONIC_B_FORMAT)
			{
				m_musicFormat = AL_FORMAT_BFORMAT2D_16;
			}
			break;
		case 4:
			if (sf_command(m_musicFile, SFC_WAVEX_GET_AMBISONIC, nullptr, 0) == SF_AMBISONIC_B_FORMAT)
			{
				m_musicFormat = AL_FORMAT_BFORMAT3D_16;
			}
			break;
		default:
			HAPI.UserMessage("Unsupported channel count in: " + filename, "An error occured");
			sf_close(m_musicFile);
			break;
	}

	const size_t frameSize = (static_cast<size_t>(constants::k_bufferSamples) * static_cast<size_t>(m_musicFileInfo.
		                          channels)) * sizeof(short);
	m_memBuff = static_cast<short*>(malloc(frameSize));
}

Music::~Music()
{
	// free the OpenAL sources
	alDeleteSources(1, &m_source);

	// Close the music file if it's open
	if (m_musicFile)
	{
		sf_close(m_musicFile);
	}

	m_musicFile = nullptr;

	// free the memory buffer
	free(m_memBuff);

	// delete the music buffers
	alDeleteBuffers(constants::k_numBuffers, m_buffers);
}

void Music::Play()
{
	// clear any OpenAL errors
	alGetError();

	// Rewind the source position and clear the buffer queue
	alSourceRewind(m_source);
	alSourcei(m_source, AL_BUFFER, 0);

	ALsizei i;

	// Fill the buffer queue
	for (i = 0; i < constants::k_numBuffers; ++i)
	{
		// Get data and give it to the buffer
		sf_count_t len = sf_read_short(m_musicFile, m_memBuff, constants::k_bufferSamples);
		if (len < 1)
		{
			break;
		}

		len *= m_musicFileInfo.channels * static_cast<sf_count_t>(sizeof(short));

		alBufferData(m_buffers[i], m_musicFormat, m_memBuff, static_cast<ALsizei>(len), m_musicFileInfo.samplerate);
	}

	assert(alGetError() != AL_NO_ERROR);

	// Queue and start playback
	alSourceQueueBuffers(m_source, i, m_buffers);
	alSourcePlay(m_source);

	assert(alGetError() != AL_NO_ERROR);
}

void Music::UpdateBufferStream() const
{
	// Clear any errors
	alGetError();

	// Get the relevant source info
	ALint state;
	alGetSourcei(m_source, AL_SOURCE_STATE, &state);

	ALint processed;
	alGetSourcei(m_source, AL_BUFFERS_PROCESSED, &processed);

	// Check no errors have occurred in getting the info
	assert(alGetError() != AL_NO_ERROR);


	// Un-queue and handle each processed buffer
	while (processed > 0)
	{
		ALuint buffer;
		alSourceUnqueueBuffers(m_source, 1, &buffer);
		processed--;

		// Read the next chunk of data
		sf_count_t len = sf_readf_short(m_musicFile, m_memBuff, constants::k_bufferSamples);

		// Refill the buffer
		if (len > 0)
		{
			len *= m_musicFileInfo.channels * static_cast<sf_count_t>(sizeof(short));
			alBufferData(buffer, m_musicFormat, m_memBuff, static_cast<ALsizei>(len), m_musicFileInfo.samplerate);

			// Queue it back to the source
			alSourceQueueBuffers(m_source, 1, &buffer);
		}

		// Check for errors
		assert(alGetError() != AL_NO_ERROR);
	}

	// Make sure the source hasn't under-run
	if (state != AL_PLAYING && state != AL_PAUSED)
	{
		ALint queued;

		// If no buffers are queued then playback is finished
		alGetSourcei(m_source, AL_BUFFERS_QUEUED, &queued);

		if (queued == 0)
		{
			return;
		}

		alSourcePlay(m_source);

		// Check for errors
		assert(alGetError() != AL_NO_ERROR);
	}
}
