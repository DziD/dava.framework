/*==================================================================================
    Copyright (c) 2008, DAVA Consulting, LLC
    All rights reserved.

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright
    notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
    notice, this list of conditions and the following disclaimer in the
    documentation and/or other materials provided with the distribution.
    * Neither the name of the DAVA Consulting, LLC nor the
    names of its contributors may be used to endorse or promote products
    derived from this software without specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE DAVA CONSULTING, LLC AND CONTRIBUTORS "AS IS" AND
    ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
    WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
    DISCLAIMED. IN NO EVENT SHALL DAVA CONSULTING, LLC BE LIABLE FOR ANY
    DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
    (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
    LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
    ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
    (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
    SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

    Revision History:
        * Created by Ivan Petrochenko
=====================================================================================*/

#ifndef __DAVAENGINE_SOUND_DATA_PROVIDER_H__
#define __DAVAENGINE_SOUND_DATA_PROVIDER_H__

#include "Base/BaseTypes.h"
#include "Sound/ALUtils.h"

namespace DAVA
{

class SoundBuffer;
class SoundDataProvider
{
public:
	SoundDataProvider(const String & fileName);
	virtual ~SoundDataProvider() {};

	virtual bool Init();
	virtual int32 LoadData(int8 ** buffer, int32 desiredSize) = 0;
	virtual void Rewind() = 0;
	inline int32 GetStreamBufferSize();

	int32 GetSampleRate();
#ifdef __DAVASOUND_AL__
	ALenum GetFormat();
#endif

protected:
	String fileName;
	int32 dataSize;
	int32 leftDataSize;
	int32 channelsCount;
	int32 sampleRate;
	int32 sampleSize;
	int32 streamBufferSize;

	bool isInited;
};

int32 SoundDataProvider::GetStreamBufferSize()
{
	return streamBufferSize;
}

};

#endif //__DAVAENGINE_SOUND_DATA_PROVIDER_H__