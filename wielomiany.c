#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
//#include <fcntl.h>
#include "wielomiany.h"
#include <gtk/gtk.h>

extern void msg(gchar* message);

int max(int a, int b){
    if(a>b)
        return a;
    return b;
}



char* print(wielomian v,bool delete){
    char* wynik=g_malloc((8*v->size)*sizeof(char));
    *wynik='\0';
    char temp[30];
    *temp='\0';
    for(int i=0;i<v->size-1;i++){
        if(v->val[i]==0){ // omit empty powers
            continue;
        }
        sprintf(temp,"%.2f%c^%i",v->val[i],v->var[0],v->size-i-1);
        strcat(wynik,temp);
        if(v->val[i+1]>=0){
            strcat(wynik,"+");
        }
    }
    sprintf(temp,"%.4f",v->val[v->size-1]);
    strcat(wynik,temp);
    if(delete){
        del(v);
    }

    return wynik;
}

double compute(wielomian v, wielomian x,bool delete){
    double wynik=0;
    for(int i=0;i<v->size;i++){
        wynik*=x->val[0];
        wynik+=v->val[i];
    }
    if(delete){
        del(v);
        del(x);
    }
    return wynik;
}

wielomian copy(wielomian v){
    wielomian a=malloc(sizeof(Wielomian));
    a->size=v->size;
    if(v->size>1){
        a->var[0]=v->var[0];
    }
    double* tab=calloc(a->size,sizeof(double));
    for(int i=0;i<a->size;i++){
        tab[i]=v->val[i];
    }
    a->val=tab;
    return a;
}

wielomian add(wielomian v, wielomian y){
    if(v==NULL||y==NULL||(v->size>1&&y->size>1&&v->var[0]!=y->var[0])){
        msg("unsupported action");
        return NULL;
    }
    wielomian z=malloc(sizeof(Wielomian));
    z->size=max(v->size,y->size);
    if(v->size>1){
        z->var[0]=v->var[0];
    }
    else if(y->size>1){
        z->var[0]=y->var[0];
    }
    double* tab=calloc(z->size,sizeof(double));
    for(int i=0;i<z->size;i++){

        if(v->size>i){
            tab[i+(z->size-v->size)]+=v->val[i];
        }
        if(y->size>i){
            tab[i+(z->size-y->size)]+=y->val[i];
        }
    }
    z->val=tab;
    del(v);
    del(y);
    return z;
}

wielomian sub(wielomian v, wielomian y){
    if(v==NULL||y==NULL||(v->size>1&&y->size>1&&v->var[0]!=y->var[0])){
        msg("unsupported action");
        return NULL;
    }
    wielomian z=malloc(sizeof(Wielomian));
    z->size=max(v->size,y->size);
    if(v->size>1){
        z->var[0]=v->var[0];
    }
    else if(y->size>1){
        z->var[0]=y->var[0];
    }
    double* tab=calloc(z->size,sizeof(double));
    for(int i=0;i<z->size;i++){
        if(v->size>i){
            tab[i+(z->size-v->size)]+=v->val[i];
        }
        if(y->size>i){
            tab[i+(z->size-y->size)]-=y->val[i];
        }
    }
    z->val=tab;
    while(z->size>1&&z->val[0]==0){
        z->size--;
        for(int i=0;i<z->size;i++){
            z->val[i]=z->val[i+1];
        }
    }
    del(v);
    del(y);
    return z;
}

wielomian multiply(wielomian v, wielomian y){
    if(v==NULL||y==NULL||(v->size>1&&y->size>1&&v->var[0]!=y->var[0])){
        msg("unsupported action");
        return NULL;
    }
    wielomian z=malloc(sizeof(Wielomian));
    z->size=v->size+y->size-1;
    if(v->size>1){
        z->var[0]=v->var[0];
    }
    else if(y->size>1){
        z->var[0]=y->var[0];
    }
    double* tab=calloc(z->size,sizeof(double));
    for(int i=0;i<v->size;i++){
        for(int j=0;j<y->size;j++){
            tab[i+j]+=v->val[i]*y->val[j];
        }
    }
    z->val=tab;
    del(v);
    del(y);
    return z;
}



