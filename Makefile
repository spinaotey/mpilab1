COMPILER        =       mpicc
CFLAGS          =       
LFLAGS          =       -lmpi
OBJECTS         =       darts.o
OBJECTS2        =       darts2.o
INCLUDES        =       

darts:           $(OBJECTS)
		$(COMPILER) $(CFLAGS) -o darts $(OBJECTS) $(LFLAGS)

darts.o: 	$(INCLUDES) darts.c
		$(COMPILER) $(CFLAGS) -c darts.c $(LFLAGS)

darts2:           $(OBJECTS2)
		$(COMPILER) $(CFLAGS) -o darts2 $(OBJECTS2) $(LFLAGS)

darts2.o: 	$(INCLUDES) darts2.c
		$(COMPILER) $(CFLAGS) -c darts2.c $(LFLAGS)

clean:
		rm -f *.o *~

realclean:	clean
		rm -f darts darts2
