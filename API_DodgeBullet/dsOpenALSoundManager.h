//
//  dsOpenALSoundManager.h

#include "dsManager.h"
#include "dsSound.h"
#include "cwaves.h"

class dsOpenALSoundManager : public dsManager
{
public:
	dsOpenALSoundManager();
	~dsOpenALSoundManager();
	
	dsSound *LoadSound(std::string name, bool isLoop);

	bool LoadWaveToBuffer(const char *szWaveFile, ALuint uiBufferID, ALenum eXRAMBufferMode);
	
protected:
	void InitOpenAL();
	void DestroyOpenAL();
	
};

dsOpenALSoundManager *GetOpenALSoundManager();