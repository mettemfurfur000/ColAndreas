CFLAGS += -O3 -g
LDFLAGS += -lm

CFLAGS += -IC:/msys64/mingw64/include/bullet

LDFLAGS32 += -LC:/msys64/mingw32/lib -lmingw32 -lws2_32 -llua
LDFLAGS32 += -lm
LDFLAGS32 += -Lwin32/ -lbinn-3.0

LDFLAGS += -LC:/msys64/mingw64/lib -lmingw32 -lws2_32 -llua

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

# LDFLAGS += -Lx64/ 
# LDFLAGS += -lbinn-3.0

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

WIZARDFILE = wizard.exe

.PHONY: make_folders wizard
wizard: $(OBJPATH)/wizard.o $(objects)
	g++ $(CFLAGS) -o $(REGULAROUTPATH)/$(WIZARDFILE) $(OBJPATH)/wizard.o $(objects) $(LDFLAGS) -static-libstdc++ -static-libgcc
	-./grab_dlls.sh $(REGULAROUTPATH)/$(WIZARDFILE) /mingw64/bin 1
	cp default.cfg $(REGULAROUTPATH)/
	cp commands.txt $(REGULAROUTPATH)/
	cp arizona.sh $(REGULAROUTPATH)/

.PHONY: clean
clean:
	rm -rf $(REGULAROUTPATH)/*
	rm -rf $(OBJPATH)/*
	rm -rf $(DLLOUTPATH)/*

.PHONY: all
all: wizard
