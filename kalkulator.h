#include <stdlib.h>
#include "wielomiany.h"


/***************** PROTOTYPY FUNKCJI ********************/
static int czytaj_znak(char **inp); // czyta kolejny widoczny znak z wejścia
static void zwroc_znak(int z, char **inp); // oddaje znak na wejście
static wielomian czytaj_wielomian(char **inp); // czyta kolejny wielomian z wejścia

static wielomian wyrazenie(char **inp, wielomian w2); // analizuje składnie wyrażenia i wylicza jego wartość
static wielomian skladnik(char **inp, wielomian w2);  // analizuje składnie składnika i wylicza jego wartość
static wielomian czynnik(char **inp);   // analizuje składnie czynnika i wylicza jego wartość
