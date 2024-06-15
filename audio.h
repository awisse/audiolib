#ifndef _AUDIO_H_
/* 
 * audio.c header
 */
#define _XOPEN_SOURCE
 
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>

#include <pulse/simple.h>
#include <pulse/error.h>

#define APPLICATION "playsound"

struct wavhdr {
  // Master RIFF chunk
  char        FileTypeBlocID[4];
  int         FileSize;
  char        FileFormatID[4];

  // Chunk describing the data format
  char        FormatBlocID[4];
  int         BlocSize;
  short       AudioFormat;
  short       NbrChannels;
  int         Frequence;
  int         BytePerSec;
  short       BytePerBloc;
  short       BitsPerSample;

  // Chunk containing the sampled data
  char        DataBlocID[4];
  int         DataSize;
};

int play(struct wavhdr *hdr, FILE* f);

#endif
