g++ -c -o network_posix.o network_posix.cpp

ar rcs libcommon.a network_posix.o
