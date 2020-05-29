//
//  dsSound.mm


#include "dsSound.h"
#include "dsOpenALSoundManager.h"


#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))


//-------------------------------------------------------------------
// class dsSound

dsSound::dsSound(std::string name) : dsResource(name)
{
	m_isLoop = false;
	
	m_sourceID = 0;
	m_volume = 1.0;
	m_pitch = 1.0;	
	m_pBufferData = NULL;
}

dsSound::~dsSound()
{
	//delete base source...
	alDeleteSources(1, &m_sourceID);
	
	VEC_SOUNDSOURCE::iterator it, it_end = m_vecSoundSource.end();
	for(it = m_vecSoundSource.begin(); it != it_end; it++)
	{
		alDeleteSources(1, &(*it));
	}
	m_vecSoundSource.clear();
	
	//now the buffer, since it wasn't copied to openAL
	alDeleteBuffers(1, &m_bufferID);
	
	if(m_pBufferData)
	{
		free(m_pBufferData);
		m_pBufferData = NULL;
	}	
	
	
}



bool dsSound::Load(std::string &name, bool isLoop)
{		
	ALenum  format;
	ALsizei size;
	ALsizei freq;
	
	alGetError(); //clear any previous error...

	// grab a buffer ID from openAL
	alGenBuffers(1, &m_bufferID);

	dsOpenALSoundManager *pSoundManger = GetOpenALSoundManager();
	if (pSoundManger->LoadWaveToBuffer(name.c_str(), m_bufferID, 0))
	{

	}
	
	// grab a source ID from openAL, this will be the base source ID
	alGenSources(1, &m_sourceID); 
	
	// attach the buffer to the source
	alSourcei(m_sourceID, AL_BUFFER, m_bufferID);

	m_isLoop = isLoop;	
	if(m_isLoop)
		alSourcei(m_sourceID, AL_LOOPING, AL_TRUE);
		

	if((m_error = alGetError()) != AL_NO_ERROR) 
		return false;

	return true;	
}


bool dsSound::Play()
{
	if(isPlaying()) //see if the base source is busy...
	{
		//if so, create a new source
		ALuint tmpSourceID;
		alGenSources(1, &tmpSourceID);
		
		//attach the buffer to the source
		alSourcei(tmpSourceID, AL_BUFFER, m_bufferID);
		if(m_isLoop)
			alSourcei(m_sourceID, AL_LOOPING, AL_TRUE);
		
		alSourcePlay(tmpSourceID);
		
		m_vecSoundSource.push_back(tmpSourceID);		
		
		return ((m_error = alGetError()) != AL_NO_ERROR);
	}
	
	//if the base source isn't busy, just use that one...
	alSourcePlay(m_sourceID);
	
	return ((m_error = alGetError()) != AL_NO_ERROR);
}


bool dsSound::Stop()
{
	alSourceStop(m_sourceID);
	
	return ((m_error = alGetError()) != AL_NO_ERROR);
}

bool dsSound::Pause()
{
	alSourcePause(m_sourceID);
	
	return ((m_error = alGetError()) != AL_NO_ERROR);
}

bool dsSound::Rewind()
{
	alSourceRewind(m_sourceID);
	
	return ((m_error = alGetError()) != AL_NO_ERROR);
}


// returns whether the BASE source is playing or not
bool dsSound::isPlaying()
{	
	ALenum state;	
    alGetSourcei(m_sourceID, AL_SOURCE_STATE, &state);
	
    return (state == AL_PLAYING);
}

// returns YES if the base or any temporary sounds are playing...
bool dsSound::isMultiPlaying()
{
	return isPlaying() || m_vecSoundSource.size() > 0;
}


void dsSound::SetVolume(ALfloat newVolume)
{
	m_volume = MAX(MIN(newVolume, 1.0f), 0.0f); //cap to 0-1
	alSourcef(m_sourceID, AL_GAIN, m_volume);	
	
	//now set the volume for any temporary sounds...
	VEC_SOUNDSOURCE::iterator it, it_end = m_vecSoundSource.end();
	for(it = m_vecSoundSource.begin(); it != it_end; it++)
	{
		alSourcef((*it), AL_GAIN, m_volume);
	}	
}

void dsSound::SetPitch(ALfloat newPitch)
{
	m_pitch = newPitch;
	
	alSourcef(m_sourceID, AL_PITCH, m_pitch);	
	
	VEC_SOUNDSOURCE::iterator it, it_end = m_vecSoundSource.end();
	for(it = m_vecSoundSource.begin(); it != it_end; it++)
	{
		alSourcef((*it), AL_PITCH, m_pitch);
	}
}


