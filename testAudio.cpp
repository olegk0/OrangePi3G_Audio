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

void print_usage() {
    printf("Usage:\n ./testAudio output mode arg\nWhere:\n  output: headphone|speaker\n  mode: mp3 or fm\n  arg: file.mp3 or on|off\n");
}

typedef enum {
    wmode_fm,
    wmode_mp3,
} wmode_t;

int main(int argc, char **argv) {
    if (argc < 4) {
        print_usage();
        return 0;
    }

    uint32_t output_device;  // AUDIO_DEVICE_OUT_EARPIECE AUDIO_DEVICE_OUT_SPEAKER  AUDIO_DEVICE_OUT_WIRED_HEADSET AUDIO_DEVICE_OUT_WIRED_HEADPHONE AUDIO_DEVICE_OUT_DEFAULT
    if (strcmp(argv[1], "headphone") == 0) {
        output_device = AUDIO_DEVICE_OUT_WIRED_HEADPHONE;
    } else if (strcmp(argv[1], "speaker") == 0) {
        output_device = AUDIO_DEVICE_OUT_SPEAKER;
    } else {
        print_usage();
        return 0;
    }

    wmode_t wmode;
    if (strcmp(argv[2], "mp3") == 0) {
        wmode = wmode_mp3;
    } else if (strcmp(argv[2], "fm") == 0) {
        wmode = wmode_fm;
    } else {
        print_usage();
        return 0;
    }

    bool fm_on = false;
    if (strcmp(argv[3], "on") == 0) {
        fm_on = true;
    }

    if (wmode == wmode_fm) {
        printf("FM %s\n", fm_on ? "on" : "off");
    } else {
        printf("Play file %s\n", argv[3]);
    }

    int ret;
    bool SpeechControlEnable = false;
    AudioMTKHardware audioMTKHardware(SpeechControlEnable);
    printf("=====================0=======================\n");
    ret = audioMTKHardware.setMode(AUDIO_MODE_NORMAL);
    // ret = audioMTKHardware.setMode(AUDIO_MODE_RINGTONE);
    printf("setMode, ret:%d\n", ret);
    printf("=====================1=======================\n");
    AudioResourceManagerInterface *mAudioResourceManager = AudioResourceManager::getInstance();
    ret = mAudioResourceManager->SelectOutputDevice(output_device);  // AUDIO_DEVICE_OUT_EARPIECE
    printf("SelectOutputDevice, ret:%d\n", ret);

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
    return 0;
#endif

    if (wmode == wmode_fm) {
        printf("======================2======================\n");
        /*set the audio volume for all audio activities other than voice call.
         * Range between 0.0 and 1.0. If any value other than NO_ERROR is returned,
         */
        ret = audioMTKHardware.setMasterVolume(0.5f);  //
        printf("setMasterVolume, ret:%d\n", ret);

#if 1
        AudioFtm *mAudioFtmInstance = AudioFtm::getInstance();
        mAudioFtmInstance->Audio_FM_I2S_Play(fm_on);
#else
        int par1 = AUDIO_USER_TEST;
        /*
        0x100 - Fm Enable
        0x101 - Fm Disable
        */
        int par2 = fm_on ? 0x100 : 0x101;
        ret = audioMTKHardware.SetAudioCommand(par1, par2);
        printf("SetAudioCommonCommand, ret:%d\n", ret);
#endif
        sleep(1);
    } else {
        printf("======================2======================\n");
        /*set the audio volume for all audio activities other than voice call.
         * Range between 0.0 and 1.0. If any value other than NO_ERROR is returned,
         */
        ret = audioMTKHardware.setMasterVolume(0.0f);  //
        printf("setMasterVolume, ret:%d\n", ret);
        printf("======================3======================\n");

        //   int buffer_size = mpg123_outblock(mh);
        //   unsigned char buffer[buffer_size];

        if (mpg123_init() == MPG123_OK) {
            int err;
            mh = mpg123_new(NULL, &err);
            if (mh) {
                if (openMp3(argv[3])) {
                    int format = AUDIO_FORMAT_PCM_16_BIT;        // AUDIO_FORMAT_PCM_16_BIT
                    uint32_t channels = AUDIO_CHANNEL_OUT_MONO;  // = AUDIO_CHANNEL_OUT_STEREO;         // AUDIO_CHANNEL_OUT_STEREO
                    if (mp3_channels > 1) {
                        channels = AUDIO_CHANNEL_OUT_STEREO;
                    }
                    uint32_t sampleRate = sample_rate;  // 44100 by def
                    status_t status = 0;                //
                    AudioStreamOut *streamOutput = audioMTKHardware.openOutputStream(output_device, &format, &channels, &sampleRate, &status);
                    printf("openOutputStream, ret:%p\n", streamOutput);
                    printf("======================4======================\n");
                    uint32_t hwBufferSize;

                    hwBufferSize = streamOutput->bufferSize();  // 16k bytes
                    printf("hwBufferSize:%d\n", hwBufferSize);
                    unsigned char outBuffer[hwBufferSize + 10];

                    printf("======================9======================\n");
                    size_t done;
                    int cnt = 0;
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

                mpg123_delete(mh);
            } else {
                printf("Error mpg123_new %d\n", err);
            }
            mpg123_exit();
        } else {
            printf("Error mpg123_init\n");
        }
    }
    return 0;
}