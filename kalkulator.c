#include <stdlib.h>
#include <ctype.h>
#include <gtk/gtk.h>
#include "kalkulator.h"

extern void msg(gchar *message);
#define WIELOMIAN '~'


wielomian oblicz(char *wejscie, wielomian w2) {
    int z;
    char *inptr = wejscie;
    wielomian wynik = NULL;

    if ((z = czytaj_znak(&inptr)) != EOF) {
        zwroc_znak(z, &inptr);
        wynik = wyrazenie(&inptr, w2);
    } else {
        msg("Puste wejście");
    }
    return wynik;
}

static void zwroc_znak(int z, char **inp) {
    if (z != EOF && z != WIELOMIAN)
        --*inp;
}

static int czytaj_znak(char **inp) {
    int z;

    if (**inp == '\0') return EOF;
    while ((z = *(*inp)++) != '\0' && isspace(z));
    if (isdigit(z) || z == '.' || z == 'x' || z == 'y' || z == 'z') {
        zwroc_znak(z, inp);
        return WIELOMIAN;
    }
    return z == 0 ? EOF : z;
}

static wielomian czytaj_wielomian(char **inp) {
    int z;
    double n = 0.0, pot10 = 1.0;
    wielomian w = malloc(sizeof(Wielomian));

    while ((z = *(*inp)++) != '\0' && isdigit(z)) {
        n = 10.0 * n + (z - '0');
    }
    if (z == '.') {
        while ((z = *(*inp)++) != '\0' && isdigit(z)) {
            n = 10.0 * n + (z - '0');
            pot10 *= 10.0;
        }
    }

    for (int i = 0; i < 3; i++) {
        w->var[i] = 1;
    }
    w->size = 1;

    if (z == 'x' || z == 'y' || z == 'z') {
        if (n == 0.0) {
            n = 1.0;
        }
        zwroc_znak(z, inp);
        while ((z = *(*inp)++) == 'x' || z == 'y' || z == 'z') {
            int pow = 0;
            int c;
            if (*(*inp) == '^') {
                (*inp)++;
                while ((c = *(*inp)++) != '\0' && isdigit(c)) {
                    pow = 10 * pow + (c - '0');
                }
                zwroc_znak(c == 0 ? EOF : c, inp);
            } else {
                pow = 1;
            }
            switch (z) {
                case 'x':
                    w->var[0] += pow;
                    break;
                case 'y':
                    w->var[1] += pow;
                    break;
                case 'z':
                    w->var[2] += pow;
                    break;
                default:
                    msg("Niespodziewany błąd");
                    return NULL;
            }
        }
        for(int i=0;i<3;i++){
            w->size*=w->var[i];
        }
        zwroc_znak(z == 0 ? EOF : z, inp);
        double *tab = calloc(w->size, sizeof(double));
        *tab = n / pot10;
        w->val = tab;
    } else {
        double *tab = malloc(sizeof(double));
        tab[0] = n / pot10;
        w->val = tab;
        zwroc_znak(z == 0 ? EOF : z, inp);
    }
    return w;
}

static wielomian wyrazenie(char **inp, wielomian w2) {
    int z;
    wielomian wyn, x2;

    if ((z = czytaj_znak(inp)) != '-' && z != '+')
        zwroc_znak(z, inp);
    wyn = skladnik(inp, w2);

    if (z == '-') {
        wielomian zero = from_d_to_w(0);
        wyn = sub(zero, wyn);
    }

    while ((z = czytaj_znak(inp)) == '+' || z == '-') {
        x2 = skladnik(inp, w2);
        wyn = (z == '+' ? add(wyn, x2) : sub(wyn, x2));
    }
    zwroc_znak(z, inp);
    return wyn;
}

static wielomian skladnik(char **inp, wielomian w2) {
    int z, c;
    wielomian wyn, x[3];

    if ((z = czytaj_znak(inp)) == 'c' || z == 'G' || z == 'm' || z == 'd') {
        while ((c = *(*inp)++) != '\0' && (isalpha(c) || c == '_'));

        wyn = wyrazenie(inp, NULL);
        if (c == '(') {
            if (z == 'm' || z == 'd') {
                if ((c = czytaj_znak(inp)) == ')') {
                    wyn = (z == 'm' ? m_zero(wyn) : derivative(wyn, true));
                } else {
                    msg("Brakuje nawiasu");
                    return NULL;
                }
            } else {
                if (*(*inp) == ',') {
                    (*inp)++; //ignore ','
                } else {
                    msg("Brakuje przecinka");
                    return NULL;
                }
                char zmienna[3]={0,0,0};
                if(z=='c'){
                    zmienna[0]=*(*inp)++;
                    if(*(*inp)=='='){
                        (*inp)++; //ignore '='
                    }
                    else{
                        msg("Brakuje znaku '='");
                        return NULL;
                    }
                }
                x[0] = wyrazenie(inp, NULL);
                if(z=='c'){
                    for(int i=1;i<3;i++){
                        if (*(*inp) == ',') {
                            (*inp)++; //ignore ','
                        } else {
                            break;
                        }
                        zmienna[i]=*(*inp)++;
                        if(*(*inp)=='='){
                            (*inp)++; //ignore '='
                        }
                        else{
                            msg("Brakuje znaku '='");
                            return NULL;
                        }
                        x[i]=wyrazenie(inp,NULL);
                    }
                }
                if ((c = czytaj_znak(inp)) == ')') {
                    if(z=='c'){
                        wielomian akt[3]={NULL,NULL,NULL};
                        for(int i=0;i<3;i++){
                            switch (zmienna[i]) {
                                case 'x':
                                    akt[0]=x[i];
                                    break;
                                case 'y':
                                    akt[1]=x[i];
                                    break;
                                case 'z':
                                    akt[2]=x[i];
                                    break;
                                case 0:
                                    break;
                                default:
                                    msg("Nieznana zmienna");
                                    return NULL;
                            }
                        }
                        wyn=from_d_to_w(compute(wyn,(akt[0]!=NULL?akt[0]:from_d_to_w(0)),(akt[1]!=NULL?akt[1]:from_d_to_w(0)),(akt[2]!=NULL?akt[2]:from_d_to_w(0)),true));
                    }else{
                        wyn=nwd(wyn,x[0]);
                    }
                } else {
                    msg("Brakuje nawiasu");
                    return NULL;
                }
            }
        } else {
            msg("Brakuje nawiasu");
        }
    } else {
        zwroc_znak(z, inp);
        wyn = czynnik(inp);
        while ((z = czytaj_znak(inp)) == '*' || z == '/') {
            x[0] = czynnik(inp);
            wyn = (z == '*' ? multiply(wyn, x[0]) : divide(wyn, x[0], w2, true));
        }
        zwroc_znak(z, inp);
    }
    return wyn;
}

static wielomian czynnik(char **inp) {
    int z;
    wielomian wyn;

    if ((z = czytaj_znak(inp)) == WIELOMIAN)
        return czytaj_wielomian(inp);
    else if (z == '(') {
        wyn = wyrazenie(inp, NULL);
        if ((z = czytaj_znak(inp)) == ')')
            return wyn;
        else {
            msg("Brakuje nawiasu");
        }
    }
    return NULL;
}

