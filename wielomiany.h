#include <stdbool.h>
typedef struct Wielomian1d Wielomian;
typedef Wielomian* wielomian;
struct Wielomian1d{
    char var[3];
    int size;
    double *val;
};




int max(int a, int b);
char* print(wielomian v,bool delete);
double compute(wielomian v, wielomian x,bool delete);
wielomian copy(wielomian x);
wielomian add(wielomian v, wielomian y);
wielomian sub(wielomian v, wielomian y);
wielomian multiply(wielomian v, wielomian y);
wielomian divide(wielomian v, wielomian y,wielomian r,bool delete);
bool greater_than(wielomian v, wielomian y);
wielomian nwd(wielomian v, wielomian y);
wielomian derivative(wielomian v);
wielomian m_zero(wielomian v);
wielomian from_d_to_w(double d);
void del(wielomian v);


//TODO  we're going to store up to 3 dimensions in linear form knowing sizes of each dimension
int from_polar_to_linear(int x, int y, int z, int xs, int ys, int zs);






