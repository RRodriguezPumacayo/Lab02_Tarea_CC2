#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <list>
#include <vector>
#include <iomanip>

using namespace std;

const int PAGE_SIZE = 4096; // Tamaño fijo de la página

class GestorAlmacenamiento {
private:
    string nombreArchivo;
    int numTotalPags;
    int posPag;

public:
    GestorAlmacenamiento(const string& _nombreArchivo) : nombreArchivo(_nombreArchivo), numTotalPags(0), posPag(0) {}
    // Constructor de la clase GestorAlmacenamiento que recibe el nombre del archivo a gestionar



    // Abre el archivo en modo binario y lee todo su contenido en el string fileContent
    void leerBloque(int numPag, string& buffer) {
        ifstream file(nombreArchivo, ios::binary);  // se abre el archivo
        stringstream ss;  				//archivo en formato de cadena
        ss << file.rdbuf();				// se copia el contenido del archivo al obj ss 
        string fileContent = ss.str();



        // Calcula la posición de inicio del bloque de datos en el archivo
        int startPos = (numPag + 1) * PAGE_SIZE + 4;
        if (startPos >= fileContent.length()) {
            cout << "Error: El archivo no tiene suficientes páginas." << endl;
            return;
        }


        // Obtiene el bloque de datos del archivo y lo almacena en el parámetro buffer
        buffer = fileContent.substr(startPos, PAGE_SIZE);

        file.close();
    }
    // Escribe un bloque de datos en la posición numPag del archivo
    void escribirBloque(int numPag, const string& data) {
        fstream file(nombreArchivo, ios::binary | ios::in | ios::out);
        file.seekp((numPag)*PAGE_SIZE + 4, ios::beg);
        file.write(data.c_str(), data.size());
        file.close();
    }
};

class Page {
public:
    string data;
    bool dirty;
    int pin_count;
    // Constructor de la clase Page que inicializa las variables dirty y pin_count en false y 0 respectivamente
    Page() : dirty(false), pin_count(0) {}
};


// Enumeracin que representa las estrategias de reemplazo de paginas
enum ReplacementStrategy {
    LRU,
    MRU,
    Clock
};

class BufferPoolManager {
private:
    int numFrames;			// numero de frames(paginas) disponibles en el bufer
    unordered_map<int, Page> pageTable;  // mapea un numero de pagina aun objeto de la clase Page
    list<int> freeList;		// lista enlazada que sigue los indices de  los frames disponivles 
    list<int> lruList;		// lista enl. seguimiento del orden de acceso de las paginas, la menos reciente util
    list<int> mruList;		// lista enl. seguimiento  del orden de acceso de las paginas en el buf, la mas reciente util
    vector<int> clockList;	// vector almacena 0 o 1 para cada frame en en bvufer
    int clockHand;			// entero que indica pos actual del puntero  en el algort "clock"
    GestorAlmacenamiento storageManager;  // objeto de GestorAlmacenamiento para escr y leerbloque
    ReplacementStrategy replacementStrategy;  // enumerador que indica la estrategia de remplazo

public:
    // Constructor de la clase BufferPoolManager que inicializa las variables y estructuras de datos

    BufferPoolManager(int _numFrames, const string& archivo, ReplacementStrategy _replacementStrategy)
        : numFrames(_numFrames), clockHand(0), storageManager(archivo), replacementStrategy(_replacementStrategy) {
        for (int i = 0; i < numFrames; ++i) {
            freeList.push_back(i);
            clockList.push_back(0);
        } // clockHand = 0 indica pos actual de la lista circular, storageManager=archivo para administrar almac en disco
    }		// repacementStrategy estrategia de remplazo



    // Obtiene una página del buffer pool segun el ID de la pagina
    Page* FetchPage(int page_id) {
        // La pagina ya está en el buffer pool
        if (pageTable.count(page_id) > 0) {

            UpdateLRUList(page_id);
            return &pageTable[page_id];
        }
        else {	// La pagina no está en el buffer pool, se debe obtener del almacenamiento

            /// No hay marcos libres disponibles, se debe desalojar una página usando la estrategia de reemplazo seleccionada
            if (freeList.empty()) {

                // se llama a la funcion EvictPag para selecc la  pagina a desalojar	
                int evicted_page_id = EvictPage();
                //int evicted_page_id = EvictPage();
                Page& evicted_page = pageTable[evicted_page_id];

                // La pgina es modificada, se debe escribir de vuelta en el almacenamiento
                if (evicted_page.dirty) {	// significa qye se deben escribir los cambios de la pag antes d desalojarla

                    FlushPage(evicted_page_id);  // se llama a Fulshpage para escribir los datos
                }
                pageTable.erase(evicted_page_id);  // se elimian la entrada
                freeList.push_front(evicted_page_id); // se agrega marco libre a la lista freeList
                clockList[evicted_page_id] = 0;  // y se actualiza para reglejar la pag fue desalojada
            }
            int frame_id = freeList.front();  // se obtiene el identific del primer marco libre de freelist
            freeList.pop_front();       // se elimina el primer elemento ya q se usara como marc para pag actual
            Page& page = pageTable[page_id];   // se obtiene referencia a la pagina_id en el unordered_map
            page.data = FetchPageDataFromStorage(page_id); // se obtiene datos de la pagina
            page.dirty = false;  // ya que la pag se acaba d cargar y aun no a sido moficiada
            page.pin_count = 1;  // indica qe la pag esta siendo utilizada por almenos 1 referencia
            UpdateLRUList(page_id);  // se actualiza la lista LRU para reflejar q la pag fue accedida recientemente
            return &page;   // devuelve puntero a la pagina desde la funcio nFecthPage
        }
    }
    // Desmarca una pagina del buffer pool
    void UnpinPage(int page_id, bool is_dirty) {
        if (pageTable.count(page_id) > 0) {  // verifica si la pagina_id esta presente en el bufer
            Page& page = pageTable[page_id];
            page.pin_count--;
            // Escribe los datos de la pagina en el almacenamiento
            if (is_dirty) { // si la pagina esta sucia
                page.dirty = true;
            }
            UpdateLRUList(page_id);
        }
    }

