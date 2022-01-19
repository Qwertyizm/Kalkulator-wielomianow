#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
//#include <fcntl.h>
#include "wielomiany.h"
#include <gtk/gtk.h>

extern void msg(gchar *message);

int max(int a, int b) {
    if (a > b)
        return a;
    return b;
}

int from_kart_to_linear(int x, int y, int z, int xs, int ys) {
    return xs * (ys * z + y) + x;
}

bool is_int(double z) {
    double wynik = (int) z;
    if (wynik != z) {
        return false;
    }
    return true;
}

char *print(wielomian v, bool delete) {
    char *wynik = g_malloc((20 * v->size) * sizeof(char));
    *wynik = '\0';
    char temp[30];
    *temp = '\0';
    double wart;
    bool czy_pierwszy = false;
    for (int i = 0; i < v->var[0]; i++) {
        for (int ii = 0; ii < v->var[1]; ii++) {
            for (int iii = 0; iii < v->var[2]; iii++) {
                wart = v->val[from_kart_to_linear(i, ii, iii, v->var[0], v->var[1])];
                if (wart == 0) { // omit empty powers
                    continue;
                }
                if (czy_pierwszy && wart >= 0) {
                    strcat(wynik, "+");
                }
                if (wart != 1.0) {
                    if (is_int(wart)) {
                        sprintf(temp, "%i", (int) wart);
                    } else {
                        sprintf(temp, "%.4f", wart);
                    }
                    strcat(wynik, temp);
                    czy_pierwszy = true;
                } else {
                    if (v->var[0] - 1 == i && v->var[1] - 1 == ii & v->var[2] - 1 == iii) {
                        if (is_int(wart)) {
                            sprintf(temp, "%i", (int) wart);
                        } else {
                            sprintf(temp, "%.4f", wart);
                        }
                        strcat(wynik, temp);
                        czy_pierwszy = true;
                    }
                }

                if (v->var[0] - 1 > i) {
                    strcat(wynik, "x");
                    czy_pierwszy = true;
                    int pot = v->var[0] - 1 - i;
                    if (pot != 1) {
                        sprintf(temp, "^%i", pot);
                        strcat(wynik, temp);
                    }
                }
                if (v->var[1] - 1 > ii) {
                    strcat(wynik, "y");
                    czy_pierwszy = true;
                    int pot = v->var[1] - 1 - ii;
                    if (pot != 1) {
                        sprintf(temp, "^%i", pot);
                        strcat(wynik, temp);
                    }
                }
                if (v->var[2] - 1 > iii) {
                    strcat(wynik, "z");
                    czy_pierwszy = true;
                    int pot = v->var[2] - 1 - iii;
                    if (pot != 1) {
                        sprintf(temp, "^%i", pot);
                        strcat(wynik, temp);
                    }
                }

            }
        }

    }
    if (strlen(wynik) == 0) {
        sprintf(wynik, "0");
    }
    if (delete) {
        del(v);
    }

    return wynik;
}

double compute(wielomian v, wielomian x, wielomian y, wielomian z, bool delete) {
    double wynik = 0;
    double potx = 1, poty = 1, potz = 1;
    double px = 1, py = 1, pz = 1;
    if (x != NULL) {
        px = x->val[0];
    }
    if (y != NULL) {
        py = y->val[0];
    }
    if (z != NULL) {
        pz = z->val[0];
    }
    for (int i = 0; i < v->var[0]; i++) {
        poty = 1;
        for (int ii = 0; ii < v->var[1]; ii++) {
            potz = 1;
            for (int iii = 0; iii < v->var[2]; iii++) {
                wynik += v->val[from_kart_to_linear(v->var[0] - 1 - i, v->var[1] - 1 - ii, v->var[2] - 1 - iii,
                                                    v->var[0], v->var[1])] * potx * poty * potz;
                potz *= pz;
            }
            poty *= py;
        }
        potx *= px;
    }
    if (delete) {
        del(v);
        del(x);
        del(y);
        del(z);
    }
    return wynik;
}

wielomian copy(wielomian v) {
    wielomian a = malloc(sizeof(Wielomian));
    a->size = v->size;
    for (int i = 0; i < 3; i++) {
        a->var[i] = v->var[i];
    }
    double *tab = calloc(a->size, sizeof(double));
    for (int i = 0; i < a->size; i++) {
        tab[i] = v->val[i];
    }
    a->val = tab;
    return a;
}

