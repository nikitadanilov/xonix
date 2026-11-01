# Makefile for XONIX

###########################################################################
#                                                                         #
# ###########################################################################
# #                                                                         #
# # User-visible targets:                                                   #
# #                                                                         #
###                shared library from it                                   #
  #                                                                         #
  ###########################################################################


###########################################################################
#                                                                         #
# ###########################################################################
# #                                                                         #
# #  Compiler settings.                                                     #
# #  Tune for your system.                                                  #
###                                                                         #
  #                                                                         #
  ###########################################################################

###########################################################################
#                                                                         #
# ###########################################################################
# #                                                                         #
# #  GCC                                                                    #
###                                                                         #
  #                                                                         #
  ###########################################################################

QHOME?=$(HOME)/p/quercus/quercus-0.0.4

CC=gcc
CPP=g++
DEFINES=#-D_REENTRANT #-D_THREAD_SAFE -D__USE_REENTRANT
COPT=-pthread -std=gnu++11 -fpermissive #-fno-inline-functions -fno-vtable-thunks -fkeep-inline-functions 
PROFOPT=-g #-pg
LIBS=-L/usr/X11/lib -L$(LIB_DIR) -L$(QHOME)/lib \
	-lm -lXpm -lX11 -ldl -lreadline -lncurses #-lgc # -lposix4 -lQRBase -lQRExceptions	-lQRIter -lQRContainers	-lQRContainersMisc -lQRList -lQRObserve -lQRAlgorithms -lQRPlatform -lQRThreads -lQRXq -lquercus
SHARED_COPT=-fPIC -shared
SHARED_LDOPT=-shared -dy
CFLAGS=$(COPT) $(PROFOPT) -pipe -w
#CFLAGS=$(COPT) $(PROFOPT) -pipe -W -Wall -Wshadow -Wpointer-arith -Wwrite-strings -Wconversion -Wstrict-prototypes -Wmissing-prototypes -Woverloaded-virtual -Winline -Wformat -Wconversion -Wsynth -ansi -pedantic #-Wcast-qual -Wcast-align -Wenum-clash -Waggregate-return -Wredundant-decls -Weffc++  -Wtraditional -Wid-clash-24 -Wmissing-declarations -Wnested-externs -Wbad-function-cast
LDFLAGS=$(PROFOPT) #-static

###########################################################################
#                                                                         #
# ###########################################################################
# #                                                                         #
# #  SUN Workshop compiler (SUNWspro), Solaris, SPARC Edition               #
###                                                                         #
  #                                                                         #
  ###########################################################################

#CC=cc
#CPP=CC
#DEFINES=-D_REENTRANT
#COPT=#-fast -xchip=ultra -xlibmil -xunroll=3
#PROFOPT=-g #-xpg #-xprofile=collect -Ztha #-g
#LIBS=-L/usr/X11/lib -L$(LIB_DIR) -L../quercus-0.0.2/lib $(LIB_DIR)/quercus.o \
#	-lbfd -liberty -lposix4 -lpthread -lX11 -lm -lXpm #-lgc # -lposix4 -lQRBase -lQRExceptions	-lQRIter -lQRContainers	-lQRContainersMisc -lQRList -lQRObserve -lQRAlgorithms -lQRPlatform -lQRThreads -lQRXq -lquercus
#SHARED_COPT=-PIC -G 
#SHARED_LDOPT=-G
#CFLAGS=$(COPT) $(PROFOPT) -features=rtti +w -ptr$(OBJ_DIR) -mt -instances=static#-pta 
#LDFLAGS=$(PROFOPT) -ptr$(OBJ_DIR) -mt #-xildoff 
#ADDITIONAL_INCS=-I/usr/local/include

###########################################################################
#                                                                         #
# ###########################################################################
# #                                                                         #
# #  Programs and tools.                                                    #
###                                                                         #
  #                                                                         #
  ###########################################################################

ETAGS=etags
DOC_GENERATOR_HTML=doc++ -A -H -d 
DOC_GENERATOR_TEX=doc++ -A -t -o 
SHELL=bash
DATE := $(shell env LANG=C date "+%a %b %e %T %Z %Y")
HOST := $(shell uname -n)
USR  := philogelos #$(shell /usr/xpg4/bin/id -un)

###########################################################################
#                                                                         #
# ###########################################################################
# #                                                                         #
# #  Source tree structure                                                  #
###                                                                         #
  #                                                                         #
  ###########################################################################

