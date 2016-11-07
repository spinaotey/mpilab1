COMPILER        =       mpicc
CFLAGS          =       
LFLAGS          =       -lmpi
OBJECTS         =       darts.o
INCLUDES        =       

darts:           $(OBJECTS)
		$(COMPILER) $(CFLAGS) -o darts $(OBJECTS) $(LFLAGS)

darts.o: 	$(INCLUDES) darts.c
		$(COMPILER) $(CFLAGS) -c darts.c $(LFLAGS)

clean:
		rm -f *.o *~

realclean:	clean
		rm -f darts

