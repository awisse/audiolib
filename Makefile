LDLIBS=-lpulse -lpulse-simple
RM=rm -f
objects=audio.o
playsound: $(objects)
	$(CC) -o playsound $(objects) $(LDLIBS)

.PHONY : clean
clean :
	$(RM) playsound $(objects)