wielomian add(wielomian v, wielomian y) {
    if (v == NULL || y == NULL) {
        msg("unsupported action");
        return NULL;
    }
    wielomian z = malloc(sizeof(Wielomian));
    z->size = 1;
    for (int i = 0; i < 3; i++) {
        z->var[i] = max(v->var[i], y->var[i]);
        z->size *= z->var[i];
    }

    double *tab = calloc(z->size, sizeof(double));
    for (int i = 0; i < z->var[0]; i++) {
        for (int ii = 0; ii < z->var[1]; ii++) {
            for (int iii = 0; iii < z->var[2]; iii++) {
                if (v->var[0] > i && v->var[1] > ii && v->var[2] > iii) {
                    tab[from_kart_to_linear(z->var[0] - v->var[0] + i, z->var[1] - v->var[1] + ii,
                                            z->var[2] - v->var[2] + iii, z->var[0],
                                            z->var[1])] += v->val[from_kart_to_linear(i, ii, iii, v->var[0],
                                                                                      v->var[1])];
                }
                if (y->var[0] > i && y->var[1] > ii && y->var[2] > iii) {
                    tab[from_kart_to_linear(z->var[0] - y->var[0] + i, z->var[1] - y->var[1] + ii,
                                            z->var[2] - y->var[2] + iii, z->var[0],
                                            z->var[1])] += y->val[from_kart_to_linear(i, ii, iii, y->var[0],
                                                                                      y->var[1])];
                }
            }
        }
    }
    z->val = tab;
    uprosc(z);
    del(v);
    del(y);
    return z;
}

wielomian sub(wielomian v, wielomian y) {
    if (v == NULL || y == NULL) {
        msg("unsupported action");
        return NULL;
    }
    wielomian z = malloc(sizeof(Wielomian));
    z->size = 1;
    for (int i = 0; i < 3; i++) {
        z->var[i] = max(v->var[i], y->var[i]);
        z->size *= z->var[i];
    }

    double *tab = calloc(z->size, sizeof(double));
    for (int i = 0; i < z->var[0]; i++) {
        for (int ii = 0; ii < z->var[1]; ii++) {
            for (int iii = 0; iii < z->var[2]; iii++) {
                if (v->var[0] > i && v->var[1] > ii && v->var[2] > iii) {
                    tab[from_kart_to_linear(z->var[0] - v->var[0] + i, z->var[1] - v->var[1] + ii,
                                            z->var[2] - v->var[2] + iii, z->var[0],
                                            z->var[1])] += v->val[from_kart_to_linear(i, ii, iii, v->var[0],
                                                                                      v->var[1])];
                }
                if (y->var[0] > i && y->var[1] > ii && y->var[2] > iii) {
                    tab[from_kart_to_linear(z->var[0] - y->var[0] + i, z->var[1] - y->var[1] + ii,
                                            z->var[2] - y->var[2] + iii, z->var[0],
                                            z->var[1])] -= y->val[from_kart_to_linear(i, ii, iii, y->var[0],
                                                                                      y->var[1])];
                }
            }
        }
    }
    z->val = tab;
    del(v);
    del(y);
    uprosc(z);
    return z;
}

wielomian multiply(wielomian v, wielomian y) {
    if (v == NULL || y == NULL) {
        msg("unsupported action");
        return NULL;
    }
    wielomian z = malloc(sizeof(Wielomian));

    z->size = 1;
    for (int i = 0; i < 3; i++) {
        z->var[i] = v->var[i] + y->var[i] - 1;
        z->size *= z->var[i];
    }
    double *tab = calloc(z->size, sizeof(double));
    for (int i = 0; i < v->var[0]; i++) {
        for (int ii = 0; ii < v->var[1]; ii++) {
            for (int iii = 0; iii < v->var[2]; iii++) {
                for (int j = 0; j < y->var[0]; j++) {
                    for (int jj = 0; jj < y->var[1]; jj++) {
                        for (int jjj = 0; jjj < y->var[2]; jjj++) {
                            tab[from_kart_to_linear(i + j, ii + jj, iii + jjj, z->var[0], z->var[1])] +=
                                    v->val[from_kart_to_linear(i, ii, iii, v->var[0], v->var[1])]
                                    * y->val[from_kart_to_linear(j, jj, jjj, y->var[0], y->var[1])];
                        }
                    }
                }
            }
        }
    }
    z->val = tab;
    uprosc(z);
    del(v);
    del(y);
    return z;
}

