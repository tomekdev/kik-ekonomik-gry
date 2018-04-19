LDFLAGS = -g -lSDL2 -lSDL2_image -lSDL2_ttf
CPPFLAGS = -g
OBJECTS = main.o \
	  AI.o \
	  gamelogics.o \
	  texture.o \
	  drawboard.o \
	  init.o \
	  graphics.o \
	  score.o 
#	  game.o

kik: $(OBJECTS)
	g++ -o kik $(OBJECTS) $(LDFLAGS)

all: kik

clean:
	rm -rv $(OBJECTS) kik
