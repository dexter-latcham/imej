CC=gcc
SRC_DIR=src
INCLUDE_DIR=include
OBJ_DIR=obj

SRC_FILES=project.c translation.c vector.c matrix.c raster.c object.c scene.c camera.c
INCLUDE=-I $(INCLUDE_DIR)
CFLAGS=-Wall
LIBS=-lm
X11_LIBS= -lX11 -lpthread
DEFIMES=-Dbeep

IMEJ_LIST?=mixed
ifeq ($(IMEJ_LIST),linked)
	SRC_FILES+=objectListLinked.c
else ifeq ($(IMEJ_LIST),array)
	SRC_FILES+=objectListArray.c
else ifeq ($(IMEJ_LIST),mixed)
	SRC_FILES+=objectListMixed.c
endif

IMEJ_DISPLAY?=x11
ifeq ($(IMEJ_DISPLAY),x11)
	SRC_FILES+=x11Window.c
	LIBS+=$(X11_LIBS)
else ifeq ($(IMEJ_DISPLAY),ppm)
	SRC_FILES+=ppm.c
else ifeq ($(IMEJ_DISPLAY),term)
	SRC_FILES+=terminal.c
endif

IMEJ_INPUT?=false
ifeq ($(IMEJ_INPUT),true)
	SRC_FILES+=input.c
	DEFINES+=-DIMEJ_DO_INPUT
endif

IMEJ_PROJECTION?=perspective
ifeq ($(IMEJ_PROJECTION),orthagonal)
	DEFINES+=-DIMEJ_ORTHAGONAL
endif

IMEJ_COLOUR?=IMEJ_32_BIT_COLOUR
ifeq ($(IMEJ_COLOUR),16)
	DEFINES+=-DIMEJ_16_BIT_COLOUR
else
	DEFINES+=-DIMEJ_32_BIT_COLOUR
endif

OBJ_FILES=$(patsubst %.c,$(OBJ_DIR)/%.o,$(SRC_FILES))

$(shell mkdir -p $(OBJ_DIR))

imej: clean $(OBJ_FILES)
	ar rcs imej.a $(OBJ_DIR)/*.o

link: clean imej
ifndef FILENAME
	@echo "please specify a demo to compile"
	@exit 1
endif
	@$(CC) -g $(INCLUDE) $(FILENAME) -D$(IMEJ_COLOUR) -o imej $(LIBS) -L. -l:imej.a -O3

clean:
	rm -f imej $(OBJ_DIR)/*.o imej.a

test: clean imej
	@$(CC) tests/testRunner.c -o unitTest $(LIBS) -L. -l:imej.a -lcheck
	@if [ $$? -eq 0 ]; then \
		./unitTest \
	else \
		echo "failed to compile unit tests"; \
	fi

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c 
	@$(CC) -c -g $< -o $@ $(CFLAGS) $(INCLUDE) $(DEFINES) -O3
