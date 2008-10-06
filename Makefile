
# ------------------ Compilation options ------------------------

# Loads math library.  
LIBS = -lm

# Flags for the C compiler:
#   -Wall for strict gcc warnings (requires prototypes for all functions).
#   -g to produce debug data for gdb
#   -O for optimization
CFLAGS = -Wall -g

CC = gcc

# --------------------- Code modules ----------------------------

# Object files
OBJ = src/match.o src/util.o

# Definitions
DEFS = src/defs.h

CVLIBS = -I /usr/local/include/opencv -L /usr/local/lib -lm -lcv -lhighgui -lcvaux

# ------------------------ Rules --------------------------------

match: ${OBJ}
	${CC} -o bin/$@ ${CFLAGS} ${OBJ} ${LIBS}

createkeys: src/createkeys.cpp
	g++ src/$@.cpp -o bin/$@ ${CFLAGS} ${CVLIBS}

clean:
	-rm src/*.o

# Implicit rule used by Gnu Make: $(CC) -c $(CPPFLAGS) $(CFLAGS)
${OBJ}: ${DEFS}
