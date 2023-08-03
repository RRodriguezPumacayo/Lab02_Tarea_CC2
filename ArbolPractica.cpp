#include<iostream>

struct Nodo{
    int dato;
    Nodo *der;
    Nodo *izq;
    Nodo *padre;
};

Nodo *crearNodo(int dato, Nodo *padre){
    Nodo *nuevoNodo = new Nodo();
    nuevoNodo->dato = dato;
    nuevoNodo->der = NULL;
    nuevoNodo->izq = NULL;
    nuevoNodo->padre = padre;
}

void insertarNodo(Nodo *&arbol, int dato, Nodo *padre)
{
    if(arbol==NULL){
        Nodo *nuevoNodo = crearNodo(dato, padre);
        arbol = nuevoNodo;
    }
    else if(dato<arbol->dato){
        insertarNodo(arbol->izq, dato, arbol);
    }else{
        insertarNodo(arbol->der, dato, arbol);
    }
}

void mostrarArbol(Nodo *&arbol, int contador)
{
    if(arbol==NULL){
        return;
    }else{
        mostrarArbol(arbol->der, contador+1);
        for(int i=0;i<contador;i++){
            std::cout<<"    ";
        }
        std::cout<<arbol->dato<<std::endl;
        mostrarArbol(arbol->izq, contador+1);
    }
}

bool buscarArbol(Nodo *&arbol, int encontrar){
    if(arbol==NULL){
        std::cout<<"Dato "<<encontrar<<" NO ENCONTRADO\n";
        return false;
    }else if(arbol->dato==encontrar){
        std::cout<<"Dato "<<encontrar<<" ENCONTRADO\n";
        return true;
    }else if(encontrar<arbol->dato){
        buscarArbol(arbol->izq, encontrar);
    }else{
        buscarArbol(arbol->der, encontrar);
    }
}

void preOrden(Nodo *&arbol)
{
    if(arbol==NULL){
        return;
    }
    preOrden(arbol->izq);
    std::cout<<arbol->dato<<" - ";
    preOrden(arbol->der);
}

void inOrden(Nodo *&arbol)
{
    if(arbol==NULL){
        return;
    }
    std::cout<<arbol->dato<<" - ";
    inOrden(arbol->izq);
    inOrden(arbol->der);
}

void postOrden(Nodo *&arbol)
{
    if(arbol==NULL){
        return;
    }
    postOrden(arbol->izq);
    postOrden(arbol->der);
    std::cout<<arbol->dato<<" - ";
}

Nodo *minimoNodo(Nodo *&arbol){
    if(arbol==NULL){
        return NULL;
    }else if(arbol->izq){
        return minimoNodo(arbol->izq);
    }else{
        return arbol;
    }
}

void destruirNodo(Nodo *nodoDestruir){
    nodoDestruir->izq=NULL;
    nodoDestruir->der=NULL;
    nodoDestruir->padre=NULL;
    delete nodoDestruir;
}

void reemplazarNodo(Nodo *&arbol, Nodo *nuevo){
    if(arbol->padre){
        if(arbol->padre->izq!=NULL){
            if(arbol->padre->izq->dato==arbol->dato){
                arbol->padre->izq = nuevo;
            }
        }
        if(arbol->padre->der!=NULL){
            if(arbol->padre->der->dato==arbol->dato){
                arbol->padre->der = nuevo;
            }
        }
        if(nuevo){
            nuevo->padre = arbol->padre;
        }
    }
}

void eliminarNodo(Nodo *& arbol){
    if(arbol->izq && arbol->der){
        std::cout<<"Nodo CON AMBOS HIJOS\n";
        Nodo *minimo = minimoNodo(arbol->der);
        arbol->dato = minimo->dato;
        eliminarNodo(minimo);
    }else if(arbol->izq){
        std::cout<<"Nodo IZQUIERDO\n";
        reemplazarNodo(arbol, arbol->izq);
    }else if(arbol->der){
        std::cout<<"Nodo DERECHO\n";
        reemplazarNodo(arbol, arbol->der);
    }else{
        std::cout<<"Nodo SIN HIJOS\n";
        reemplazarNodo(arbol, NULL);
    }
}

void buscarEliminarNodo(Nodo *&arbol, int eliminar)
{
    if(arbol==NULL){
        std::cout<<"Nodo NO encontrado\n";
        return;
    }else if(eliminar<arbol->dato){
        buscarEliminarNodo(arbol->izq, eliminar);
    }else if(eliminar>arbol->dato){
        buscarEliminarNodo(arbol->der, eliminar);
    }else{
        std::cout<<"Nodo Encontrado\n";
        eliminarNodo(arbol);
    }
}

int main()
{
    Nodo *arbol = NULL;
    insertarNodo(arbol, 5, NULL);
    insertarNodo(arbol, 4, NULL);
    insertarNodo(arbol, 3, NULL);
    insertarNodo(arbol, 2, NULL);
    insertarNodo(arbol, 1, NULL);
    insertarNodo(arbol, 6, NULL);
    insertarNodo(arbol, 7, NULL);
    insertarNodo(arbol, 8, NULL);
    insertarNodo(arbol, 9, NULL);
    mostrarArbol(arbol,0);
    std::cout<<"Pre-Orden\n";
    preOrden(arbol);
    std::cout<<std::endl;
    std::cout<<"In-Orden\n";
    inOrden(arbol);
    std::cout<<std::endl;
    std::cout<<"Post-Orden\n";
    postOrden(arbol);
    std::cout<<std::endl;
    buscarEliminarNodo(arbol, 8);
    buscarEliminarNodo(arbol, 2);
    mostrarArbol(arbol,0);
    return 0;
}
