#include<iostream>
#include<string.h>

using namespace std;

int longitud(char cadena[])
{
    int i;

    for(i=0;cadena[i]!='\0';i++);
    return i;
}

void concatena(char cadena1[], char cadena2[])
{
     int i,j,k;
     k=0;
     j=longitud(cadena1);


     for(i=0;i<longitud(cadena2);i++){
        cadena1[j]=cadena2[i];
        j++;
     }
}

int main()
{
    char cad1[20], cad2[20];

    printf("Introduzca la primera cadena: ");
    gets(cad1);

    printf("Introduzca la segunda cadena: ");
    gets(cad2);

    concatena(cad1, cad2);

    printf("La cadena concatenada es %s\n", cad1);

    system("pause");
    return 0;
}