SRC_DIR=./src
INC_DIR=./inc
OBJ_DIR=./obj
LIB_DIR=./lib
DOC_DIR=./doc
APP_NAME=xonix
VARIANTS=x11 curses
JUNK_FILES=*.o *.d *.so core $(VARIANTS:%=$(APP_NAME).%) *~ \\\#*\\\#
BUILD_INFO_FILE=$(INC_DIR)/build.h
BUILD_TAG_FILE=.build


###########################################################################
#                                                                         #
# ###########################################################################
# #                                                                         #
# #  Your should not be interested in anything below this line              #
###                                                                         #
  #                                                                         #
  ###########################################################################

###########################################################################
#                                                                         #
# ###########################################################################
# #                                                                         #
# #   Make misc.                                                            #
###                                                                         #
  #                                                                         #
  ###########################################################################

.DONE:
	@echo Top Makefile done!

.FAILED:
	@echo Top Makefile failed.

.INIT:	
	@echo Begin Top Makefile

.SUFFIXES:
.SUFFIXES:	.c .h .cpp .hpp .ipp .tpp $(SUFFIXES)

###########################################################################
#                                                                         #
# ###########################################################################
# #                                                                         #
# #  Sources.                                                               #
###                                                                         #
  #                                                                         #
  ###########################################################################

###########################################################################
#                                                                         #
# ###########################################################################
# #                                                                         #
# #   Headers.                                                              #
###                                                                         #
  #                                                                         #
  ###########################################################################

PACKAGES=.

# header files
CPP_INC= \
	Game.hpp \
	Board.hpp \
	Cell.hpp \
	BoardMap.hpp \
	ACursor.hpp \
	FrameFeeder.hpp \
	BombThread.hpp \
	Keyboard.hpp \
	Player.hpp \
	IBombThread.hpp \
	OBombThread.hpp \
	XBombThread.hpp \
	SBombThread.hpp \
	StatusLine.hpp \
	SClipper.hpp \
	Geom.hpp \
	XGlyph.hpp \
	FontGlyph.hpp \
	PixmapGlyph.hpp \
	BitmapGlyph.hpp \
	AnimGlyph.hpp \
	GameObject.hpp \
	GameThread.hpp \
	GameConfig.hpp \
	GameManager.hpp \
	GameScheduler.hpp \
	Option.hpp \
	TopFile.hpp

#	Clipper.hpp \

###########################################################################
#                                                                         #
# ###########################################################################
# #                                                                         #
# #   C++ classes implementations.                                          #
###                                                                         #
  #                                                                         #
  ###########################################################################

# C++ sources --- each corresponds to object file
CPP_SRC= \
	Game.cpp \
	Board.cpp \
	Cell.cpp \
	BoardMap.cpp \
	ACursor.cpp \
	FrameFeeder.cpp \
	BombThread.cpp \
	Keyboard.cpp \
	Player.cpp \
	IBombThread.cpp \
	OBombThread.cpp \
	XBombThread.cpp \
	SBombThread.cpp \
	StatusLine.cpp \
	SClipper.cpp \
	Geom.cpp \
	FontGlyph.cpp \
	PixmapGlyph.cpp \
	BitmapGlyph.cpp \
	AnimGlyph.cpp \
	GameObject.cpp \
	GameThread.cpp \
	GameConfig.cpp \
	GameManager.cpp \
	GameScheduler.cpp \
	Option.cpp \
	TopFile.cpp

#	Clipper.cpp \

###########################################################################
#                                                                         #
# ###########################################################################
# #                                                                         #
# #   Derived from file lists.                                              #
###                                                                         #
  #                                                                         #
  ###########################################################################

INCS=-I. -I$(INC_DIR) $(ADDITIONAL_INCS) \
	-I$(QHOME)/inc -I$(QHOME)
ALL_SOURCE=$(CPP_SRC) $(CPP_IPP) $(CPP_TPP)
CPP_OBJ=$(CPP_SRC:.cpp=.o)
CPP_SOBJ=$(CPP_SRC:.cpp=.so)

INC=$(CPP_INC:%=$(INC_DIR)/%)
SRC=$(ALL_SOURCE:%=$(SRC_DIR)/%)
OBJ=$(CPP_OBJ:%=$(OBJ_DIR)/%)
# Each variant is compiled into its own object directory.
X11_OBJ=$(CPP_OBJ:%=$(OBJ_DIR)/x11/%) $(OBJ_DIR)/x11/main.o
CURSES_OBJ=$(CPP_OBJ:%=$(OBJ_DIR)/curses/%) $(OBJ_DIR)/curses/main.o
SOBJ=$(CPP_SOBJ:%=$(LIB_DIR)/%)


