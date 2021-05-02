#include<iostream>
#include<string.h>

using namespace std;

void lexico(char *cadena[], long long tamanio){
    int div=tamanio/26;
    int res=tamanio-(div*26);
    long long j=0;
    long z=26;
    for(int i=0;i<25;i++){
        cadena[i];
        j++;
        cout<<div<<" veces ("<<cadena[i]<<","<<j<<") "<<endl;
        if((div*26)+res==tamanio)
            cout<<div+res<<" veces ("<<cadena[i+1]<<","<<j+1<<") "<<endl;
    }
}

int main(){
    long long tamanio=1000000;
    int d=tamanio/26;
    char *cadena[26]={"a","b","c","d","e","f","g","h","i","j","k","l","m","n","o","p","q","r","s","t","u","v","w","x","y","z"};

    cout<<"Tamanio de la cadena = "<<tamanio<<endl;
    lexico(cadena,tamanio);

}
