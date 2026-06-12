CC = gcc
CFLAGS = -Wall -g

TARGET = social
SRCS = main.c graph.c hash.c bfs.c dfs.c sort.c
OBJS = $(SRCS:.c=.o)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)