    bool FlushPage(int page_id) {
        if (pageTable.count(page_id) > 0) {
            Page& page = pageTable[page_id];
            if (page.dirty) {
                // Write page data to storage
                WritePageDataToStorage(page_id, page.data);
                page.dirty = false;
                return true;
            }
        }
        return false;
    }
    // Escribe todas las páginas del buffer pool en el almacenamiento
    void FlushAllPages() {
        for (const auto& entry : pageTable) {
            int page_id = entry.first;
            FlushPage(page_id);
        }
    }

private:
    // Obtiene los datos de una página desde el almacenamiento
    string FetchPageDataFromStorage(int page_id) {
        string buffer;
        storageManager.leerBloque(page_id, buffer);
        return buffer;
    }


    // Escribe los datos de una página en el almacenamiento
    void WritePageDataToStorage(int page_id, const string& data) {
        storageManager.escribirBloque(page_id, data);
    }

    // Actualiza la lista LRU
    void UpdateLRUList(int page_id) {
        lruList.remove(page_id);
        lruList.push_front(page_id);
    }

    // Actualiza la lista MRU
    void UpdateMRUList(int page_id) {
        mruList.remove(page_id);
        mruList.push_front(page_id);
    }


    // Realiza el desalojo de una página según la estrategia de reemplazo seleccionada
    int EvictPage() {
        int evicted_page_id;
        switch (replacementStrategy) {
        case LRU:
            evicted_page_id = lruList.back();
            lruList.pop_back();
            break;
        case MRU:
            evicted_page_id = mruList.back();
            mruList.pop_back();
            break;
        case Clock:
            while (true) {
                if (clockList[clockHand] == 0) {
                    evicted_page_id = clockHand;
                    clockHand = (clockHand + 1) % numFrames;
                    break;
                }
                else {
                    clockList[clockHand] = 0;
                    clockHand = (clockHand + 1) % numFrames;
                }
            }
            break;
        }
        return evicted_page_id;
    }
};

// Genera un archivo con registros de longitud fija
void generateFixedLengthFile(const string& filename) {
    ifstream inputFile("titanic.csv");
    ofstream outputFile(filename);

    if (!inputFile || !outputFile) {
        cout << "Error: No se pudo abrir el archivo." << endl;
        return;
    }

    // Leer el archivo de entrada y escribir en el archivo de salida
    string line;
    while (getline(inputFile, line)) {
        // Agregar cabecera a cada página
        if (outputFile.tellp() % PAGE_SIZE == 0) {
            outputFile << "Cabecera" << endl;
        }

        // Leer los campos del registro CSV
        stringstream ss(line);
        string field;
        vector<string> fields;
        while (getline(ss, field, ',')) {
            fields.push_back(field);
        }

        // Escribir los campos con tamaño fijo en el archivo de salida
        for (const string& field : fields) {
            // Asegurar un tamaño fijo para cada campo
            string fixedLengthField = field.substr(0, 13);
            fixedLengthField.resize(13, ' ');

            outputFile << fixedLengthField;
        }

        outputFile << endl;
    }

    inputFile.close();
    outputFile.close();

    cout << "Archivo generado: " << filename << endl;
}


// Genera un archivo con registros de longitud variable
void generateVariableLengthFile(const string& filename) {
    ifstream inputFile("titanic.csv");
    ofstream outputFile(filename);

    if (!inputFile || !outputFile) {
        cout << "Error: No se pudo abrir el archivo." << endl;
        return;
    }

    // Leer el archivo de entrada y escribir en el archivo de salida
    string line;
    getline(inputFile, line); // Leer y almacenar la primera línea (cabecera)

    // Escribir los nombres de los campos al principio del archivo de salida
    outputFile << "Cabecera" << endl;
    outputFile << line << endl;

    while (getline(inputFile, line)) {
        // Escribir el tamaño del registro en el archivo de salida
        int recordSize = line.size();
        outputFile << recordSize << endl;

        // Escribir el registro en el archivo de salida
        outputFile << line << endl;
    }

    inputFile.close();
    outputFile.close();

    cout << "Archivo generado: " << filename << endl;
}




int main() {
    const string filenameFixed = "fixed_length2.txt";
    const string filenameVariable = "variable_length2.txt";

    // Generar archivo con registros de longitud fija
    generateFixedLengthFile(filenameFixed);

    // Generar archivo con registros de longitud variable
    generateVariableLengthFile(filenameVariable);


    // Crear una instancia de BufferPoolManager
   //BufferPoolManager bufferManager(10, filenameFixed, ReplacementStrategy::LRU);

   // Ejemplo de uso de FetchPage
   //Page* page1 = bufferManager.FetchPage(0);
   // Manipular la página...

   // Ejemplo de uso de FlushPage
   //bufferManager.FlushPage(0);

   // Ejemplo de uso de UnpinPage
   //bufferManager.UnpinPage(0, true);

   // Ejemplo de uso de FlushAllPages
   //bufferManager.FlushAllPages();

    return 0;

}




