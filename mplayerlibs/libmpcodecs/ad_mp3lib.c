


#ifdef _XBOX
#include <xtl.h>
#else
#include <windows.h>
#endif
#include <sys/stat.h>
#include "../config.h"
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>

#include "../cygwin_inttypes.h"
#include "../help.orig\help_mp-en.h"

#include "../mp_msg.h"
#include "../bswap.h"

#ifdef USE_MP3LIB

#include "ad_internal.h"
int x_plus_minus_3dnow;
static ad_info_t info =
    {
        "MPEG layer-2, layer-3",
        "mp3lib",
        "Nick Kurshev",
        "mpg123",
        "Optimized to MMX/SSE/3Dnow!"
    };

LIBAD_EXTERN(mp3lib)

#include "../mp3lib/mp3.h"

extern int fakemono;

static sh_audio_t* dec_audio_sh=NULL;

// MP3 decoder buffer callback:
int mplayer_audio_read(char *buf,int size)
{
    return demux_read_data(dec_audio_sh->ds,buf,size);
}

static int preinit(sh_audio_t *sh)
{
    sh->audio_out_minsize=32*36*2*2; //4608;
    return 1;
}

static int init(sh_audio_t *sh)
{
    // MPEG Audio:
    dec_audio_sh=sh; // save sh_audio for the callback:
    //  MP3_Init(fakemono,mplayer_accel,&mplayer_audio_read); // TODO!!!
#ifdef USE_FAKE_MONO
    MP3_Init(fakemono);
#else
MP3_Init();
#endif
    MP3_samplerate=MP3_channels=0;
    sh->a_buffer_len=MP3_DecodeFrame(sh->a_buffer,-1);
    if(!sh->a_buffer_len)
        return 0; // unsupported layer/format
    sh->channels=2; // hack
    sh->samplerate=MP3_samplerate;
    sh->i_bps=MP3_bitrate*(1000/8);
    MP3_PrintHeader();
    return 1;
}

static void uninit(sh_audio_t *sh)
{}

static int control(sh_audio_t *sh,int cmd,void* arg, ...)
{
    switch(cmd)
    {
    case ADCTRL_RESYNC_STREAM:
        MP3_DecodeFrame(NULL,-2); // resync
        MP3_DecodeFrame(NULL,-2); // resync
        MP3_DecodeFrame(NULL,-2); // resync
        return CONTROL_TRUE;
    case ADCTRL_SKIP_FRAME:
        MP3_DecodeFrame(NULL,-2); // skip MPEG frame
        return CONTROL_TRUE;
    }
    return CONTROL_UNKNOWN;
}

static int decode_audio(sh_audio_t *sh_audio,unsigned char *buf,int minlen,int maxlen)
{
    return MP3_DecodeFrame(buf,-1);
}
#endif
