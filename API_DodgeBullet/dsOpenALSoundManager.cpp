//
//  dsOpenALSoundManager.mm


#include "dsOpenALSoundManager.h"


static dsOpenALSoundManager g_pOpenALSoundManager;
static CWaves  *g_pWaveLoader;

dsOpenALSoundManager *GetOpenALSoundManager()
{
	return &g_pOpenALSoundManager;
}

dsOpenALSoundManager::dsOpenALSoundManager()
{
	InitOpenAL();
}

dsOpenALSoundManager::~dsOpenALSoundManager()
{
	DestroyOpenAL();
}

void dsOpenALSoundManager::InitOpenAL()
{
	ALCcontext	*context = NULL;
	ALCdevice	*device = NULL;
	
	// Initialization
	device = alcOpenDevice(NULL); // select the "preferred device"
	if(!device) 
		return ;
	
	// use the device to make a context
	context = alcCreateContext(device, NULL);
	if(!context) 
		return ;
	
	// set my context to the currently active one
	alcMakeContextCurrent(context);

	g_pWaveLoader = new CWaves();
}


void dsOpenALSoundManager::DestroyOpenAL()
{
	ALCcontext	*context = NULL;
    ALCdevice	*device = NULL;
	
	//Get active context (there can only be one)
    context = alcGetCurrentContext();
	
    //Get device for active context
    device = alcGetContextsDevice(context);
	
	alcMakeContextCurrent(NULL);
	
    //Release context
    alcDestroyContext(context);
	
    //Close device
    alcCloseDevice(device);	

	if (g_pWaveLoader) delete g_pWaveLoader;
	g_pWaveLoader = NULL;
}

bool dsOpenALSoundManager::LoadWaveToBuffer(const char *szWaveFile, ALuint uiBufferID, ALenum eXRAMBufferMode)
{
	WAVEID			WaveID;
	ALint			iDataSize, iFrequency;
	ALenum			eBufferFormat;
	ALchar			*pData;
	bool			bReturn;

	bReturn = AL_FALSE;
	if (g_pWaveLoader)
	{
		if (SUCCEEDED(g_pWaveLoader->LoadWaveFile(szWaveFile, &WaveID)))
		{
			if ((SUCCEEDED(g_pWaveLoader->GetWaveSize(WaveID, (unsigned long*)&iDataSize))) &&
				(SUCCEEDED(g_pWaveLoader->GetWaveData(WaveID, (void**)&pData))) &&
				(SUCCEEDED(g_pWaveLoader->GetWaveFrequency(WaveID, (unsigned long*)&iFrequency))) &&
				(SUCCEEDED(g_pWaveLoader->GetWaveALBufferFormat(WaveID, &alGetEnumValue, (unsigned long*)&eBufferFormat))))
			{
				// Set XRAM Mode (if application)
				//if (eaxSetBufferMode && eXRAMBufferMode)
				//	eaxSetBufferMode(1, &uiBufferID, eXRAMBufferMode);

				alGetError();
				alBufferData(uiBufferID, eBufferFormat, pData, iDataSize, iFrequency);
				if (alGetError() == AL_NO_ERROR)
					bReturn = AL_TRUE;

				g_pWaveLoader->DeleteWaveFile(WaveID);
			}
		}
	}

	return bReturn;
}

dsSound *dsOpenALSoundManager::LoadSound(std::string name, bool isLoop)
{
	dsSound *pSound = (dsSound *)Find(name);
	if(pSound)
		return pSound;
	
	pSound = new dsSound(name);
	
	if(!pSound->Load(name, isLoop))
	{
		delete pSound;
		return NULL;
	}
	
	Add(name, (dsResource *)pSound);
	
	return pSound;
}



