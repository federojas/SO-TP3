CC= gcc
GCCFLAGS= -std=gnu99  -Wall -pedantic 
GCCLIBS= -lm

SOURCE_SERVER= serverDOG.c
SOURCE_CLIENT= client.c
SOURCE_LEVELS= levels.c

OBJECT_LEVELS=$(SOURCE_LEVELS:.c=.o)

EXECUTABLE_SERVER= serverDOG
EXECUTABLE_CLIENT= client

all: $(OBJECT_LEVELS) client
	$(CC) $(GCCFLAGS) $(SOURCE_SERVER) $(OBJECT_LEVELS) -I./include -o $(EXECUTABLE_SERVER) $(GCCLIBS)
	touch .RUN_ME
	objcopy --add-section .RUN_ME=.RUN_ME  serverDOG
	rm .RUN_ME

%.o: %.c
	$(CC) $(GCCFLAGS) -I./include -c $^ $(GCCLIBS)

client:
	$(CC) $(GCCFLAGS) $(SOURCE_CLIENT) -o $(EXECUTABLE_CLIENT)

clean:
	rm -rf *.o serverDOG client

cleanTest: rm -rf output.cppOut report.tasks results.valgrind;

test: ./pvs.sh;
	cppcheck --quiet --enable=all --force --inconclusive serverDOG.c levels.c client.c 2>> output.cppOut;

.PHONY: all clean cleanTest test client serverDOG server