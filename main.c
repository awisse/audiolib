/*
 * A simple command line program to play .wav uncompressed audio files.
 * I might add .mp3 later.
 */
#include "audio.h"

#define FILE_TYPE_BLOC_ID "RIFF"
#define FILE_FORMAT_ID "WAVE"
#define FORMAT_BLOC_ID "fmt "
#define DATA_BLOC_ID "data"

void usage();

int get_wav_header(FILE* f, struct wavhdr *hdr);
void print_wav_header(struct wavhdr *hdr);

int main(int argc, char* argv[]) {

  int c; // char returned by getopt
  int perr; // Error code returned by "play"
  FILE *f; // audio (.wav) file
  struct wavhdr hdr; // Header of .wav file
  int show_hdr = 0; // Flag


  while ((c = getopt(argc, argv, "hw")) != -1) {
    switch (c) {
      case 'h':
      case '?':
        usage();
        return 0;
      case 'w':
        show_hdr = 1;
        break;
    }
  }

  f = fopen(argv[optind], "r");

  if (get_wav_header(f, &hdr) == -1) {
    fprintf(stderr, ".wav header not recognized.");
    fclose(f);
    return 1;
  }

  if (show_hdr) {
    print_wav_header(&hdr);
  }

  if (hdr.AudioFormat != 1) {
    fprintf(stderr, "Audio Format (%hd) not supported\n", hdr.AudioFormat);
    fclose(f);
    return 1;
  }

  printf("Play %s\n", argv[optind]);

  perr = play(&hdr, f);

  fclose(f);

  if (perr != 0) {
    fprintf(stderr, "Error playing %s\n", argv[optind]);
    return 1;
  }
  return 0;
}


void usage() {
  /* Print usage instructions */
  puts("Usage: playsound [Options] filename");
  puts("  -w: Show fileheader (.wav)");
  puts("  -h: Show help");
}

int get_wav_header(FILE* f, struct wavhdr *hdr) {

  size_t bytes_read;

  bytes_read = fread(hdr, 1, sizeof(*hdr), f);
  
  if (bytes_read < sizeof(*hdr)) {
    return -1;
  }

  return 0;
}


void print_wav_header(struct wavhdr *header) {
  printf("FileTypeBlocID: %.4s\n",header->FileTypeBlocID);
  printf("FileSize:       %d\n",header->FileSize);
  printf("FileFormatID:   %.4s\n",header->FileFormatID);

  // Chunk describing the data format
  printf("FormatBlocID:   %.4s\n",header->FormatBlocID);
  printf("BlocSize:       %d\n",header->BlocSize);
  printf("AudioFormat:    %hd\n",header->AudioFormat);
  printf("NbrChannels:    %hd\n",header->NbrChannels);
  printf("Frequence:      %d\n",header->Frequence);
  printf("BytePerSec:     %d\n",header->BytePerSec);
  printf("BytePerBloc:    %hd\n",header->BytePerBloc);
  printf("BitsPerSample:  %hd\n",header->BitsPerSample);

  // Chunk containing the sampled data
  printf("DataBlocID:     %.4s\n",header->DataBlocID);
  printf("DataSize:       %d\n",header->DataSize);
}
