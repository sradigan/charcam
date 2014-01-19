PRJNAME = CharCam
OBJS = BaseApplication.o CharCam.o
CC = g++
DEBUG = -g
CFLAGS = -Wall -c $(DEBUG)
CFLAGS += $(shell pkg-config --cflags OGRE OIS)
LFLAGS = -Wall $(DEBUG)
LFLAGS += $(shell pkg-config --libs OGRE OIS) -lboost_system
SRCFLDR = src
BINFLDR = bin
APP = $(shell echo $(PRJNAME) | tr '[:upper:]' '[:lower:]')

OgreApp : $(OBJS) install
	$(CC) $(LFLAGS) $(OBJS) -o $(BINFLDR)/$(APP)

$(PRJNAME).o : $(SRCFLDR)/BaseApplication.h $(SRCFLDR)/BaseApplication.cpp $(SRCFLDR)/$(PRJNAME).h $(SRCFLDR)/$(PRJNAME).cpp
	$(CC) $(CFLAGS) $(SRCFLDR)/$(PRJNAME).cpp

BaseApplication.o : $(SRCFLDR)/BaseApplication.h $(SRCFLDR)/BaseApplication.cpp 
	$(CC) $(CFLAGS) $(SRCFLDR)/BaseApplication.cpp

install :
	cp -r $(SRCFLDR)/dist/media $(BINFLDR)
	cp -r $(SRCFLDR)/dist/bin/* $(BINFLDR)

clean:
	\rm *.o *~ $(APP)

tar:
	tar cfv *.cpp *.h Makefile
