// Instituto Tecnológico de Costa Rica
// Carrera de Ingeniería Electrónica
// Curso de Análisis y Diseño de Algoritmos
// Grupo 21
// Laboratorio de Semana 15
// Estudiante programador: José David Montoya Vargas
// Profesor: Bryan Hernández Sibaja
// Alajuela, Costa Rica
// II Semestre, 2025

#include <iostream>		 // Biblioteca para entrada y salida de datos.	
#include <string>		 // Biblioteca para manejar cadenas de texto.
#include <unordered_set> // Biblioteca para conjuntos no ordenados.
#include <limits>		 // Biblioteca para manejar límites numéricos y evitar problemas de entrada. (cin y getline)

using namespace std;

struct Nodo; // Se prototipa la estructura para usarla en Arista.

// Nodo de la lista de adyacencia (aristas).
struct Arista {
	Nodo* destino;		// Puntero al nodo conectado.
	Arista* siguiente;  // Puntero a la siguiente conexión en la lista.

	// Constructor para inicializar la conexión con un nodo.
	Arista(Nodo* nodo) : destino(nodo), siguiente(nullptr) {}
};

// Nodo del grafo (lista de nodos).
struct Nodo {
	string nombre;
	Arista* adyacente; // Puntero cabeza de la lista de adyacencia (las conexiones).
	Nodo* siguiente;   // Puntero al siguiente nodo en la lista del grafo.

	Nodo(const string& nom) : nombre(nom), adyacente(nullptr), siguiente(nullptr) {}
};

// Función para destruir el grafo y liberar memoria.
void liberar_grafo(Nodo*& grafo) {
	while (grafo != nullptr) { // Se recorre cada nodo del grafo.
		// Se borra la lista de adyacencia de este nodo.
		Arista* arista = grafo->adyacente;

		while (arista != nullptr) { // Se recorren y borran todas las aristas.
			Arista* auxArista = arista;
			arista = arista->siguiente;

			delete auxArista;
		}
		// Se borra el nodo.
		Nodo* auxNodo = grafo;
		grafo = grafo->siguiente;

		delete auxNodo;
	}
}

// Nivel 1 (30pts) : Implementación de un grafo
// Cree un programa que permita crear un grafo no dirigido donde:
//	1. El usuario indica cuántos nodos iniciales tendrá el grafo.
//	2. El usuario ingresa los nombres de los nodos, además de los grafos con los cuales tendrá
//	   conexión.
//	3. Luego, el usuario podrá :
//		o Agregar un nuevo nodo al grafo.
//		o Agregar nuevas aristas relacionadas con ese nodo.Tome en cuenta los nodos
//		  previamente creados.

Nodo* buscar_nodo(Nodo* grafo, const string& nombre) {
	Nodo* actual = grafo; // grafo es el puntero al primer nodo de la lista.

	while (actual != nullptr) { // Se recorre la lista de nodos y se retorna si encuentra uno con el nombre ingresado.
		if (actual->nombre == nombre) {
			return actual;
		}
		actual = actual->siguiente;
	}

	return nullptr; // Si no se encuentra se retorna nulo.
}

void agregar_nodo(Nodo*& grafo, const string& nombre) {
	// Se evitan nombres repetidos.
	if (buscar_nodo(grafo, nombre) != nullptr) {
		cout << "Ya existe un nodo con el nombre '" << nombre << "'. En este caso, no se agregara.\n";
		return;
	}

	Nodo* nuevo = new Nodo(nombre);

	if (grafo == nullptr) {
		grafo = nuevo; // Se crea el primer nodo.
	}
	else {
		// Se inserta al final para mantener orden de creación.
		Nodo* actual = grafo;

		while (actual->siguiente != nullptr) {
			actual = actual->siguiente;
		}
		actual->siguiente = nuevo;
	}
}

// Primero se agrega una conexión simple de un nodo a otro, esto para evitar duplicar código y separar responsabilidades.
void agregar_conexion_simple(Nodo* origen, Nodo* destino) {
	if (origen == nullptr || destino == nullptr) return; // Validación básica.

	// Se evita duplicar aristas.
	Arista* actual = origen->adyacente;

	while (actual != nullptr) {
		if (actual->destino == destino) {
			return; // En este caso ya la conexion existe.
		}
		actual = actual->siguiente;
	}

	// Se inserta al inicio de la lista de adyacencia (conexiones).
	Arista* nueva = new Arista(destino);
	nueva->siguiente = origen->adyacente;
	origen->adyacente = nueva;
}

