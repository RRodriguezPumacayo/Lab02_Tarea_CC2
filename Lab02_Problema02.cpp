#include<iostream>

using namespace std;

void llenar(long long *v, long long n){
    for(int i=1;i<=n;i++)
        *(v+i)=i;
}

void crear(long long *&v, long long n){//*&v toma la dirección del contenido de la ubicación de la memoria a la que apunta el puntero ...
                                       //lo que significa el valor del puntero en sí
    v = new long long[n];//La primera expresión se usa para asignar memoria para contener un solo elemento de tipo .
                         //El segundo se utiliza para asignar un bloque (una matriz) de elementos de tipo ,
                         //donde es un valor entero que representa la cantidad de estos.
}

long long *reversa(long long *v,long long n){

    while(n>=1){
        *(v+n)=n;
        cout<<*(v+n)<<" ";
        --n;
    }
}

int main(){
    long long n;
    long long *v,*v2;
    cout<<"Cantidad de elementos? ";cin>>n;
    crear(v,n);
    llenar(v,n);
    reversa(v,n);

}
