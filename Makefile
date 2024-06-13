LDLIBS=-lpulse -lpulse-simple
RM=rm -f
SOURCES=audio.c
playsound: $(SOURCES)
	$(CC) -o playsound $^ $(LDLIBS)

.PHONY : install
install :
	cp -p playsound $(HOME)/bin
.PHONY : clean
clean :
	$(RM) playsound 