###########################################################################
#                                                                         #
# ###########################################################################
# #                                                                         #
# #  User-visible targets.                                                  #
###                                                                         #
  #                                                                         #
  ###########################################################################

all: $(VARIANTS:%=$(APP_NAME).%)

$(APP_NAME).x11 : $(X11_OBJ) $(OBJ_DIR)/quercus.o
	$(CPP) $(LDFLAGS) -o $@ $(CFLAGS) $(X11_OBJ) $(OBJ_DIR)/quercus.o $(LIBS)

$(APP_NAME).curses : $(CURSES_OBJ) $(OBJ_DIR)/quercus.o
	$(CPP) $(LDFLAGS) -o $@ $(CFLAGS) $(CURSES_OBJ) $(OBJ_DIR)/quercus.o $(LIBS)

$(OBJ_DIR)/quercus.o:
	$(CPP) -o $(OBJ_DIR)/quercus.o $(INCS) -I$(QHOME)/src $(CFLAGS) $(DEFINES) -c $(QHOME)/src/all-in-one.cpp

clean:
	for dir in `find . \( -path ./old-build -o -path ./.git \) -prune -o -type d -print` ;do ( cd $$dir ; rm -f $(JUNK_FILES) ) ;done

# Header dependencies are generated by the compiler (-MMD) into
# $(OBJ_DIR)/<variant>/*.d; there is no separate depend step.

tags:
	$(ETAGS) -l c++ -C -o TAGS $(SRC) $(INC)

archive: 
	LANG=C ; tar cf - $(SRC_DIR) $(INC_DIR) $(DOC_DIR) img/ Makefile .build | gzip -9c > ../../archive/xonix-`date "+%Y-%m-%d-%H-%M"`.tar.gz

touch:
	find . | xargs touch

makedirs:
	for type in $(SRC_DIR) $(INC_DIR) $(OBJ_DIR) $(LIB_DIR) $(DOC_DIR) ;do for package in $(PACKAGES) ;do mkdir -p $$type/$$package;	done ; done

final: $(INC_DIR)/xonix.hpp $(INC_DIR)/xonix-src.hpp
	$(CPP) $(INCS) -I../quercus-0.0.2 -I../quercus-0.0.2/inc -I../quercus-0.0.2/src $(ADDITIONAL_INCS) -I$(SRC_DIR) $(CFLAGS) src/all-in-one.cpp

##########################################################################
$(OBJ_DIR)/%.o:	$(SRC_DIR)/%.c
	$(CC) $(INCS) $(CFLAGS) $(DEFINES) -c -o $@ $<

$(LIB_DIR)/%.so: $(SRC_DIR)/%.cpp
	$(CPP) $(INCS) $(CFLAGS) $(DEFINES) $(SHARED_COPT) -c -o $@ $<

$(OBJ_DIR)/%.o:	$(SRC_DIR)/%.cpp
	$(CPP) $(INCS) $(CFLAGS) $(DEFINES) -c -o $@ $<

$(OBJ_DIR)/x11/%.o:	$(SRC_DIR)/%.cpp
	@mkdir -p $(@D)
	$(CPP) $(INCS) $(CFLAGS) $(DEFINES) -DXONIX_USE_XQ -MMD -MP -c -o $@ $<

$(OBJ_DIR)/curses/%.o:	$(SRC_DIR)/%.cpp
	@mkdir -p $(@D)
	$(CPP) $(INCS) $(CFLAGS) $(DEFINES) -DXONIX_USE_CQ -MMD -MP -c -o $@ $<

$(OBJ_DIR)/%.o:	$(SRC_DIR)/%.ipp
	$(CPP) $(INCS) $(CFLAGS) $(DEFINES) -c -o $@ $<

$(OBJ_DIR)/%.o:	$(SRC_DIR)/%.tpp
	$(CPP) $(INCS) $(CFLAGS) $(DEFINES) -c -o $@ $<

##########################################################################
$(INC_DIR)/xonix.hpp: Makefile $(INC_DIR)/xonix.hpp.in
	cp $(INC_DIR)/xonix.hpp.in $(INC_DIR)/xonix.hpp
	for include in $(CPP_INC) ;do echo '#include <'$$include'>' ;done >> $(INC_DIR)/xonix.hpp
	echo '#endif' >> $(INC_DIR)/xonix.hpp

$(INC_DIR)/xonix-src.hpp: Makefile
	cp /dev/null $(INC_DIR)/xonix-src.hpp
	for include in $(CPP_SRC) ;do echo '#include <'$$include'>' ;done >> $(INC_DIR)/xonix-src.hpp

##########################################################################

-include $(VARIANTS:%=$(OBJ_DIR)/%/*.d)
