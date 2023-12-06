CC = gcc
CFLAGS = -Wall -Wextra -g
LDFLAGS =
TARGET = imprimerie
OBJS = imprimerie.o etat_machine.o main.o

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $^ -o $@ $(LDFLAGS)

%.o: %.c %.h
	$(CC) -c $< -o $@ $(CFLAGS)

%.o: %.c
	$(CC) -c $< -o $@ $(CFLAGS)

.PHONY: clean
clean:
	rm -f $(OBJS)
	rm -f $(TARGET)