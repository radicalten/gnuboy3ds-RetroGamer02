/*
 * sdl-audio.c
 * sdl audio interface
 *
 * (C) 2001 Laguna
 *
 * Licensed under the GPLv2, or later.
 */

#include <stdlib.h>
#include <stdio.h>

#include <SDL/SDL.h>

#include "rc.h"
#include "pcm.h"


struct pcm pcm;


static int sound = 1;
static int samplerate = 32728;
static int stereo = 1;
static volatile int audio_done;
static int paused;

rcvar_t pcm_exports[] =
{
	RCV_BOOL("sound", &sound, "enable sound"),
	RCV_INT("stereo", &stereo, "enable stereo"),
	RCV_INT("samplerate", &samplerate, "samplerate, recommended: 32768"),
	RCV_END
};


static void audio_callback(void *blah, byte *stream, int len)
{
	memcpy(stream, pcm.buf, len);
	audio_done = 1;
}


void pcm_init()
{
	int i;
	SDL_AudioSpec as = {0}, ob;

	if (!sound) return;

	SDL_InitSubSystem(SDL_INIT_AUDIO);
	as.freq = samplerate;
	as.format = AUDIO_S8;
	as.channels = 1 + stereo;
	as.samples = samplerate / 60;
	for (i = 1; i < as.samples; i<<=1);
	as.samples = i;
	as.callback = audio_callback;
	as.userdata = 0;
	if (SDL_OpenAudio(&as, &ob) == -1) {
		sound = 0;
		return;
	}

	pcm.hz = ob.freq;
	pcm.stereo = ob.channels - 1;
	pcm.len = ob.size;
	pcm.buf = malloc(pcm.len);
	pcm.pos = 0;
	memset(pcm.buf, 0, pcm.len);

	SDL_PauseAudio(0);
}

int pcm_submit()
{
	if (!pcm.buf || paused) {
		pcm.pos = 0;
		return 0;
	}
	if (pcm.pos < pcm.len) return 1;
	while (!audio_done)
		SDL_Delay(4);
	audio_done = 0;
	pcm.pos = 0;
	return 1;
}

void pcm_close()
{
	if (sound) SDL_CloseAudio();
}

void pcm_pause(int dopause)
{
	paused = dopause;
	SDL_PauseAudio(paused);
}




