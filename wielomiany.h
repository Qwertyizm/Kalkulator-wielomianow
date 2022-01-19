#include <stdbool.h>

typedef struct Wielomian3d Wielomian;
typedef Wielomian *wielomian;
struct Wielomian3d {
    int size;  //rozmiar ogólny
    int var[3]; //rozmiar poszczególnych zmiennych

    double *val; //reprezentacja wartości przy potęgach zmiennych
};


int max(int a, int b);  //wyznacza max z dwóch liczb

char *print(wielomian v, bool delete); //zwraca reprezentację wielomiany jako ciąg znaków

double compute(wielomian v, wielomian x, wielomian y, wielomian z, bool delete); //wyznacza wartość wielomianu w punkcie

wielomian copy(wielomian x); //kopiuje wielomian

wielomian add(wielomian v, wielomian y); //dodaje do siebie dwa wielomiany

wielomian sub(wielomian v, wielomian y); //odejmuje wielomian od wielomianu

wielomian multiply(wielomian v, wielomian y); //mnoży dwa wielomiany

wielomian divide(wielomian v, wielomian y, wielomian r, bool delete); //dzieli wielomian przez wielomian

bool greater_than_zero(wielomian v); //sprawdza czy wielomian jest różny od zera

wielomian nwd(wielomian v, wielomian y); //wyznacza największy wspólny dzielnik

wielomian derivative(wielomian v, bool delete); //wyznacza pochodną wielomianu jednej zmiennej

wielomian m_zero(wielomian v); //wyznacza miejsce zerowe wielomianu jednej zmiennej

wielomian from_d_to_w(double d); //tworzy strukturę wielomian z liczby

void del(wielomian v); //dealokuje pamięć zajmowaną przez wielomian

void uprosc(wielomian v); //upraszcza zapis wielomianu

int from_kart_to_linear(int x, int y, int z, int xs, int ys); //wyznacza położenie punktu w liniowej reprezentacji przestrzeni 3d






