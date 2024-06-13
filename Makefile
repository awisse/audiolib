LDLIBS=-lpulse -lpulse-simple
RM=rm -f
objects=audio.o
playsound: $(objects)
	$(CC) -o playsound $(objects) $(LDLIBS)

.PHONY : install
install :
	cp -p playsound $(HOME)/bin
.PHONY : clean
clean :
	$(RM) playsound $(objects)
