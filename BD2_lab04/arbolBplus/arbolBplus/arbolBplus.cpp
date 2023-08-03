#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <list>
#include <vector>
#include <iomanip>

using namespace std;

const int PAGE_SIZE = 4096; // Tamaño fijo de la página
const int MAX_KEYS = 5;
const int MAX_CHILDREN = 5;

struct Nodo {    
    bool isLeaf;
    Nodo** hijo;
    int numKey;
    Nodo* siguienteHoja;    
};

class BPlusNode {
    public:
        bool isLeaf;
        int* keys;
        BPlusNode** hijos;
        int numKey;
        BPlusNode* nextLeaf; // Apuntador al siguiente nodo hoja (solo en nodos hoja)

        BPlusNode(bool leaf = false);
        ~BPlusNode();
};

    BPlusNode::BPlusNode(bool leaf) {
        isLeaf = leaf;
        keys = new int[MAX_KEYS]; // MAX_KEYS es un valor que defines según el orden del árbol
        hijos = new BPlusNode * [MAX_CHILDREN]; // MAX_CHILDREN es un valor que defines según el orden del árbol
        nextLeaf = nullptr;
    }

    BPlusNode::~BPlusNode() {
        delete[] keys;
        delete[] hijos;
    }

class BPlusTree {
    public:
        BPlusTree();
        ~BPlusTree();

        void remove(int key);
        bool search(int key);
        void print();
    private:
        BPlusNode* root;
        // Otras funciones privadas de ayuda para la inserción, eliminación, búsqueda y recorrido del árbol
};

BPlusNode* crearNodo(int numKey) {
    BPlusNode* nuevoNodo = new BPlusNode();
    nuevoNodo->numKey = numKey;
}

void insert(int key, BPlusNode*& arbol) {
    BPlusNode* nuevoNodo = crearNodo(key);
    arbol = nuevoNodo;
}

int main() {

    BPlusTree* arbol = NULL;
    insert(20, arbol);
    return 0;
}