wielomian divide(wielomian v, wielomian y, wielomian r, bool delete) {
    if (v == NULL || y == NULL ) {
        msg("unsupported action");
        return NULL;
    }
    uprosc(v);
    uprosc(y);
    wielomian z = malloc(sizeof(Wielomian));
    z->size=1;
    for(int i=0;i<3;i++){
        z->var[i] = v->var[i] - y->var[i] + 1;
        z->size*=z->var[i];
        if(z->var[i]<=0){
            free(z);
            z = from_d_to_w(0);
            if(delete){
                del(v);
                del(y);
            }
            return z;
        }
    }



    wielomian temp = copy(v);
    *r = *temp;


    double *tab = calloc(z->size, sizeof(double));
    bool done_something=true;
    while(done_something){
        done_something=false;
        for(int i=0;i<v->var[0]-y->var[0]+2&&i<v->var[0];i++){
            for(int ii=0;ii<v->var[1]-y->var[1]+2&&ii<v->var[1];ii++){
                for(int iii=0;iii<v->var[2]-y->var[2]+2&&iii<v->var[2];iii++){
                    double wartx=r->val[from_kart_to_linear(i,ii,iii,r->var[0],r->var[1])];
                    if(wartx!=0){
                        bool found=false;
                        int potx,poty,potz;
                        for(int j=0;j<=i&&j<y->var[0];j++){
                            for(int jj=0;jj<=ii&&jj<y->var[1];jj++){
                                for(int jjj=0;jjj<=iii&&jjj<y->var[2];jjj++){
                                    double warty=y->val[from_kart_to_linear(j,jj,jjj,y->var[0],y->var[1])];
                                    if(warty!=0){
                                        found=true;
                                        wartx/=warty;
                                        potx=v->var[0]-i-(y->var[0]-j);
                                        poty=v->var[1]-ii-(y->var[1]-jj);
                                        potz=v->var[2]-iii-(y->var[2]-jjj);
                                        if(potx<0||poty<0||potz<0){
                                            found=false;
                                        }
                                    }
                                    if(found) break;
                                }
                                if(found) break;
                            }
                            if(found) break;
                        }
                        if(found){
                            done_something=true;
                            tab[from_kart_to_linear(z->var[0]-(potx+1),z->var[1]-poty-1,z->var[2]-potz-1,z->var[0],z->var[1])]+=wartx;
                            for(int j=0;j<y->var[0];j++){
                                for(int jj=0;jj<y->var[1];jj++){
                                    for(int jjj=0;jjj<y->var[2];jjj++){
                                        r->val[from_kart_to_linear(v->var[0]-1-((y->var[0]-1-j)+potx),v->var[1]-((y->var[1]-jj)+poty),v->var[2]-((y->var[2]-jjj)+potz),r->var[0],r->var[1])]-=
                                                wartx*y->val[from_kart_to_linear(j,jj,jjj,y->var[0],y->var[1])];
                                    }
                                }
                            }
                        }

                    }
                }
            }
        }
    }


    z->val = tab;
    if (delete) {
        del(v);
        del(y);
    }
    uprosc(z);
    uprosc(r);
    return z;
}

bool greater_than_zero(wielomian v) {
    if (v == NULL) {
        msg("unsupported action");
        return false;
    }
    if(v->size==1&&v->val[0]==0){
        return false;
    }
    for(int j=0;j<v->var[0];j++){
        for(int jj=0;jj<v->var[1];jj++){
            for(int jjj=0;jjj<v->var[2];jjj++){
                if(v->val[from_kart_to_linear(j,jj,jjj,v->var[0],v->var[1])]!=0){
                    return true;
                }
            }
        }
    }
    return false;
}

wielomian nwd(wielomian v, wielomian y) {
    if (v == NULL || y == NULL) {
        msg("unsupported action");
        return NULL;
    }
    wielomian a = copy(v);
    wielomian b = copy(y);
    while (greater_than_zero(b)) {
        wielomian a1 = a;
        wielomian r = malloc(sizeof(Wielomian));
        r->size=0;
        wielomian temp = divide(a, b, r, false);
        del(temp);
        a = b;
        b = r;
        del(a1);
    }
    if (b != NULL) {
        del(b);
    }
    del(v);
    del(y);
    uprosc(a);
    return a;
}

