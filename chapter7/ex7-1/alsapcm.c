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
    
}

int setupDSP(snd_pcm_t *dev, int buf_size, int format, int sampleRate, int channels)
{

}