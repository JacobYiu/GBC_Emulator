CXX = g++
CXXFLAGS = -std=c++11 -Wall

SDL_CFLAGS = -lSDL2

GL_CFLAGS = -lGL

MAIN = GameBoyRun.cpp

# Anything with *1 needs to be compiled first 
# Anything with *2 needs to be compiled second
# Anything with *3 needs to be compiled last


SRC1 = LogMessage.cpp    

SRC2 = Emulator.cpp Emulator.i8080Instr.cpp Emulator.JumpTable.cpp

SRC3 = Gameboy.cpp

HDRS1 = LogMessage.h 

HDRS2 = Emulator.h

HDRS3 = Gameboy.h

#We do not want to createa an GameBoyRun.o because that is the main file we want to run and we will get duplication declarations 
OBJS1 = LogMessage.o

OBJS2 = Emulator.o Emulator.i8080Instr.o Emulator.JumpTable.o

OBJS3 = Gameboy.o 

OBJS = $(OBJS1) $(OBJS2) $(OBJS3) 

TARGET = startGBC

$(TARGET): $(OBJS) $(MAIN)
	echo "Compiling Target"
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(MAIN) $(OBJS) $(SDL_CFLAGS) $(GL_CFLAGS)

$(OBJS3): $(SRC3) $(HDRS3) $(SRC2) $(HDRS2) $(OBJS2)
	$(CXX) $(CXXFLAGS) -c $< -o $@ $(SDL_CFLAGS) $(GL_CFLAGS) 

Emulator%.o: Emulator%.cpp $(HDRS2) $(SRC1) $(HDRS1) $(OBJS1)
	$(CXX) $(CXXFLAGS) -c $< -o $@ $(SDL_CFLAGS) $(GL_CFLAGS)

$(OBJS1): $(SRC1) $(HDRS1)
	$(CXX) $(CXXFLAGS) -c $< -o $@ $(SDL_CFLAGS) $(GL_CFLAGS) 

clean: 
	rm -rf $(OBJS1) $(OBJS2) $(TARGET)