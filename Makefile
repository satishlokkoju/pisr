
# ------------------ Compilation options ------------------------

# Loads math library.  
LIBS = -lm

# Flags for the C compiler:
#   -Wall for strict gcc warnings (requires prototypes for all functions).
#   -g to produce debug data for gdb
#   -O for optimization
CFLAGS = -Wall -g

CC = g++

# --------------------- Code modules ----------------------------

# Object files
OBJ = util.o common.o

# Definitions
DEFS = src/defs.h

CVLIBS = -I /usr/local/include/opencv -L /usr/local/lib -lm -lcv -lhighgui -lcvaux -lml

# ------------------------ Rules --------------------------------

all: match opencv createColorHists createEdgeVectors createkeys vocabtreematcher clean

match: util.o common.o match.o
	${CC} -o bin/$@ ${CFLAGS} match.o ${OBJ} ${LIBS} ${CVLIBS}

opencv: src/opencv.cpp
	g++ src/$@.cpp -o bin/$@ ${OBJ} ${CFLAGS} ${CVLIBS}

createColorHists: src/createColorHists.cpp
	g++ src/$@.cpp -o bin/$@ ${OBJ} ${CFLAGS} ${CVLIBS}

createEdgeVectors: src/createEdgeVectors.cpp
	g++ src/$@.cpp -o bin/$@ ${OBJ} ${CFLAGS} ${CVLIBS}

createkeys: src/createkeys.cpp
	g++ src/$@.cpp -o bin/$@ ${OBJ} ${CFLAGS} ${CVLIBS}

vocabtreematcher: src/vocabtreematcher.cpp
	g++ -o bin/$@ ${CFLAGS} ${OBJ} ${CVLIBS} src/vocabtreematcher.cpp src/vocsearch/ff_voctree.cpp src/vocsearch/ff_database.cpp src/vocsearch/ff_invfile.cpp

tmp: src/tmp.cpp util.o common.o match.o
	g++ ${OBJ} ${CFLAGS} ${CVLIBS} src/tmp.cpp

clean:
	-rm *.o

common.o: src/common.cpp src/defs.h
	g++ -c src/common.cpp ${CFLAGS} ${CVLIBS}

match.o: src/match.c src/defs.h
	g++ -c src/match.c ${CFLAGS} ${CVLIBS}

util.o: src/util.c src/defs.h
	g++ -c src/util.c ${CFLAGS} ${CVLIBS}
