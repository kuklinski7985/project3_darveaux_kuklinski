SOURCES = main.c project2.c circbuff.c debug.c #uart.c 

CMOCKAS = unittest.c
OBJS = $(SOURCES:.c=.o)
IMP = $(SOURCES:.c=.i)

DEBUG = -g -Wall -Werror -std=c99 -O0
CPPFLAGS = -DPROJECT2 -DVERBOSE
LDFLAGS = -lm -Wl,-Map,project2.map
CFLAGS = -c
LFLAGS = -S

ifeq ($(PLATFORM),HOST)
	CC = gcc

endif

ifeq ($(PLATFORM),BBB)
	CC = arm-linux-gnueabihf-gcc

endif

ifeq ($(PLATFORM),KL25Z)
	CC = arm-none-eabi-gcc 
	PLATFORM_FLAGS = -march=armv6 \
		 -mtune=cortex-m0plus\
		 -mthumb \
		 -mfloat-abi=softfp \
		 -mfpu=fpv4-sp-d16 \
		 --specs=nosys.specs

endif

%.o:%.c
	$(CC) $(DEBUG) $(CPPFLAGS) $(CFLAGS) -MMD $^ -o $@

%.o:%.c
	$(CC) $(DEBUG) $(CPPFLAGS) $(CFLAGS)  -MMD $^ -o $@


%.i:%.c
	$(CC) $(DEBUG) -E $(CPPFLAGS) $^ -o $@

%.asm:%.c

	$(CC) $(DEBUG) $(CPPFLAGS) $(CFLAGS) -S $< -o $@


.PHONY: compile-all
compile-all: $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS)  -o $@

.PHONY: build
build: $(OBJS)
	$(CC) $(DEBUG) $(OBJS) $(PLATFORM_FLAGS) $(LDFLAGS) -o project2.elf
	size project2.elf $(OBJS)

.PHONY: unittest
unittest: $(MOCKS)
	$(CC) $(CMOCKAS) -I -WI, -lcmocka -o $@ -lm

.PHONY: clean
clean:
	-rm *.i *.o *.map *.d project2.elf unittest *.out
