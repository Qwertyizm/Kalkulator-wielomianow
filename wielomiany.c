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



char* print(wielomian v){
    char* wynik=malloc((8*v->size)*sizeof(char));
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
    sprintf(temp,"%.2f",v->val[v->size-1]);
    strcat(wynik,temp);
    del(v);
    return wynik;
}

wielomian compute(wielomian v, wielomian x){
    wielomian wynik=malloc(sizeof(Wielomian));
    wynik->size=1;
    double* w_tab=calloc(1,sizeof(double));
    for(int i=0;i<v->size;i++){
        w_tab[0]*=x->val[0];
        w_tab[0]+=v->val[i];
    }
    wynik->val=w_tab;
    del(v);
    del(x);
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
        z->size=1;
        double* tab=calloc(1,sizeof(double));
        tab[0]=0;
        z->val=tab;
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
    wielomian zero=malloc(sizeof(Wielomian));
    zero->size=1;
    double* z_tab=calloc(1,sizeof(double));
    z_tab[0]=0;
    zero->val=z_tab;
    zero->var[0]=b->var[0];
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

void del(wielomian v){
    free(v->val);
    free(v);
}