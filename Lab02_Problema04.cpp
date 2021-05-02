#include<iostream>

using namespace std;

int tamanio(char cadena[],int i){
    while(cadena[i]!='\0'){
        cout<<i+1<<" ";
        i++;
    }
    return tamanio(cadena,i);
}

int main(){
    int i=0;
    char cadena[]={"1234567890"};

    cout<<"Cadena: "<<cadena<<endl;

    tamanio(cadena,i);
}
