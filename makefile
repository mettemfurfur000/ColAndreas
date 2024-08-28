CFLAGS += -O3 -std=c++11 

# CFLAGS += -pg -no-pie -fno-omit-frame-pointer -fno-inline-functions -fno-inline-functions-called-once -fno-optimize-sibling-calls -Wno-write-strings -fshort-wchar

LDFLAGS += -lm

CFLAGS += -IC:/msys64/mingw64/include/bullet

LDFLAGS32 += -LC:/msys64/mingw32/lib -lmingw32 -lws2_32 -llua5.1
LDFLAGS32 += -lm
LDFLAGS32 += -Lwin32/ -lbinn-3.0

LDFLAGS += -LC:/msys64/mingw64/lib -lmingw32 -lws2_32 -llua5.1

LDFLAGS += -lBullet2FileLoader.dll
LDFLAGS += -lBullet3Collision.dll
LDFLAGS += -lBullet3Common.dll
LDFLAGS += -lBullet3Dynamics.dll
LDFLAGS += -lBullet3Geometry.dll
LDFLAGS += -lBullet3OpenCL_clew.dll
LDFLAGS += -lBulletCollision.dll
LDFLAGS += -lBulletDynamics.dll
LDFLAGS += -lBulletExampleBrowserLib.dll
LDFLAGS += -lBulletFileLoader.dll
LDFLAGS += -lBulletInverseDynamics.dll
LDFLAGS += -lBulletInverseDynamicsUtils.dll
LDFLAGS += -lBulletRobotics.dll
LDFLAGS += -lBulletRoboticsGUI.dll
LDFLAGS += -lBulletSoftBody.dll
LDFLAGS += -lBulletWorldImporter.dll
LDFLAGS += -lBulletXmlWorldImporter.dll
LDFLAGS += -lBussIK.dll
LDFLAGS += -lConvexDecomposition.dll
LDFLAGS += -lGIMPACTUtils.dll
LDFLAGS += -lHACD.dll
LDFLAGS += -lLinearMath.dll
LDFLAGS += -lOpenGLWindow.dll
LDFLAGS += -lclsocket
LDFLAGS += -lgwen.dll

LDFLAGS += -Lx64/ -lbinn-3.0

DLLOUTNAME = calua.dll
SERVERNAME = calua_physics_server.exe

DLLOUTPATH = dll_build
REGULAROUTPATH = build
OBJPATH = obj
SRCPATH = src
MAINFILESPATH = mains

sources := $(shell cd src;echo *.cpp)
objects := $(patsubst %.cpp,obj/%.o,$(sources))
headers := $(shell cd src/include;echo *.h)

.PHONY: make_folders
make_folders:
	mkdir -p $(OBJPATH)
	mkdir -p $(REGULAROUTPATH)
	mkdir -p $(DLLOUTPATH)

$(OBJPATH)/%.o : $(SRCPATH)/%.cpp
	g++ $(CFLAGS) -c $^ -o $@

$(OBJPATH)/%.o: $(MAINFILESPATH)/%.cpp
	g++ $(CFLAGS) -c $^ -o $@

$(OBJPATH)/%.o: $(SRCPATH)/%.c
	gcc -c $^ -o $@

$(OBJPATH)/%.o: $(MAINFILESPATH)/%.c
	gcc -c $^ -o $@

.PHONY: make_folders dll 
dll: remake_networking $(OBJPATH)/plugin_adapter.o $(OBJPATH)/networking.o 
	gcc -shared -fpic -static-libgcc -o $(DLLOUTPATH)/$(DLLOUTNAME) $(OBJPATH)/plugin_adapter.o $(OBJPATH)/networking.o $(LDFLAGS32)
	-./grab_dlls.sh $(DLLOUTPATH)/$(DLLOUTNAME) /mingw32/bin 1
	-./grab_dlls.sh $(DLLOUTPATH)/$(DLLOUTNAME) win32 1
	cp test.lua $(DLLOUTPATH)/
	echo $$'\a'

.PHONY: make_folders server 
server: remake_networking $(OBJPATH)/server.o $(objects) 
	g++ $(CFLAGS) -o $(REGULAROUTPATH)/$(SERVERNAME) $(OBJPATH)/server.o $(objects) $(LDFLAGS) -static-libstdc++ -static-libgcc
	-./grab_dlls.sh $(REGULAROUTPATH)/$(SERVERNAME) /mingw64/bin 1
	-./grab_dlls.sh $(REGULAROUTPATH)/$(SERVERNAME) x64 1
	cp ../WizardApp/build/world.cadb build/
	echo $$'\a'

.PHONY: clean_dll clean_server clean_obj
clean_dll: clean_obj
	rm -rf $(DLLOUTPATH)/*
clean_server: clean_obj
	rm -rf $(REGULAROUTPATH)/*
clean_obj:
	rm -rf $(OBJPATH)/*

.PHONY: remake_networking
remake_networking:
	rm -rf $(OBJPATH)/networking.o

.PHONY: all
all: dll server
