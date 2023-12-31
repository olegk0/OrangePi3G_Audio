#ifndef AUDIO_MTK_STREAM_IN_CLIENT_H
#define AUDIO_MTK_STREAM_IN_CLIENT_H

#include "AudioType.h"
#include "AudioUtility.h"
#include "AudioStreamAttribute.h"
#include <utils/threads.h>

extern "C" {
#include "bli_exp.h"
#include "MtkAudioSrc.h"
}

class AudioMTKStreamIn;

// #define ENABLE_BLI

class AudioMTKStreamInClient {
   public:
    AudioMTKStreamInClient(uint32 BuffeSize, uint32 ClientId);
    ~AudioMTKStreamInClient();
    RingBuf mRingBuf;
    uint32 mClientId;
    uint32 mMemDataType;  // which type of data need to be record
    uint32 mSourceType;
    AudioStreamAttribute *mAttributeClient;
    bool mEnable;
    Mutex mLock;
    Condition mWaitWorkCV;

#ifdef ENABLE_BLI
    // BLI_SRC
    BLI_HANDLE *mBliHandlerBuffer;
    char *mBliOutputLinearBuffer;

    // Bli src new version
    MtkAudioSrc *mBliSrc;
#endif
    bool mEnableBesRecord;
    AudioMTKStreamIn *mStreamIn;
    struct timespec mInputStartTime;
};

#endif
