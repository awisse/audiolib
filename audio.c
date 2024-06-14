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
#define _XOPEN_SOURCE

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
 
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
 
#include <pulse/simple.h>
#include <pulse/error.h>
 
#define LOOPS 1
#define BUFSIZE 0x1000

void usage() {
  /* Print usage instructions */
  puts("Usage: playsound [Options] [filename]");
  puts("  --header: Print file header (.wav)");
  exit(0);
}

 
int play(const char* filename, pa_simple* s, const pa_sample_spec ss) {
  int error;
  int fd;

  /* Rewind the audio file */
  if ((fd = open(filename, O_RDONLY)) < 0) {
    fprintf(stderr, __FILE__": open() failed: %s\n", strerror(errno));
    return -1;
  }

  for (;;) {
    uint8_t buf[BUFSIZE];
    ssize_t r;
 
#if 0
    pa_usec_t latency;

    if ((latency = pa_simple_get_latency(s, &error)) == (pa_usec_t) -1) {
      fprintf(stderr, __FILE__": pa_simple_get_latency() failed: %s\n", pa_strerror(error));
      goto finish;
    }

    fprintf(stderr, "%0.0f usec    \r", (float)latency);
#endif
 
    /* Read some data ... */
    if ((r = read(fd, buf, sizeof(buf))) <= 0) {
      if (r == 0) /* EOF */
        break;

      fprintf(stderr, __FILE__": read() failed: %s\n", strerror(errno));
      return -1;
    }

    printf("%ld bytes read\n", r);
 
    /* ... and play it */
    if (pa_simple_write(s, buf, (size_t) r, &error) < 0) {
      fprintf(stderr, __FILE__": pa_simple_write() failed: %s\n", pa_strerror(error));
      return -1;
    }
  }
 
  /* Make sure that every single sample was played */
  if (pa_simple_drain(s, &error) < 0) {
    fprintf(stderr, __FILE__": pa_simple_drain() failed: %s\n", pa_strerror(error));
    return -1;
  }

  return 0;

}

int main(int argc, char* argv[]) {
 

  /* The Sample format to use */
  static const pa_sample_spec ss = {
    .format = PA_SAMPLE_S16LE,
    .rate = 44100,
    .channels = 2
  };
 
  pa_simple *s = NULL;
  int ret = 1;
  int error;

  
 
  /* replace STDIN with the specified file if needed */
  if (argc < 1) {
    fprintf(stderr, "Usage: playsound <filename>\n");
    return 1;
  }
 
  /* Create a new playback stream */
  if (!(s = pa_simple_new(NULL, argv[0], PA_STREAM_PLAYBACK, NULL, "playback", &ss, 
          NULL, NULL, &error))) {
    fprintf(stderr, __FILE__": pa_simple_new() failed: %s\n", pa_strerror(error));
    goto finish;
  }
 
  for (int i=0; i < LOOPS; i++)
    if (play(argv[1], s, ss))
    goto finish;
 
  ret = 0;
 
finish:
 
  if (s)
    pa_simple_free(s);
 
  return ret;
}

