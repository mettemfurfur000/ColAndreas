#!/bin/bash

# update everything
pacman -Suuy
# install msys2 stuff
pacman -Sy mingw-w64-x86_64-toolchain
pacman -Sy mingw-w64-i686-toolchain
# install stuff for real binary
pacman -Sy mingw-w64-x86_64-bullet   mingw-w64-x86_64-lua51 mingw-w64-i686-lua51
#         ^                         ^                      ^
#         bullet for physics server |                      |
#                                   lua for server         |
#                                                          lua for client (cuz raksamp was compiler for 32 bit only)