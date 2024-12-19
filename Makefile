CXX      := gcc 
CFLAGS := -pedantic-errors -Wall -Wextra `pkg-config --cflags sdl2`
LIBS  := -L/usr/lib -lstdc++ -lm `pkg-config --libs sdl2` -lSDL2_image

make: main.c vec.c util.c
	$(CC) $(CFLAGS) -o te main.c vec.c util.c $(LIBS)