PREFIX		=	arm-linux-gnueabihf-
CC			=	gcc
CFLAGS		+=	-Wall
LDFLAGS		+=	-lm

###############################################################################
# USER CONFIG #
###############################################################################

SOURCES 	+= main1.h 

###############################################################################
# DEFAULT CONFIG #
###############################################################################

VPATH		:= 	$(dir $(shell find src/ -name '*.c') $(shell find src/ -name '*.h')) src/init
SOURCES 	:= 	$(notdir $(shell find src/ -name '*.c'))
INCLUDES	:=	$(foreach d, $(wildcard src/*/includes), -I$d)
INCLUDES	+= 	-Iconfig
OBJ			:=	$(patsubst %.c, build/%.o, $(SOURCES))
SOURCES		+= $(notdir $(wildcard $(INCLUDES)/*.h) )
EXECUTABLE	?=	cpidrone.elf


###############################################################################
# TARGETS #
###############################################################################


all: $(SOURCES) $(EXECUTABLE)

compile: $(SOURCES) $(OBJ)

$(EXECUTABLE): $(OBJ)
	$(CC) $(OBJ) -o bin/$@ $(LDFLAGS)

build/%.o: %.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@ $(INCLUDES)

clean_o:
	rm -rf build/*.o

clean: clean_o
	rm -rf bin/*.elf

test:
	@echo "SRC: $(SOURCES)"
	@echo "OBJ: $(OBJ)"
	@echo "INC: $(INCLUDES)"