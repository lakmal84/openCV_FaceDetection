CC=g++
CFLAGS=-c -Wall -pthread 
LDFLAGS= -pthread
LIBFLAGS= -lopencv_core -lopencv_imgproc -lopencv_highgui -lopencv_gpu -lopencv_objdetect -lboost_program_options
SOURCES=main.cpp fd_cpu.cpp fd_gpu.cpp face_detect.hpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=../bin/face_detection

all: $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) $(LIBFLAGS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@
clean:
	rm -rf *.o ../bin/face_detection
