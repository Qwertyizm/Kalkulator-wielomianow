CC=gcc
CFLAGS= -std=c11 -Wall -Wextra -Werror
LDFLAGS= -lm
DEPS = kalkulator.h wielomiany.h
NAME = kalkulator
PKG = 'pkg-config gtk+-3.0 --cflags --libs'
$(NAME): kalkulator.o wielomiany.o main.c
	$(CC) $(CFLAGS) kalkulator.o wielomiany.o main.c -o $(NAME) $(PKG) $(LDFLAGS)
wielomiany.o: wielomiany.c $(DEPS)
	$(CC) -c $(CFLAGS) wielomiany.c -o wielomiany.o
kalkulator.o: kalkulator.c $(DEPS)
	$(CC) -c $(CFLAGS) kalkulator.c -o kalkulator.o $(PKG)