// Luego se usa esta función para agregar conexiones no dirigidas, como se necesita.
void agregar_conexion_no_dirigida(Nodo* grafo, const string& nombreA, const string& nombreB) {
	if (nombreA == nombreB) {
		cout << "\nNo se permite conectar un nodo consigo mismo.\n";
		return;
	}

	Nodo* nodoA = buscar_nodo(grafo, nombreA);
	Nodo* nodoB = buscar_nodo(grafo, nombreB);

	if (nodoA == nullptr || nodoB == nullptr) {
		cout << "\nAlguno de los nodos no existe. No se agrego la conexion.\n";
		return;
	}

	// Como el grafo es no dirigido, se agregan ambas direcciones.
	agregar_conexion_simple(nodoA, nodoB);
	agregar_conexion_simple(nodoB, nodoA);
}

// Para llevar el control de los cambios se crea la función para imprimir el grafo.
void imprimir_grafo(Nodo* grafo) {
	Nodo* actual = grafo;

	cout << "\n| ------ Grafo actual ------ |\n\n";
	
	while (actual != nullptr) {
		cout << "Nodo '" << actual->nombre << "' -> ";

		if (actual->adyacente == nullptr) { // Si no tiene conexiones.
			cout << "sin conexiones";
		}
		else {
			Arista* arista = actual->adyacente; // Se recorre la lista de adyacencia, es decir, las conexiones.
			bool primero = true;

			while (arista != nullptr) {
				if (!primero) cout << ", "; // Solo se agrega coma si no es el primero.
				
				cout << arista->destino->nombre;

				primero = false; // Indica que ya se imprimió el primer nodo.
				arista = arista->siguiente;
			}
		}
		cout << endl;

		actual = actual->siguiente;
	}
	cout << "\n| -------------------------- |\n\n";
}

void nivel_1(Nodo*& grafo) {
	string nombre = "";
	int opcion = 0, conexiones = 0, cantidadNodosIniciales = 0, contadorNodos = 0;

	cout << "| ------------------------- Nivel 1 : Implementacion de un grafo ------------------------- |\n\n"
		 << "Ingrese la cantidad de nodos iniciales que tendra el grafo: "; cin >> cantidadNodosIniciales; cout << endl;
	cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Se limpia el buffer de entrada.

	// Se ingresan los nombres de los nodos iniciales.
	for (int i = 0; i < cantidadNodosIniciales; ++i) {
		cout << "Ingrese el nombre del nodo " << (i + 1) << ": "; getline(cin, nombre);
		agregar_nodo(grafo, nombre);
	}
	cout << endl;

	// Se ingresan las conexiones para los nodos iniciales.
	Nodo* actual = grafo;

	while (actual != nullptr && contadorNodos < cantidadNodosIniciales) { // Se recorre cada nodo para agregar sus conexiones.
		conexiones = 0;

		// Se calcula la cantidad máxima de conexiones posibles para este nodo.
		int maxConexiones = cantidadNodosIniciales - 1;

		// Se valida que el usuario no ingrese más conexiones de las permitidas.
		do {
			if (conexiones < 0 || conexiones > maxConexiones) {
				cout << "\nEste nodo solo puede conectarse con " << maxConexiones
					 << " nodos como maximo. Intentelo de nuevo.\n\n";
			}
			cout << "Ingrese la cantidad de conexiones para el nodo "
				 << actual->nombre << " (max " << maxConexiones << "): "; cin >> conexiones;
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
		} while (conexiones < 0 || conexiones > maxConexiones);

		for (int j = 0; j < conexiones; ++j) {
			string nombreConexion;

			cout << "  Ingrese el nombre del nodo con el cual se conectara " << actual->nombre << ": ";
			getline(cin, nombreConexion);

			agregar_conexion_no_dirigida(grafo, actual->nombre, nombreConexion);
		}
		cout << endl;

		actual = actual->siguiente;
		contadorNodos++;
	}
	imprimir_grafo(grafo);

	// Menú con el que se podrá seguir manipulando el grafo.
	do {
		cout << "| --------------- Gestion del grafo creado --------------- |\n\n"
			 << " [ 1 ] Agregar un nuevo nodo.\n"
			 << " [ 2 ] Agregar una nueva arista entre nodos existentes.\n"
			 << " [ 3 ] Imprimir grafo.\n"
			 << " [ 0 ] Salir.\n\n"
			 << "| -------------------------------------------------------- |\n"
			 << "Digite el numero de opcion a la cual desea ingresar: "; cin >> opcion; cout << endl;
		cin.ignore(numeric_limits<streamsize>::max(), '\n');

		// Bloque de condicionales para el menú.
		if (opcion == 1) {
			cout << "Ingrese el nombre del nuevo nodo: "; getline(cin, nombre);
			cout << endl;

			agregar_nodo(grafo, nombre);
		}
		else if (opcion == 2) {
			string nombreA = "", nombreB = "";

			cout << "Nombre del primer nodo: "; getline(cin, nombreA);
			cout << "Nombre del segundo nodo: "; getline(cin, nombreB);

			agregar_conexion_no_dirigida(grafo, nombreA, nombreB);

			cout << endl;
		}
		else if (opcion == 3) {
			imprimir_grafo(grafo);
		}
	} while (opcion != 0);
}


