# compiler
CC=gcc
#flags
CLFAGS= -Wall -Wextra -pedantic -fsanitize=address

# target executable
TARGET = jcedit

all: $(TARGET)

$(TARGET): $(TARGET).c syntax.c
	$(CC) $(CFLAGS) -o $(TARGET) $(TARGET).c syntax.c

clean:
	$(RM) $(TARGET)
