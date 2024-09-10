# Source files
FILES = client.c Utils.c

# Object files
OBJS = client.o Utils.o

# Compiler
CC = gcc

# Compiler flags
#Compiler flags like -Wall enable all warnings and -g enable debugging info.
CFLAGS = -Wall -g 

# Target executable name
TARGET = client

# Link Command: Create the executable by linking the object files
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

# Compile Commands: Compile each source file into an object file
client.o: client.c
	$(CC) $(CFLAGS) -c client.c

Utils.o: Utils.c Utils.h
	$(CC) $(CFLAGS) -c Utils.c

# Clean up build files
clean:
	rm -f $(OBJS) $(TARGET)
