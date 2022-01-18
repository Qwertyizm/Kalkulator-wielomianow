#include <stdbool.h>

typedef struct Wielomian3d Wielomian;
typedef Wielomian *wielomian;
struct Wielomian3d {
    int size;  //rozmiar ogólny
    int var[3]; //rozmiar poszczególnych zmiennych

    double *val;
};


int max(int a, int b);

char *print(wielomian v, bool delete);

double compute(wielomian v, wielomian x,wielomian y, wielomian z, bool delete);

wielomian copy(wielomian x);

wielomian add(wielomian v, wielomian y);

wielomian sub(wielomian v, wielomian y);

wielomian multiply(wielomian v, wielomian y);

wielomian divide(wielomian v, wielomian y, wielomian r, bool delete);

bool greater_than_zero(wielomian v);

wielomian nwd(wielomian v, wielomian y);

wielomian derivative(wielomian v, bool delete);

wielomian m_zero(wielomian v);

wielomian from_d_to_w(double d);

void del(wielomian v);


//TODO  we're going to store up to 3 dimensions in linear form knowing sizes of each dimension
int from_kart_to_linear(int x, int y, int z, int xs, int ys);






