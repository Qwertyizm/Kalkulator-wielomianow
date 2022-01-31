CC=gcc
CFLAGS= -std=c11 -Wall -Wextra -Werror  $(shell pkg-config --cflags gtk+-3.0)
LDFLAGS = -lm
LDLIBS = $(shell pkg-config gtk+-3.0  --libs)
LDEXTRAFLAGS= -rdynamic
NAME = kalkulator

SRC = wielomiany.c kalkulator.c main.c
DEPS= wielomiany.h kalkulator.h
OBJS = wielomiany.o kalkulator.o main.o
YOU : $(SRC) $(NAME)

$(NAME): $(OBJS)
	$(CC) $(LDEXTRAFLAGS) $(CFLAGS) $(OBJS) -o kalkulator $(LDFLAGS) $(LDLIBS)

%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $< $(LDFLAGS) $(LDLIBS)
clean:
	rm -f $(OBJS) $(NAME)