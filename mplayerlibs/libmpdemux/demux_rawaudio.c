
#ifdef _XBOX
#include <xtl.h>
#else
#include <windows.h>
#endif
#include "config.h"
#include <stdlib.h>
#include <stdio.h>

#include "cygwin_inttypes.h"
#include "mp_msg.h"
#include "url.h"
#include "network.h"
#include "stream.h"
#include "demuxer.h"
#include "stheader.h"
#include "url.h"
#include "mp3_hdr.h"
#include "../help.orig\help_mp-en.h"
#include "../cfgparser.h"

#pragma warning (disable:4018)
#pragma warning (disable:4244)
#pragma warning (disable:4305)
#pragma warning (disable:4133)
#pragma warning (disable:4013)


extern int demuxer_type;
static int channels = 2;
static int samplerate = 44100;
static int samplesize = 2;
static int format = 0x1; // Raw PCM

config_t demux_rawaudio_opts[] = {
                                     { "on", &demuxer_type, CONF_TYPE_FLAG, 0,0, DEMUXER_TYPE_RAWAUDIO, NULL },
                                     { "channels", &channels, CONF_TYPE_INT,CONF_RANGE,1,8, NULL },
                                     { "rate", &samplerate, CONF_TYPE_INT,CONF_RANGE,1000,8*48000, NULL },
                                     { "samplesize", &samplesize, CONF_TYPE_INT,CONF_RANGE,1,8, NULL },
                                     { "format", &format, CONF_TYPE_INT, CONF_MIN, 0 , 0, NULL },
                                     {NULL, NULL, 0, 0, 0, 0, NULL}
                                 };


extern void resync_audio_stream(sh_audio_t *sh_audio);

int demux_rawaudio_open(demuxer_t* demuxer)
{
    sh_audio_t* sh_audio;
    WAVEFORMATEX* w;

    sh_audio = new_sh_audio(demuxer,0);
    sh_audio->wf = w = (WAVEFORMATEX*)malloc(sizeof(WAVEFORMATEX));
    w->wFormatTag = sh_audio->format = format;
    w->nChannels = sh_audio->channels = channels;
    w->nSamplesPerSec = sh_audio->samplerate = samplerate;
    w->nAvgBytesPerSec = samplerate*samplesize*channels;
    w->nBlockAlign = channels*samplesize;
    sh_audio->samplesize = samplesize;
    w->wBitsPerSample = 8*samplesize;
    w->cbSize = 0;

    demuxer->movi_start = demuxer->stream->start_pos;
    demuxer->movi_end = demuxer->stream->end_pos;

    demuxer->audio->sh = sh_audio;
    sh_audio->ds = demuxer->audio;

    return 1;
}

int demux_rawaudio_fill_buffer(demuxer_t* demuxer, demux_stream_t *ds)
{
    sh_audio_t* sh_audio = demuxer->audio->sh;
    int l = sh_audio->wf->nAvgBytesPerSec;
    off_t spos = stream_tell(demuxer->stream);
    demux_packet_t*  dp;

    if(demuxer->stream->eof)
        return 0;

    dp = new_demux_packet(l);
    dp->pts = (spos - demuxer->movi_start)  / (float)(sh_audio->wf->nAvgBytesPerSec);
    dp->pos = (spos - demuxer->movi_start);

    stream_read(demuxer->stream,dp->buffer,l);
    ds_add_packet(ds,dp);

    return 1;
}

void demux_rawaudio_seek(demuxer_t *demuxer,float rel_seek_secs,int flags)
{
    stream_t* s = demuxer->stream;
    sh_audio_t* sh_audio = demuxer->audio->sh;
    off_t base,pos;

    base = (flags & 1) ? demuxer->movi_start : stream_tell(s);
    if(flags & 2)
        pos = base + ((demuxer->movi_end - demuxer->movi_start)*rel_seek_secs);
    else
        pos = base + (rel_seek_secs*sh_audio->i_bps);

    pos -= (pos % (sh_audio->channels * sh_audio->samplesize) );
    stream_seek(s,pos);
    sh_audio->delay= (pos-ds_tell_pts(demuxer->audio)-sh_audio->a_in_buffer_len) / (float)(sh_audio->wf->nAvgBytesPerSec);
    resync_audio_stream(sh_audio);
    //  printf("demux_rawaudio: streamtell=%d\n",(int)stream_tell(demuxer->stream));
}
