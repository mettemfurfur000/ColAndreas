###### some of the tutor is generated with chatgpt becus im too lazyu to writ i t by miself

# Building Tutorial
  
This tutorial will guide you through the process of building the project components. Please note that different components require different build environments.

## Cloning the repo
```
git clone https://github.com/mettemfurfur000/ColAndreas
cd ColAndreas
```

## Building the DLL

The DLL is built in a MinGW-w64 32-bit environment (aka migw32 terminal, light-grey one (not magenta))

1. Install MinGW-w64 with 32-bit architecture support. (msys2 terminal for best results probly)

```
pacman -Sy mingw-w64-x86_64-toolchain
pacman -Sy mingw-w64-i686-toolchain
```
  
or use `./install-msys2-stuff.sh`, it should install all stuff u will need (run twice if u just installed msys2, it will update the terminal itself too)
2. Open a MinGW-w64 32-bit terminal.
3. 

```
make dll
```

4. After successful compilation, you'll find the built DLL in the `dll-build` directory.

## Building the Server

The server is built in a MinGW-w64 64-bit environment.
  
1. Open a MinGW-w64 64-bit terminal.
2.
```
make server
```
3. After successful compilation, you'll find the built server executable in the `build` directory.


If anything goes wrong, check the error messages and make sure you have all the necessary dependencies installed (or leave an issue on github)