#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <gtk/gtk.h>
#include "kalkulator.h"


extern void msg(gchar* message);
#pragma clang diagnostic push
#pragma ide diagnostic ignored "LoopDoesntUseConditionVariableInspection"
#define WIELOMIAN '0'



/***************** DEFINICJE FUNKCJI ********************/
wielomian oblicz(char *wejscie, wielomian w2)
{
    int z;
    char *inptr=wejscie;
    wielomian wynik=NULL;

    if((z=czytaj_znak(&inptr)) != EOF) {
        zwroc_znak(z,&inptr);
        wynik=wyrazenie(&inptr,w2);
    }
    else{
        msg("Puste wejście");
    }
    return wynik;
}

static void zwroc_znak(int z, char **inp)
{
  if (z != EOF && z != WIELOMIAN)
      --*inp;
}

static int czytaj_znak(char **inp)
{
    int z;

    if (**inp == '\0') return EOF;
    while ((z=*(*inp)++) != '\0' && isspace(z)) ;
    if (isdigit(z) || z == '.' || z == 'x'|| z == 'y'|| z == 'z') {
        zwroc_znak(z, inp);
        return WIELOMIAN;
    }
    return z == 0 ? EOF : z;
}

static wielomian czytaj_wielomian(char **inp)
{
    int z;
    double n=0.0, pot10=1.0;
    wielomian w=malloc(sizeof(Wielomian));

    while ((z=*(*inp)++) != '\0' && isdigit(z)){
        n=10.0 * n + (z-'0');
    }
    if (z == '.' ){
        while ((z=*(*inp)++) != '\0' && isdigit(z)) {
            n=10.0 * n + (z-'0');
            pot10*=10.0;
        }
    }
    if(z=='x'||z=='y'||z=='z'){
        if(n==0.0){
            n=1.0;
        }
        w->var[0]=z;
        int pow=0;
        if(*(*inp)=='^'){
            (*inp)++;
            while ((z=*(*inp)++) != '\0' && isdigit(z)){
                pow=10 * pow + (z-'0');
            }
            zwroc_znak(z == 0 ? EOF : z, inp);
        }
        else{
            pow=1;
        }
        w->size=pow+1;
        double* tab=malloc(w->size*sizeof(double));
        *tab=n/pot10;
        for(int i=1;i<w->size;i++){
            *(tab+i)=0;
        }
        w->val=tab;
    }else{
        w->size=1;
        double* tab=malloc(sizeof(double));
        tab[0]=n/pot10;
        w->val=tab;
        zwroc_znak(z == 0 ? EOF : z, inp);
    }
    return w;
}

static wielomian wyrazenie(char **inp, wielomian w2)
{
    int z;
    wielomian wyn, x2;

    if ((z=czytaj_znak(inp)) != '-' && z != '+')
      zwroc_znak(z, inp);
    wyn=skladnik(inp,w2);

    if (z == '-'){
        wielomian zero=from_d_to_w(0);
        wyn=sub(zero,wyn);
    }

    while ((z=czytaj_znak(inp)) == '+' || z == '-') {
        x2=skladnik(inp,w2);
        wyn=(z == '+' ? add(wyn,x2) : sub(wyn,x2));
    }
    zwroc_znak(z, inp);
    return wyn;
}

static wielomian skladnik(char **inp,wielomian w2)
{
    int z,c;
    wielomian wyn,x2;
    if((z=czytaj_znak(inp))=='c'||z=='G'||z=='m'||z=='d'){
        while((c=*(*inp)++)!='\0'&& (isalpha(c)||c=='_'));
            if(z=='m'||z=='d'){
                zwroc_znak(c,inp);
                wyn=czynnik(inp);
                wyn=(z=='m' ? m_zero(wyn) : derivative(wyn,true));
            }
            else{
                if(c=='('){
                    wyn=czynnik(inp);
                    (*inp)++; //ignore ','
                    x2=czynnik(inp);
                    if((c=czytaj_znak(inp))==')'){
                        wyn=(z=='c' ? from_d_to_w(compute(wyn,x2,true)) : nwd(wyn,x2));
                    }
                    else{
                        msg("Brakuje nawiasu");
                        return NULL;
                    }
                }
                else{
                    msg("Błędna składnia");
                }
            }
    }
    else{
        zwroc_znak(z,inp);
        wyn=czynnik(inp);
        while ((z=czytaj_znak(inp)) == '*' || z == '/') {
            x2=czynnik(inp);
            wyn=(z == '*' ? multiply(wyn,x2) : divide(wyn,x2,w2,true));
        }
        zwroc_znak(z, inp);
    }
    return wyn;
}

static wielomian czynnik(char **inp)
{
    int z;
    wielomian wyn;

    if ((z=czytaj_znak(inp)) == WIELOMIAN)
        return czytaj_wielomian(inp);
    else if (z == '(') {
        wyn = wyrazenie(inp,NULL);
        if ((z = czytaj_znak(inp)) == ')')
            return wyn;
        else{
            msg("Brakuje nawiasu");
        }
    }
    return NULL;
}

#pragma clang diagnostic pop