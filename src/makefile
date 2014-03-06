CC=C:\Dev-Cpp\bin\g++ 
CFLAGS=-c  
LDFLAGS=-lopengl32 -lglu32 -lglaux -luser32 -lgdi32 -lComdlg32 -lComctl32
SOURCES=Camera.cpp Color.cpp Point3D.cpp Texture.cpp TextureVertex2D.cpp Triangle.cpp Wall.cpp Main.cpp MifObject.cpp
OBJECTS=$(SOURCES:.cpp=.o) 
EXECUTABLE="Graphics Editor"

all: $(SOURCES) $(EXECUTABLE)
	
	
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@