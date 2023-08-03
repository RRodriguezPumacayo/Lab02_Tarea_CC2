#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <list>
#include <vector>
#include <iomanip>

using namespace std;

const int PAGE_SIZE = 4096; // Tamaño fijo de la página
const int MAX_KEYS = 2;
const int MAX_CHILDREN = MAX_KEYS+1;

class BNodo{
    public:
        bool isLeaf;
        int* keys;
        BNodo** hijos;
        BNodo* sigHoja;

    BNodo(){
        std::cout<<"Nodo Creado\n";
        keys = new int[MAX_KEYS];
        hijos = new BNodo*[3];
    }
};

void insertarNodo(int key, BNodo *&arbol){
    int aux;
    /*
    for(int i=0;i<MAX_KEYS-1;i++){
        for(int j=i+1;j<MAX_KEYS;j++){
            if(arbol->keys[i] > arbol->keys[j]){
            aux = arbol->keys[i];
            arbol->keys[i] = arbol->keys[j];
            arbol->keys[j] = aux;
            }
        }
    }
    */
    for(int i=0;i<MAX_KEYS;i++){
        if(arbol->keys[i] != 0){
            arbol->keys[i] = key;
            std::cout<<"i: "<<arbol->keys[i]<<std::endl;
            return;
        }
    }
}

void mostrarNodo(BNodo *&arbol){
    std::cout<<"Mostrando: ";
    for(int i=0;i<MAX_KEYS;i++){
        std::cout<<arbol->keys[i]<<"\t";
    }
}

int main() {
    BNodo* arbol = new BNodo();
    insertarNodo(10, arbol);
    insertarNodo(12, arbol);
    insertarNodo(14, arbol);
    mostrarNodo(arbol);
    return 0;
}
