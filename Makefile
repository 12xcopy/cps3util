CC ?= gcc

FLAGS ?= -std=gnu99 -Wall -Wextra
FLAGS += -Wno-unused-function -g

SRC = source/main.c source/vfs.c source/simm.c

LIBS = -lzip

all:
	$(CC) $(FLAGS) $(SRC) -o cpsutil $(LIBS)

clean:
	rm cpsutil