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

void random(long long *v,long long n){
    long long aleatorio, desde=1, hasta=n;
    for(int i=0;i<n;i++){
        aleatorio = rand()%(hasta-desde+1)+desde;
        *(v+i)=aleatorio;
    }
}

void ascendente(long long *v,long long n){
    int i;
    long long aux;
    for(i=0;i<n;i++){
        for(int j=i+1;j<n;j++){
            if(*(v+i)>*(v+j)) //9,4,6
            {
                aux=*(v+i);
                *(v+i)=*(v+j);
                *(v+j)=aux;
            }
        }
    }

    cout<<"\nVector ordenado de forma ascendente\n";

    for(i=0;i<n;i++){
        cout<<*(v+i)<<" ";
    }
}

int main(){
    long long n;
    long long *v;
    cout<<"Cantidad de elementos? ";cin>>n;
    crear(v,n);
    llenar(v,n);
    random(v,n);
    ascendente(v,n);


}
