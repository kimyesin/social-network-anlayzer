CC = gcc
CFLAGS = -Wall -g

TARGET = social
SRCS = main.c src/network.c src/feature.c
OBJS = $(SRCS:.c=.o)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f main.o src/network.o src/feature.o $(TARGET)