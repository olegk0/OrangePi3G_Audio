#include <cstdio>
#include <cstdlib>

#include <AudioMTKHardware.h>
// #include "AudioParamTuning.h"
#include <mpg123.h>

mpg123_handle *mh = NULL;
int sample_size;
long sample_rate;
int mp3_channels, mp3_encoding;

bool openMp3(char *file) {
    printf("file: %s\n", file);
    mpg123_close(mh);
    /* open the file and get the decoding format */
    if (mpg123_open(mh, file) == MPG123_OK) {
        mpg123_getformat(mh, &sample_rate, &mp3_channels, &mp3_encoding);
        sample_size = mpg123_encsize(mp3_encoding) * mp3_channels;
        printf("sample_size: %d channels:%d  rate:%d\n", sample_size, mp3_channels, sample_rate);

        return true;
        //        mpg123_close(mh);
    }
    return false;
}

int main(int argc, char **argv) {
    printf("Begin\n");
    int ret;
    int cnt;

    bool SpeechControlEnable = false;
    AudioMTKHardware audioMTKHardware(SpeechControlEnable);
    printf("=====================0=======================\n");
    ret = audioMTKHardware.setMode(AUDIO_MODE_NORMAL);
    // ret = audioMTKHardware.setMode(AUDIO_MODE_RINGTONE);
    printf("setMode, ret:%d\n", ret);
    printf("=====================1=======================\n");
    AudioResourceManagerInterface *mAudioResourceManager = AudioResourceManager::getInstance();
    ret = mAudioResourceManager->SelectOutputDevice(AUDIO_DEVICE_OUT_WIRED_HEADPHONE);  // AUDIO_DEVICE_OUT_EARPIECE
    printf("SelectOutputDevice, ret:%d\n", ret);
    printf("======================2======================\n");
    /*set the audio volume for all audio activities other than voice call.
     * Range between 0.0 and 1.0. If any value other than NO_ERROR is returned,
     */
    ret = audioMTKHardware.setMasterVolume(0.0f);  //
    printf("setMasterVolume, ret:%d\n", ret);

#if 0
    int par1 = AUDIO_USER_TEST;
    /*
    0 - EarphoneTest
    4 - LouderSPKTest

    */
    int par2 = 0;
    ret = audioMTKHardware.SetAudioCommand(par1, par2);
    printf("SetAudioCommonCommand, ret:%d\n", ret);
    sleep(1);
    par2++;
    audioMTKHardware.SetAudioCommand(par1, par2);
    sleep(1);
    // return 0;
#endif

    printf("======================3======================\n");

    //   int buffer_size = mpg123_outblock(mh);
    //   unsigned char buffer[buffer_size];

    if (argc < 2) {
        return 1;
    }

    if (mpg123_init() != MPG123_OK) {
        printf("Error mpg123_init\n");
        return 1;
    }

    int err;
    mh = mpg123_new(NULL, &err);
    if (mh == NULL) {
        printf("Error mpg123_new %d\n", err);
        return 1;
    }

    if (openMp3(argv[1])) {
        uint32_t devices = AUDIO_DEVICE_OUT_WIRED_HEADPHONE;  // AUDIO_DEVICE_OUT_EARPIECE AUDIO_DEVICE_OUT_SPEAKER  AUDIO_DEVICE_OUT_WIRED_HEADSET AUDIO_DEVICE_OUT_WIRED_HEADPHONE AUDIO_DEVICE_OUT_DEFAULT
        int format = AUDIO_FORMAT_PCM_16_BIT;                 // AUDIO_FORMAT_PCM_16_BIT
        uint32_t channels = AUDIO_CHANNEL_OUT_MONO;           // = AUDIO_CHANNEL_OUT_STEREO;         // AUDIO_CHANNEL_OUT_STEREO
        if (mp3_channels > 1) {
            channels = AUDIO_CHANNEL_OUT_STEREO;
        }
        uint32_t sampleRate = sample_rate;  // 44100 by def
        status_t status = 0;                //
        AudioStreamOut *streamOutput = audioMTKHardware.openOutputStream(devices, &format, &channels, &sampleRate, &status);
        printf("openOutputStream, ret:%p\n", streamOutput);
        printf("======================4======================\n");
        uint32_t hwBufferSize;

        hwBufferSize = streamOutput->bufferSize();  // 16k bytes
        printf("hwBufferSize:%d\n", hwBufferSize);
        unsigned char outBuffer[hwBufferSize + 10];

        printf("======================7======================\n");
        //       Audio_Set_HeadPhone(true, 1);
        printf("======================9======================\n");
        size_t done;
        cnt = 0;
        while (1) {
            cnt++;
            int ret = mpg123_read(mh, outBuffer, hwBufferSize, &done);
            if (ret == MPG123_OK || (ret == MPG123_DONE && done > 0)) {
                int r = streamOutput->write(outBuffer, done);
                printf("streamOutput  ret:%ld\n", r);

            } else {
                printf("mpg123_read  end play, ret:%d, done:%ld\n", ret, done);
                break;
            }
        }
        printf("======================10======================\n");
        mAudioResourceManager->StopOutputDevice();

    } else {
        printf("error open file:%s\n", argv[1]);
    }

    if (mh) {
        mpg123_delete(mh);
    }

    return 0;
}