LDLIBS=-lpulse -lpulse-simple
CFLAGS=
headers=audio.h
sources=main.c audio.c

all: playsound

debug: CFLAGS += -g
debug: playsound

playsound: $(sources) $(headers)
	$(CC) -o playsound $^ $(CFLAGS) $(LDLIBS)

.PHONY : install
install :
	cp -p playsound $(HOME)/bin

.PHONY : clean
clean :
	$(RM) playsound 
