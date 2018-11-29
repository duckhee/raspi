#include <fcntl.h>
#include <limits.h>
#include <linux/soundcard.h>
#include <math.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include <alsa/asoundlib.h>

#define BITS          2
#define FRAGMENT      8
#define DURATION      5.0
#define MODE          1
#define FREQ          44100
#define BUFSIZE       (int)(BITS*FREQ*DURATION*MODE)

int setupDSP(snd_pcm_t *dev, int buf_size, int format, int sampleRate, int channels);

int main(int argc, char **argv)
{
    snd_pcm_t *playback_handle;
    double total = DURATION, t;
    int freq = 440;
    short buf[BUFSIZ];
    int i, frames, count = 1;
    char *snd_dev_out = "plughw:0,0";
    
    if(snd_pcm_open(&playback_handle, snd_dev_out, SND_PCM_STREAM_PLAYBACK, 0)<0) 
    {
        perror("Cound not open output audio dev");
        return -1;
    }

    if(setupDSP(playbac_handle, BUFSIZ, SND_PCM_FORMAT_S16_LE, FREQ, MODE)<0)
    {
        perror("Cound not set output audio device");
        return -1;
    }

    printf("Make Sine wave!!!\n");
    for(i = 0; i < BUFSIZ; i++)
    {
        t = (total/BUFSIZ)*i;
        buf[i] = SHRT_MAX * sin((int)(2.0*M_PI*freq*t));
    }

    frames = BUFSIZ/(MODE*BITS);

    while(count--)
    {
        snd_pcm_prepare(playback_handle);
        snd_pcm_writei(playback_handle, buf, frames);
    }

    snd_pcm_drop(playback_handle);
    snd_pcm_close(playback_handle);

    return 0;
}

int setupDSP(snd_pcm_t *dev, int buf_size, int format, int sampleRate, int channels)
{
    snd_pcm_hw_params_t *hw_params;
    snd_pcm_uframes_t frames;
    int fragments = FRAGMENT;
    int bits = (formant == SND_PCM_FORMAT_S16_LE) ? 2 : 1;
}