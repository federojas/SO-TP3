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
	rm -rf *.o serverDOG client levels

cleantest: 
	rm -rf output.cppout report.tasks

test:
	pvs-studio-analyzer trace -- make;
	pvs-studio-analyzer analyze;
	plog-converter -a '64:1,2,3;GA:1,2,3;OP:1,2,3' -t tasklist -o report.tasks PVS-Studio.log;
	rm -f strace_out;
	rm -f PVS-Studio.log;
	cppcheck --quiet --enable=all --force --inconclusive serverDOG.c client.c levels.c  2>> output.cppout;

.PHONY: all clean cleantest test client serverDOG server