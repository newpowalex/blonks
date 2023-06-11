CC=g++
CFLAGS=-c -IC:\Users\Alex\source\libraries\c++\SFML\include -DSFML_STATIC
LDFLAGS=-LC:\Users\Alex\source\libraries\c++\SFML\lib -lsfml-graphics-s -lsfml-window-s -lsfml-system-s -lsfml-audio-s -lopengl32 -lfreetype -lwinmm -lgdi32 -mwindows
SOURCES=main.cpp Game.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=main

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@ $(LDFLAGS)

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	del -f $(OBJECTS) $(EXECUTABLE)
