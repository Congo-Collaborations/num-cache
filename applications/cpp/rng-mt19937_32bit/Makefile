#for GNU make

#DDEBUG = -O0 -g -ggdb -DDEBUG=1

CC = g++ -std=gnu++0x -U__STRICT_ANSI__ $(DDEBUG)
# CC = g++ -Wall -O3 -std=c99 -Wmissing-prototypes $(DDEBUG)
#CC = icc -Wall -O3 -std=c99 -Wmissing-prototypes $(DDEBUG)

all:  run

run:  run.c mt32.o
	${CC} -o $@  ../../../cpp/numb/sha256.cpp ../../../cpp/numb/numb.cpp run.c mt32.o ${LINKOPT}

.c.o:
	${CC} -c $<

clean:
	rm -rf run *.o *~