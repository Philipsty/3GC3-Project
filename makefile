#please use 'make clean' to clean the directory of intermediate build files and the executable
#simply typing 'make' will compile all source code files to object files .o, and then link all
#object files into an executable
#we are using a lot of makefile macros

#changing platform dependant stuff, do not change this
# Linux (default)
LDFLAGS = -lGL -lGLU -lglut
CFLAGS=-g -Wall -std=c++11
CC=g++
EXEEXT=
RM=rm

# Windows (cygwin)
ifeq "$(OS)" "Windows_NT"
	EXEEXT=.exe #on windows applications must have .exe extension
	RM=del #rm command for windows powershell
    LDFLAGS = -lfreeglut -lglu32 -lopengl32
else
	# OS X
	OS := $(shell uname)
	ifeq ($(OS), Darwin)
	        LDFLAGS = -framework Carbon -framework OpenGL -framework GLUT
	endif
endif

#change the 't1' name to the name you want to call your application
PROGRAM_NAME=a3

#run target to compile and build, and then launch the executable
# run: $(PROGRAM_NAME)
# 	./$(PROGRAM_NAME)$(EXEEXT)

run :
	$(CC) -LC:/MinGW/include -o mainProj.o Paintball.cpp Splatter.cpp main.cpp $(CFLAGS) $(LDFLAGS) -Wno-deprecated
	./mainProj.o

#when adding additional source files, such as boilerplateClass.cpp
#or yourFile.cpp, add the filename with an object extension below
#ie. boilerplateClass.o and yourFile.o
#make will automatically know that the objectfile needs to be compiled
#form a cpp source file and find it itself :)
$(PROGRAM_NAME): mainProj 
	$(CC) -LC:/MinGW/include -o $@ $^ $(CFLAGS) $(LDFLAGS)

FORCE: ;

clean:
	$(RM) *.o $(PROGRAM_NAME)$(EXEEXT)