wielomian derivative(wielomian v, bool delete) {
    if (((v->val[0] > 1) + (v->val[1] > 1) + (v->val[2] > 1)) > 1) {
        msg("unsupported action");
        return NULL;
    }
    wielomian wynik = malloc(sizeof(Wielomian));
    if (v->size == 1) {
        wynik = from_d_to_w(0);
    } else {
        wynik->size = v->size - 1;
        for (int i = 0; i < 3; i++) {
            if (v->var[i] > 1) {
                wynik->var[i] = v->var[i] - 1;
            } else {
                wynik->var[i] = 1;
            }
        }
        double *temp_tab = calloc(wynik->size, sizeof(double));
        for (int i = 0; i < wynik->size; i++) {
            temp_tab[i] = v->val[i] * (v->size - 1 - i);
        }
        wynik->val = temp_tab;
    }
    if (delete) {
        del(v);
    }
    uprosc(wynik);
    return wynik;
}

double abs_d(double d) {
    if (d < 0) {
        return -d;
    }
    return d;
}

wielomian m_zero(wielomian v) {
    if (((v->val[0] > 1) + (v->val[1] > 1) + (v->val[2] > 1)) > 1) {
        msg("unsupported action");
        return NULL;
    }
    wielomian pochodna = derivative(v, false);
    double punkt_poczatkowy = 5;
    double temp = 0;
    double dokladnosc = 0.0001;
    int pot = 0;
    for (int i = 0; i < 3; i++) {
        if (v->val[i] > 1) {
            pot = i;
            break;
        }
    }
    do {
        temp = punkt_poczatkowy;
        wielomian t = from_d_to_w(temp);
        switch (pot) {
            case 0:
                punkt_poczatkowy = temp - (compute(v, t, NULL, NULL, false)) / (compute(pochodna, t, NULL, NULL, false));
                break;
            case 1:
                punkt_poczatkowy = temp - (compute(v, NULL, t, NULL, false)) / (compute(pochodna, NULL, t, NULL, false));
                break;
            case 2:
                punkt_poczatkowy = temp - (compute(v, NULL, NULL, t, false)) / (compute(pochodna, NULL, NULL, t, false));
                break;
            default:
                msg("Niespodziewany błąd");
                return NULL;
        }
        free(t);
    } while (abs_d(punkt_poczatkowy - temp) >= dokladnosc);
    del(pochodna);
    del(v);

    return from_d_to_w(punkt_poczatkowy);
}

wielomian from_d_to_w(double d) {
    wielomian wynik = malloc(sizeof(Wielomian));
    wynik->size = 1;
    for (int i = 0; i < 3; i++) {
        wynik->var[i] = 1;
    }
    double *temp_tab = calloc(1, sizeof(double));
    temp_tab[0] = d;
    wynik->val = temp_tab;
    return wynik;
}

void uprosc(wielomian v){
    int potx=v->var[0]-1,poty=v->var[1]-1,potz=v->var[2]-1;
    for(int j=0;j<v->var[0];j++){
        for(int jj=0;jj<v->var[1];jj++){
            for(int jjj=0;jjj<v->var[2];jjj++){
                if(v->val[from_kart_to_linear(j,jj,jjj,v->var[0],v->var[1])]!=0){
                    if(j<potx){
                        potx=j;
                    }
                    if(jj<poty){
                        poty=jj;
                    }
                    if(jjj<potz){
                        potz=jjj;
                    }
                }
            }
        }
    }
    int p[3]={v->var[0]-potx,v->var[1]-poty,v->var[2]-potz};
    double* tab=calloc(p[0]*p[1]*p[2],sizeof(double));
    for(int j=0;j<p[0];j++){
        for(int jj=0;jj<p[1];jj++){
            for(int jjj=0;jjj<p[2];jjj++){
                tab[from_kart_to_linear(j,jj,jjj,p[0],p[1])]=
                        v->val[from_kart_to_linear(potx+j,poty+jj,potz+jjj,v->var[0],v->var[1])];
            }
        }
    }
    free(v->val);
    v->val=tab;
    v->size=p[0]*p[1]*p[2];
    for(int i=0;i<3;i++){
        v->var[i]=p[i];
    }
}

void del(wielomian v) {
    if (v != NULL) {
        free(v->val);
    }
    free(v);
}