// Nivel 2 (30pts) : Cantidad de nodos
// Cree una función que cuente la cantidad de nodos que tiene un grafo.
// 
// Entrada : Puntero a nodo de grafo.
// 
// IMPORTANTE : Tome en cuenta que, al contar, no se debería de contar un mismo nodo más de una
// vez, por lo que la implementación de un mapeo(diccionario) o un conjunto, le ahorrará mucho
// trabajo.

void nivel_2(Nodo* grafo) {
	unordered_set<Nodo*> visitados; // Se crea un conjunto donde se guardarán los nodos ya contados.

	cout << "| ------------------------------ Nivel 2 : Cantidad de nodos ----------------------------- |\n\n";
		 
	Nodo* actual = grafo;

	// Se recorre la lista de nodos del grafo.
	while (actual != nullptr) {
		visitados.insert(actual); // El .insert evita duplicados.
		actual = actual->siguiente;
	}
	cout << "La cantidad de nodos en el grafo creado en el Nivel 1 es de: " << visitados.size() << endl << endl;
}


// Nivel 3 (40pts) : Imprimir nodos
// Escriba una función que reciba un puntero a alguno de los nodos de un grafo e imprima
// 
//	• Entrada : Puntero a nodo de grafo.
// 
// Tome en cuenta que, además de imprimir el nodo, deberá mostrar los nodos con los cuales tiene
// conexión.

void imprimir_nodo(Nodo* nodo) {
	cout << "| ----- Impresion del nodo ----- |\n\n"
		 << "Nodo '" << nodo->nombre << "' -> ";

	if (nodo->adyacente == nullptr) { // Si no tiene conexiones.
		cout << "sin conexiones";
	}
	else {
		Arista* arista = nodo->adyacente; // Se recorre la lista de adyacencia.
		bool primero = true;

		while (arista != nullptr) {
			if (!primero) cout << ", "; // Solo se agrega coma si no es el primero.

			cout << arista->destino->nombre;

			primero = false; // Indica que ya se imprimió el primer nodo.
			arista = arista->siguiente;
		}
	}
	cout << "\n\n| ------------------------------ |";
}

void nivel_3(Nodo* grafo) {
	if (grafo == nullptr) {
		cout << "| -------------------------------- Nivel 3 : Imprimir nodos ------------------------------ |\n\n";
		cout << "El grafo esta vacio. No hay nodos que imprimir.\n\n"; // En caso de que no se haya creado un grafo.
		return;
	}

	string nombre = "";
	Nodo* nodo = nullptr;

	cout << "| -------------------------------- Nivel 3 : Imprimir nodos ------------------------------ |\n\n";

	while (nodo == nullptr) {
		cout << "Ingrese el nombre del nodo que desea imprimir: "; getline(cin, nombre); cout << endl;

		nodo = buscar_nodo(grafo, nombre); // Si lo encuentra, retorna el puntero al nodo, sino retorna nullptr.

		if (nodo == nullptr) {
			cout << "El nodo con nombre '" << nombre << "' no existe en el grafo. Intentelo de nuevo.\n\n";
		}
	}
	imprimir_nodo(nodo);
	cout << endl << endl;
}


// main() \\

int main() {
	Nodo* grafo = nullptr; // Puntero al grafo principal que se usará en el laboratorio.

	nivel_1(grafo);
	system("pause");
	system("cls");

	nivel_2(grafo);
	system("pause");
	system("cls");

	nivel_3(grafo);
	system("pause");

	liberar_grafo(grafo); // Se libera la memoria usada por el grafo.

	return 0;
}