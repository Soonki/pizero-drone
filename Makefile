CC			=	arm-linux-gnueabihf-gcc
CFLAGS		+=	-Wall
LDFLAGS		+=	


###############################################################################
# USER CONFIG #
###############################################################################

SOURCES 	+= main1.h 

###############################################################################
# DEFAULT CONFIG #
###############################################################################

VPATH		:= 	$(dir $(shell find src/ -name '*.c') $(shell find src/ -name '*.h')) src/init
SOURCES 	:= 	$(notdir $(shell find src/ -name '*.c') $(wildcard $(INCLUDES)/*.h))
INCLUDES	:=	$(foreach d, $(wildcard src/*/includes), -I$d) -Iconfig
EXECUTABLE	?=	cpidrone.elf

LIB_MODULES	:=	wiringPi
INCLUDES	+=	$(foreach m, $(LIB_MODULES), -Ilib/$m/include)
LIB_DIRS	+=	$(foreach m, $(LIB_MODULES), -Llib/$m/lib)
LDFLAGS		+=	-lm -lpthread $(foreach m, $(LIB_MODULES), -l$m)


OBJ			:=	$(patsubst %.c, build/%.o, $(SOURCES))

###############################################################################
# TARGETS #
###############################################################################


all: $(SOURCES) $(EXECUTABLE)

compile: $(SOURCES) $(OBJ)

$(EXECUTABLE): $(OBJ)
	@echo "Building $(EXECUTABLE)"
	@$(CC) $(OBJ) -o bin/$@ $(LIB_DIRS) $(LDFLAGS)
	@echo "DONE!"

build/%.o: %.c
	@mkdir -p $(@D)
	@echo "Compiling $@"
	@$(CC) $(CFLAGS) -c $< -o $@ $(INCLUDES)

clean_o:
	@echo "Removing .o files"
	@rm -rf build/*.o

clean: clean_o
	@echo "Removing .elf files"
	@rm -rf bin/*.elf

test:
	@echo "SRC: $(SOURCES)"
	@echo "OBJ: $(OBJ)"
	@echo "INC: $(INCLUDES)"