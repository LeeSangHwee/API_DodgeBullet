//
//  dsSound.h

#pragma once

#include "dsResource.h"
#include "./OpenAL1_1/include/al.h"
#include "./OpenAL1_1/include/alc.h"
#include <vector>



class dsSound : public dsResource
{
protected:	
	typedef std::vector<ALuint> VEC_SOUNDSOURCE;
	VEC_SOUNDSOURCE m_vecSoundSource;
	
	
	ALuint		m_bufferID;		
	ALuint		m_sourceID;			//base source ID
	ALdouble	m_duration;			//duration of the sound in seconds
	ALfloat		m_volume;				//volume [0 - 1]
	ALfloat		m_pitch;				//speed
	bool		m_isLoop;
	ALenum		m_error;
	ALvoid		*m_pBufferData;		//holds the actual sound data
	


	
	
public:
	dsSound(std::string name);
	virtual ~dsSound();
	
	bool Load(std::string &name, bool isLoop = false);
	
	void SetPitch(ALfloat newPitch);
	void SetVolume(ALfloat newVolume);
	bool isMultiPlaying();
	bool isPlaying();	
	bool Rewind();	
	bool Pause();	
	bool Stop();	
	bool Play();	
};
