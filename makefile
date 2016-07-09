# The recommended compiler flags for the Raspberry Pi
CCFLAGS=-Ofast -mfpu=vfp -mfloat-abi=hard -march=armv6zk -mtune=arm1176jzf-s -std=c++11 
#CCFLAGS=

# define all programs
#PROGRAMS = scanner pingtest gettingstarted
PROGRAM = main
NAME = domomain
SOURCE = ${PROGRAM:=.cpp}

all: main

${PROGRAM}: ${SOURCE} 
	g++ ${CCFLAGS} -Wall -I../ -lrf24-bcm $@.cpp -o $(NAME)


clean:
	rm -rf $(NAME)