wielomian divide(wielomian v, wielomian y,wielomian r,bool delete){
    if(v==NULL||y==NULL||(v->size>1&&y->size>1&&v->var[0]!=y->var[0])){
        msg("unsupported action");
        return NULL;
    }
    wielomian z=malloc(sizeof(Wielomian));
    z->size=v->size-y->size+1;
    if(v->size>1){
        z->var[0]=v->var[0];
    }
    else if(y->size>1){
        z->var[0]=y->var[0];
    }

    wielomian temp=copy(v);
    *r=*temp;
    if(z->size<=0){
        free(z);
        z=from_d_to_w(0);
        return z;
    }
    double* tab=calloc(z->size,sizeof(double));
    for(int i=0;i<z->size;i++){
        tab[i]=r->val[0]/y->val[0];
        for(int j=0;j<y->size-1;j++){
            r->val[j]=r->val[j+1]-tab[i]*y->val[j+1];
        }
        for(int j=y->size-1;j<r->size-1;j++){
            r->val[j]=r->val[j+1];
        }
        r->size--;
    }
    while(r->size>1&&r->val[0]==0){
        r->size--;
        for(int i=0;i<r->size;i++){
            r->val[i]=r->val[i+1];
        }
    }
    z->val=tab;
    if(delete){
        del(v);
        del(y);
    }
    return z;
}

bool greater_than(wielomian v, wielomian y){
    if(v==NULL){
        return false;
    }
    if(v==NULL||y==NULL||(v->size>1&&y->size>1&&v->var[0]!=y->var[0])){
        msg("unsupported action");
        return false;
    }
    if(v->size>y->size){
        return true;
    }
    if(v->size==y->size&&v->val[0]>y->val[0]){
        return true;
    }
    return false;
}

wielomian nwd(wielomian v, wielomian y){
    if(v==NULL||y==NULL||(v->size>1&&y->size>1&&v->var[0]!=y->var[0])){
        msg("unsupported action");
        return NULL;
    }
    wielomian a=copy(v);
    wielomian b=copy(y);
    wielomian r=NULL;
    wielomian zero=from_d_to_w(0);
    while(greater_than(b,zero)){
        wielomian a1=a;
        wielomian temp=divide(a,b,r,false);
        del(temp);
        a=b;
        b=r;
        del(a1);
    }
    if(b!=NULL){
        del(b);
    }
    del(zero);
    if(a->val[0]<0){
        for(int i=0;i<a->size;i++){
            a->val[i]=-a->val[i];
        }
    }
    del(v);
    del(y);
    return a;
}

wielomian derivative(wielomian v){
    wielomian wynik=malloc(sizeof(Wielomian));
    if(v->size==1){
        wynik=from_d_to_w(0);
    }
    else{
        wynik->size=v->size-1;
        wynik->var[0]=v->var[0];
        double* temp_tab=calloc(wynik->size,sizeof(double));
        for(int i=0;i<wynik->size;i++){
            temp_tab[i]=v->val[i]*(v->size-1-i);
        }
        wynik->val=temp_tab;
    }
    return wynik;
}

double abs_d(double d){
    if(d<0){
        return -d;
    }
    return d;
}

wielomian m_zero(wielomian v){
    wielomian pochodna=derivative(v);
    double punkt_pocztkowy=5;
    double temp=0;
    double dokladnosc=0.0001;
    do{
        temp=punkt_pocztkowy;
        wielomian t=from_d_to_w(temp);
        punkt_pocztkowy=temp-(compute(v,t,false))/(compute(pochodna,t,false));
        free(t);
    }while(abs_d(punkt_pocztkowy-temp)>=dokladnosc);
    return from_d_to_w(punkt_pocztkowy);
}

wielomian from_d_to_w(double d){
    wielomian wynik=malloc(sizeof(Wielomian));
    wynik->size=1;
    double* temp_tab=calloc(1,sizeof(double));
    temp_tab[0]=d;
    wynik->val=temp_tab;
    return wynik;
}

void del(wielomian v){
    free(v->val);
    free(v);
}