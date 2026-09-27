CC ?= gcc

CFLAGS ?= -std=gnu99 -Wall -Wextra
CFLAGS += -Wno-unused-function -Wno-stringop-truncation

SRC = source/main.c source/vfs.c source/simm.c

DEBUG ?= 0
ifeq ($(DEBUG),1)
	CFLAGS += -fsanitize=address -g3
else
	CFLAGS += -O2
endif

LIBS = -lzip

all:
	$(CC) $(CFLAGS) $(SRC) -o cpsutil $(LIBS)

clean:
	rm cpsutil