/***
  This file is part of PulseAudio.
 
  PulseAudio is free software; you can redistribute it and/or modify
  it under the terms of the GNU Lesser General Public License as published
  by the Free Software Foundation; either version 2.1 of the License,
  or (at your option) any later version.
 
  PulseAudio is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
  General Public License for more details.
 
  You should have received a copy of the GNU Lesser General Public License
  along with PulseAudio; if not, see <http://www.gnu.org/licenses/>.
***/
#include "audio.h"
 
#define BUFSIZE 0x1000

int play(struct wavhdr *hdr, FILE* f) {
  int error;
  int retval = 0; // Return value of function

  // PulseAudio variables
  pa_sample_spec ss;
  pa_simple *s;

  /* The Sample format to use */
  ss.rate = hdr->Frequence;
  ss.channels = hdr->NbrChannels;
  switch (hdr->BitsPerSample) {
    case 8:
      ss.format = PA_SAMPLE_U8;
      break;
    case 16:
      ss.format = PA_SAMPLE_S16LE;
      break;
    case 32:
      ss.format = PA_SAMPLE_S32LE;
      break;
    default:
      fprintf(stderr, "%d BitsPerSample not supported\n", hdr->BitsPerSample);
      return -1;
  }

  /* Create a new playback stream */
  if (!(s = pa_simple_new(NULL, APPLICATION, PA_STREAM_PLAYBACK, NULL, "playback", &ss, 
          NULL, NULL, &error))) {
    fprintf(stderr, __FILE__": pa_simple_new() failed: %s\n", pa_strerror(error));
    return -1;
  }

  for (;;) {
    uint8_t buf[BUFSIZE];
    ssize_t r;
 
    /* Read some data ... */
    if ((r = fread(buf, 1, sizeof(buf), f)) <= 0) {
      if (r == 0) /* EOF */
        break;

      fprintf(stderr, __FILE__": read() failed: %s\n", strerror(errno));
      retval = -1;
      break;
    }

    /* ... and play it */
    if (pa_simple_write(s, buf, (size_t) r, &error) < 0) {
      fprintf(stderr, __FILE__": pa_simple_write() failed: %s\n", pa_strerror(error));
      retval = -1;
      break;
    }
  }
 
  /* Make sure that every single sample was played */
  if ((retval == 0) && (pa_simple_drain(s, &error) < 0)) {
    fprintf(stderr, __FILE__": pa_simple_drain() failed: %s\n", pa_strerror(error));
    retval = -1;
  }

  pa_simple_free(s);

  return retval;
}
