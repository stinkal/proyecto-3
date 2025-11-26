// Instituto Tecnológico de Costa Rica
// Carrera de Ingeniería Electrónica
// Curso de Análisis y Diseño de Algoritmos
// Grupo 21
// Proyecto 3
// Estudiantes programadores: Ethan Kennan y José Montoya
// Profesor: Bryan Hernández Sibaja
// Alajuela, Costa Rica
// II Semestre, 2025


// | --------------------------------------- Indicaciones del proyecto --------------------------------------- | //

// Proyecto #3: Simulador de Red Social Simple
// Para este proyecto, se desarrollará un software simulador de red social, utilizando el lenguaje C++,
// que permita al usuario gestionar conexiones / amigos dentro de dicha red.
// 
// El nombre de dicha red social queda a criterio del equipo.
// 
// El Objetivo es modelar una pequeña red social donde los usuarios son representados como nodos
// y las amistades como aristas, utilizando estructuras de grafos no dirigidos.El programa debe
// permitir crear usuarios, establecer relaciones de amistad, buscar conexiones y generar reportes
// ordenados.
// 
// Requisitos:
//	• Los equipos de trabajo deben de ser de 1 o 2 personas, sin excepciones.
//	• Utilizar el lenguaje de programación C++ .
//	• Todo el programa se realizará mediante línea de comandos.
//	• Se debe trabajar con estructuras de datos(grafos), TDA, flujos de control, algoritmos de
//	  búsqueda y archivos.
//	• El grafo debe implementarse como no dirigido, de manera que la amistad sea recíproca.
//	• Se recomienda utilizar listas de adyacencia(por ejemplo, usando map, set o listas
//	  enlazadas).
//	• Cada usuario(nodo) deberá contener al menos :
//		o ID único(indispensable para validar que no hayan usuarios repetidos)
//		o Nombre de usuario(string)
//		o Lista de amigos(estructura asociativa o lista enlazada)
//	• Se debe crear el menú con todas las funcionalidades.
// Funcionalidades
//	1. Gestión de Usuarios
//		o Agregar, Consultar, Modificar y Eliminar nuevos usuarios a la red social.
//		o Mostrar todos los usuarios registrados.
//	2. Gestión de Amistades
//		o Establecer amistad entre dos usuarios existentes.El usuario ingresa unos cuántos
//		  caracteres como criterio de búsqueda inicial y el programa arroja una lista con todos
//		  los usuarios cuyos nombres inicien con los mismos caracteres.
//		o Validar que la amistad no se duplique.Enviar un mensaje de error.
//		o Mostrar todas las amistades de un usuario específico.
//	3. Búsqueda de Conexiones
//		o Implementar un algoritmo de Búsqueda en Amplitud(BFS) para determinar la ruta más
//		  corta de conexión entre dos usuarios(grados de separación).
//		o Mostrar el camino completo entre ambos usuarios si existe(por ejemplo : Ana → Luis
//		  → Marta → Pedro).
//	4. Reportes
//		o Generar un listado de todos los usuarios ordenados :
//			▪ Alfabéticamente por nombre, o
//			▪ Por cantidad de amigos(de mayor a menor).
//		o Permitir la exportación de este reporte a un archivo.txt.
//	5. Persistencia de Datos
//		o Guardar todos los usuarios y amistades en archivos de texto al salir del programa.
//		o Cargar los datos nuevamente al iniciar el sistema.


// | ---------------------------------------- Desarrollo del proyecto ---------------------------------------- | //


// | ---------- Sección de las estructuras de datos ---------- | //

#include <iostream>		 // Bibloteca para flujo de datos de entrada y salida.
#include <string>		 // Bibloteca para hacer uso del getline().
#include <sstream>		 // Bibloteca para usar stringstream en toString().
#include <set>			 // Bibloteca para usar conjuntos ordenados.
#include <unordered_map> // Bibloteca para usar mapas no ordenados.
#include <vector>		 // Bibloteca para usar vectores.
#include <limits>        // Biblioteca para usar limites numericos
#include <algorithm>     // Biblioteca para ayudar manipular datos.
#include <queue>         // Biblioteca para FIFO, particularmente util para BFS.

using namespace std;

// Nodo del grafo.
struct Usuario {
	int iD;
	string nombre;
	set<int> amigos; // Conjunto de IDs de amigos para evitar duplicados.

	// Constructor por defecto para evitar problemas al usar [] con unordered_map.
	Usuario() : iD(0), nombre("") {}

	// Constructor con parámetros para inicializar el usuario.
	Usuario(int id, const string& nom) : iD(id), nombre(nom) {}
};

using GrafoUsuarios = unordered_map<int, Usuario>; // Mapa no ordenado para almacenar los usuarios por su ID.

GrafoUsuarios grafoUsuarios; 

string toStringUsuario(const Usuario& usuario) { // Función para convertir los datos del usuario en una cadena de texto formateada.
	stringstream s; // Variable del tipo stringstream usada para construir la cadena de texto.

	s << "| ---------- Informacion del usuario ---------- | " << endl
	  << "  ID: "				  << usuario.iD				  << endl
	  << "  Nombre: "			  << usuario.nombre			  << endl
	  << "  Cantidad de amigos: " << usuario.amigos.size()	  << endl << endl
	  << "               | --- Amigos --- |"				  << endl; 
	
	// Se listan los nombres de los amigos.
	if (usuario.amigos.empty()) { // Si el conjunto de amigos está vacío.
		s << "         (No tiene amigos registrados)" << endl;
	}
	else {
		for (int idAmigo : usuario.amigos) { // Se recorre cada ID de los amigos y se busca dentro del grafo.
			auto amigo = grafoUsuarios.find(idAmigo); // auto deduce el tipo automáticamente y lo guarda en la variable.

			if (amigo != grafoUsuarios.end()) { // Si lo encuentra y no es end(), lo muestra. end() indica que llegó al final del mapa.
				s << "    - [" << amigo->second.iD << "] " << amigo->second.nombre << endl;
			}
		}
	}
	return s.str(); // Se retorna la cadena que contiene toda la información del usuario.
}

// | ---------- Sección de funcionalidad: 0. Helper Functions  ---------- | //

string toLowerStr(const string& s) { // funcion de case-insensitivity
    string out = s;
    for (size_t i = 0; i < out.size(); ++i) {
        out[i] = static_cast<char>(tolower(static_cast<unsigned char>(out[i])));
    }
    return out;
}

vector<int> buscarCoincidenciasNombre(const GrafoUsuarios& grafo, const string& criterio) { // funcion para buscar nombres por criterio de busqueda parcial
    vector<int> resultados;

    if (criterio.empty()) return resultados;

    string criterioLower = toLowerStr(criterio); // case-insensitive

    for (const auto& par : grafo) {
        const Usuario& usuario = par.second;
        string nombreLower = toLowerStr(usuario.nombre);

        // buscar substring
        if (nombreLower.find(criterioLower) != string::npos) {
            resultados.push_back(usuario.iD);
        }
    }

    return resultados;
}
int seleccionarUsuarioDeCoincidencias(const GrafoUsuarios& grafo, const vector<int>& matches) {
	// si la busqueda no da nada
	if (matches.empty()) {
        cout << "No se encontraron usuarios que coincidan con la busqueda.\n\n";  // <---
        return -1;
    }

    // si solo hay una coincidencia, pedir confirmacion
    if (matches.size() == 1) {
        int id = matches[0];
        auto it = grafo.find(id);
        if (it == grafo.end()) return -1; // proteccion, aunque no deberia ocurrir

        cout << "Se encontro 1 usuario: \n\n";
        cout << "  ID: [" << it->second.iD << "]  Nombre: " << it->second.nombre << "\n\n";
        cout << "Es este el usuario que busca? Ingrese 's' para confirmar, de lo contrario ingrese cualquier otro caracter: ";  // <---

		string decision;

		// si hay basura pendiente en el buffer, limpiarla SOLO si el siguiente char es '\n'
		if (cin.peek() == '\n') {
		    cin.ignore();
		}
		getline(cin, decision);
		cout << endl;

        if (!decision.empty() && (decision == "s" || decision == "S")) {
            return id;
        } else {
            cout << "Busqueda cancelada.\n\n";
            return -1;
        }
    }

    // si hay varias coincidencias, listarlas y pedir seleccion
    cout << "Se encontraron " << matches.size() << " usuarios que coinciden:\n\n";
    for (size_t i = 0; i < matches.size(); ++i) {
        int id = matches[i];
        auto it = grafo.find(id);
        if (it != grafo.end()) {
            cout << "  [ " << (i + 1) << " ] ID: " << it->second.iD << " - " << it->second.nombre << '\n';
        }
    }
    cout << "\nIngrese el numero del usuario que desea seleccionar (1-" << matches.size() << "), o 0 para cancelar: "; 

    int opcion = -1;
    while (true) {
        if (!(cin >> opcion)) {
            cout << "Entrada invalida. Ingrese un numero entre 1 y " << matches.size() << " o 0 para cancelar: ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        if (opcion == 0) {
            cout << "Seleccion cancelada.\n\n";
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return -1;
        }

        if (opcion < 1 || opcion > (int)matches.size()) {
            cout << "Opcion invalida. Ingrese un numero entre 1 y " << matches.size() << " o 0 para cancelar: ";
            continue;
        }

        // opcion valida
        int seleccionado = matches[opcion - 1];
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << endl;
        return seleccionado;
    }
}
int buscarYSeleccionarUsuario(GrafoUsuarios& grafo, const string& prompt) { // funcion multi-uso para busqueda de usuario por nombre parcial
    string criterio;
    cout << prompt;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    getline(cin, criterio);
    cout << endl;

    vector<int> matches = buscarCoincidenciasNombre(grafo, criterio);

    return seleccionarUsuarioDeCoincidencias(grafo, matches);
}

// | ---------- Sección de funcionalidad: 1. Gestión de Usuarios  ---------- | //

void agregarUsuario(GrafoUsuarios& grafo) { 
	int id = 0;
	string nombre = "";

	cout << "Ingrese el ID unico del nuevo usuario: ";
	if (!(cin >> id)){ // valida si el cin se ejecuta correctamente
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		cout << "\nEntrada invalida. Por favor ingresar un numero entero.\n\n";
		return;
	}
	else if (grafo.find(id) != grafo.end()) { // Si lo encuentra y no es end(), significa que ya existe. end() indica que llegó al final del mapa.
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		cout << "Error: Ya existe un usuario con ID [" << id << "]. Intentelo de nuevo." << endl;
		return;
	}
	else {
		cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Se limpia el buffer antes de usar getline().

		cout << "Ingrese el nombre del nuevo usuario: "; getline(cin, nombre); cout << endl;

		grafo.emplace(id, Usuario(id, nombre)); // emplace() construye el objeto Usuario directamente en el mapa, aprovechándose del constructor.

		cout << "El usuario con ID [" << id << "] y nombre '" << nombre << "' fue agregado exitosamente.\n\n";
	}
}
// Agregar usuario: devuelve true si se pudo, false si el ID ya existe.
bool agregarUsuarioPRUEBA(GrafoUsuarios& grafo, int id, const string& nombre) {																						// ESTE ES SOLO DE PRUEBA
	if (grafo.find(id) != grafo.end()) {
		return false; // ID repetido
	}
	grafo.emplace(id, Usuario{ id, nombre });
	return true;
}
void consultarUsuarioID(GrafoUsuarios& grafo) { 
	int id = 0;

	cout << "Ingrese el ID unico del usuario a buscar: "; 
	if (!(cin >> id)){ // valida si el input es un entero
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		cout << "\nEntrada invalida. Por favor ingresar un numero entero.\n\n";
		return;
	}

	if (grafo.find(id) != grafo.end()) {
		cout << "El usuario con el ID [" << id << "] fue encontrado exitosamente y es el siguiente:\n\n"
			 << toStringUsuario(grafo[id]) << endl; // Se muestra la información del usuario encontrado.
	}
	else {
		cout << "Error: No existe un usuario con ID [" << id << "]. Intentelo de nuevo.\n\n";
	}
}
void consultarUsuarioNombre(GrafoUsuarios& grafo) {

	// funcion multi-uso de busqueda por nombre
    int idSeleccionado = buscarYSeleccionarUsuario(
        grafo,
        "Ingrese el nombre o un fragmento del nombre del usuario a buscar: "               // <---
    );

    if (idSeleccionado == -1) {
        // No hay coincidencias o el usuario canceló la selección.
        return;
    }

    // Mostrar la información completa del usuario encontrado.
    cout << "Usuario encontrado:\n\n"
         << toStringUsuario(grafo[idSeleccionado]) << endl;
}
void modificarUsuarioID(GrafoUsuarios& grafo) {
	int id = 0, idNuevo = 0;
	string nombre = "";

	cout << "Ingrese el ID unico del usuario por modificar: ";
	if (!(cin >> id)){ // valida si el cin se ejecuta correctamente
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		cout << "\nEntrada invalida. Por favor ingresar un numero entero.\n\n";
		return;
	}

	if (grafo.find(id) != grafo.end()) {
		cout << "El usuario con el ID [" << id << "] fue encontrado exitosamente y es el siguiente:\n\n"
			 << toStringUsuario(grafo[id]) << endl; // Se muestra la información del usuario encontrado.

		while (true) {
			cout << "Ingrese el nuevo ID para el usuario: ";
			if (!(cin >> idNuevo)){ // valida si el cin se ejecuta correctamente
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cout << "\nEntrada invalida. Por favor ingresar un numero entero.\n\n";
			}

			else if (idNuevo != id && grafo.find(idNuevo) != grafo.end()) {
				cout << "Error: Ya existe un usuario con ID [" << idNuevo << "]. Intentelo de nuevo." << endl;
				cin.clear();
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
			}
			else {
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
				cout << "Ingrese el nuevo nombre para el usuario: "; cin >> nombre; cout << endl;

				Usuario usuario = grafo[id]; // Se copia el usuario actual.
				grafo.erase(id);			 // Se elimina el usuario con el ID antiguo.
				usuario.iD = idNuevo;			 // Se actualiza el ID.
				usuario.nombre = nombre;	 // Se actualiza el nombre.
				grafo.emplace(idNuevo, usuario);	 // Se agrega el usuario con el nuevo ID y nombre.

				cout << "El usuario ha sido modificado exitosamente.\n\n";

				return;
			}
		}
	}
	else {
		cout << "Error: No existe un usuario con ID [" << id << "]. Intentelo de nuevo.\n\n";
	}
}
void modificarUsuarioNombre(GrafoUsuarios& grafo) {
	int idNuevo = 0;
	string nombre = "", nombreNuevo = "";
	int idSeleccionado = buscarYSeleccionarUsuario(
		grafo,
		"Ingrese el nombre o un fragmento del nombre del usuario por modificar: " 			 // <---
	);

	if (idSeleccionado == -1) {
		// no se encontró usuario, o el usuario cancelo la selección.
		return;
	}

	cout << "El usuario seleccionado es el siguiente:\n\n"
		 << toStringUsuario(grafo[idSeleccionado]) << endl;
	while (true) {
		cout << "\nIngrese el nuevo id para el usuario: ";
		if (!(cin >> idNuevo)){ // valida si el cin es un entero
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cout << "\nEntrada invalida. Por favor ingresar un numero entero.\n\n";
		}

		else if (idNuevo != idSeleccionado && grafo.find(idNuevo) != grafo.end()) {
			cout << "Error: Ya existe un usuario con ID [" << idNuevo << "]. Intentelo de nuevo." << endl;
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
		}
		else {
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cout << "Ingrese el nuevo nombre para el usuario: "; getline(cin, nombreNuevo); cout << endl;

			Usuario usuario = grafo[idSeleccionado];
			usuario.iD = idNuevo;
			usuario.nombre = nombreNuevo;

			if (idNuevo != idSeleccionado) {
				for (auto& par : grafo) {
					if (par.second.amigos.erase(idSeleccionado) > 0) {
						par.second.amigos.insert(idNuevo);
					}
				}
				grafo.erase(idSeleccionado);
				grafo.emplace(idNuevo, usuario);
			} else {
				grafo[idSeleccionado].nombre = nombreNuevo;
			}

			cout << "El usuario ha sido modificado exitosamente.\n\n";
			return;
		}
	}
}
void eliminarUsuarioID(GrafoUsuarios& grafo) {
	int id = 0;
	string nombre = "", decision = "";

	cout << "Ingrese el ID unico del usuario por eliminar: ";
	if (!(cin >> id)){ // valida si el cin es un entero
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cout << "\nEntrada invalida. Por favor ingresar un numero entero.\n\n";
		}

	else if (grafo.find(id) != grafo.end()) {
		cout << "El usuario con el ID [" << id << "] fue encontrado exitosamente y es el siguiente:\n\n"
			<< toStringUsuario(grafo[id]) << endl; // Se muestra la información del usuario encontrado.

		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		cout << "Desea eliminarlo? Ingrese 's' para confirmar, de lo contrario ingrese cualquier otro caracter: "; getline(cin, decision); cout << endl;

		if (decision == "s" || decision == "S") {
			// Se elimina este ID de la lista de amigos de todos los demás.
			for (auto& par : grafo) {
				par.second.amigos.erase(id);
			}
			grafo.erase(grafo.find(id)); // Se elimina el usuario con el ID antiguo.

			cout << "El usuario ha sido eliminado exitosamente.\n\n";
		}
		else {
			cout << "Se ha cancelado la eliminacion. No se elimino ningun usuario.\n\n";
		}
	}
	else {
		cout << "Error: No existe un usuario con ID [" << id << "]. Intentelo de nuevo.\n\n";
	}
}
void eliminarUsuarioNombre(GrafoUsuarios& grafo) {
	string nombre = "", decision = "";
	vector<int> coincidentes; // Vector para almacenar los IDs de los usuarios que coinciden.
	int opcion = 0;

	cin.ignore(numeric_limits<streamsize>::max(), '\n');

	cout << "Ingrese el nombre del usuario por eliminar: "; getline(cin, nombre); cout << endl;

	// Se hace una búsqueda lineal por nombre.
	for (const auto& par : grafo) { // par.first hace referencia al ID, y par.second al nombre del usuario.
		const Usuario& usuario = par.second;

		if (usuario.nombre == nombre) {
			coincidentes.push_back(usuario.iD); // push_back() agrega el ID al final del vector.
		}
	}
	if (coincidentes.empty()) { // Si el vector queda vacío, no se encontraron coincidencias.
		cout << "No se encontro ningun usuario con el nombre '" << nombre << "'.\n\n";

		return; // Se sale de la función.
	}
	// Si solo hay uno que coincide, se elimina. 
	if (coincidentes.size() == 1) {
		int idBorrar = coincidentes[0];

		cout << "El usuario con ese nombre fue encontrado exitosamente y es el siguiente:\n\n"
			 << toStringUsuario(grafo[coincidentes[0]]) << endl;

		cout << "\nDesea eliminarlo? Ingrese 's' para confirmar, de lo contrario ingrese cualquier otro caracter: "; getline(cin, decision); cout << endl;

		if (decision == "s" || decision == "S") {
			// Se elimina este ID de la lista de amigos de todos los demás.
			for (auto& par : grafo) {
				par.second.amigos.erase(idBorrar);
			}
			grafo.erase(idBorrar);

			cout << "El usuario ha sido eliminado exitosamente.\n\n";
		}
		else {
			cout << "Se ha cancelado la eliminacion. No se elimino ningun usuario.\n\n";
		}
		return;
	}
	// Si hay varios que coinciden, se listan todos y se deja elegir al usuario el que desea editar. 
	cout << "Se encontraron " << coincidentes.size() << " usuarios con el nombre '" << nombre << "':\n\n";

	for (int i = 0; i < coincidentes.size(); ++i) {
		auto it = grafo.find(coincidentes[i]);

		if (it != grafo.end()) {
			const Usuario& usuario = it->second;
			cout << "  [ " << (i + 1) << " ] ID: " << usuario.iD << " - " << usuario.nombre << '\n';
		}
	}
	cout << "\nIngrese el numero del usuario que desea eliminar (1-" << coincidentes.size() << "): "; // Del 1 a n.

	while (true) {
		cin >> opcion;
		if (opcion < 1 || opcion >(int)coincidentes.size()) { // Validación de entradas.
			cout << "\nOpcion invalida. Ingrese un numero entre 1 y "
				 << coincidentes.size() << ": ";
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
		}
		else {
			break;
		}
	}
	int idBorrar = coincidentes[opcion - 1];

	cin.clear();
	cin.ignore(numeric_limits<streamsize>::max(), '\n');
	cout << "\nDesea eliminarlo? Ingrese 's' para confirmar, de lo contrario ingrese cualquier otro caracter: "; getline(cin, decision); cout << endl;

	if (decision == "s" || decision == "S") {
		// Se elimina este ID de la lista de amigos de todos los demás.
		for (auto& par : grafo) {
			par.second.amigos.erase(idBorrar);
		}
		grafo.erase(idBorrar); // Se elimina el usuario con el ID antiguo.

		cout << "El usuario ha sido eliminado exitosamente.\n\n";
	}
	else {
		cout << "Se ha cancelado la eliminacion. No se elimino ningun usuario.\n\n";
	}
}
void mostrarTodosUsuarios(const GrafoUsuarios& grafo) {
	if (grafo.empty()) {
		cout << "No hay usuarios registrados en la red social.\n\n";
		return;
	}
	cout << "| ----------------------------- Lista de todos los usuarios registrados -------------------------------- | \n\n";

	for (const auto& par : grafo) {
		const Usuario& usuario = par.second;
		cout << toStringUsuario(usuario) << endl;
	}
}

// | ---------- Sección de funcionalidad: 2. Gestión de Amistades  ---------- | //

void agregarAmigo(GrafoUsuarios& grafo) {
	int id1 = 0, id2 = 0;  //  declarar iD de cada usuario en la amistad

	    cout << "Como desea identificar al primer usuario?\n"            // <---
         << "  [ 1 ] Buscar por ID\n"
         << "  [ 2 ] Buscar por nombre\n"
         << "Ingrese la opcion (1-2): ";
    int modo1 = 0;
    cin >> modo1;
    cout << endl;

    if (modo1 == 1) {
        cout << "Ingrese el primer ID: ";
        if (!(cin >> id1)){ // valida si el cin es un entero
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cout << "\nEntrada invalida. Por favor ingresar un numero entero.\n\n";
			return;
		}
        cout << endl;
    } else if (modo1 == 2) {
        int res = buscarYSeleccionarUsuario(grafo, "Ingrese el nombre o un fragmento del nombre del primer usuario: "); // <---
        if (res == -1) return; // cancelado o no encontrado
        id1 = res;             // resultado de buscarYSeleccionarUsuario se guarda
    } else {
        cout << "Opcion invalida. Operacion cancelada.\n\n";
        return;
    }

    cout << "Como desea identificar al segundo usuario?\n"            // <---
         << "  [ 1 ] Buscar por ID\n"
         << "  [ 2 ] Buscar por nombre\n"
         << "Ingrese la opcion (1-2): ";
    int modo2 = 0;
    cin >> modo2;
    cout << endl;

    if (modo2 == 1) {
        cout << "Ingrese el segundo ID: ";
        if (!(cin >> id2)){ // valida si el cin es un entero
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cout << "\nEntrada invalida. Por favor ingresar un numero entero.\n\n";
			return;
		}
        cout << endl;
    } else if (modo2 == 2) {
        int res = buscarYSeleccionarUsuario(grafo, "Ingrese el nombre o un fragmento del nombre del segundo usuario: "); // <---
        if (res == -1) return; // cancelado o no encontrado
        id2 = res;
    } else {
        cout << "Opcion invalida. Operacion cancelada.\n\n";
        return;
    }

	// Verificar que no sean el mismo ID
	if (id1 == id2) {
		cout << "Error: Un usuario no puede agregarse a si mismo.\n\n";
		return;
	}

	// Verificar existencia de ambos usuarios
	if (grafo.find(id1) == grafo.end()) {
		cout << "Error: No existe un usuario con ID [" << id1 << "].\n\n";
		return;
	}
	if (grafo.find(id2) == grafo.end()) {
		cout << "Error: No existe un usuario con ID [" << id2 << "].\n\n";
		return;
	}

	// Verificar si ya son amigos
	if (grafo[id1].amigos.count(id2) > 0) {
		cout << "Los usuarios ya son amigos.\n\n";
		return;
	}

	// agregar amistad reciprocalmente
	grafo[id1].amigos.insert(id2);
	grafo[id2].amigos.insert(id1);

	cout << "Amistad agregada exitosamente entre "
		 << grafo[id1].nombre << " y " << grafo[id2].nombre << ".\n\n";  //  agarra los nombres associados con cada iD
}

void eliminarAmigo(GrafoUsuarios& grafo) {
	int id1 = 0, id2 = 0;   //  iD de los dos usuarios en la amistad

    cout << "Como desea identificar al primer usuario?\n"            // <---
         << "  [ 1 ] Buscar por ID\n"
         << "  [ 2 ] Buscar por nombre\n"
         << "Ingrese la opcion (1-2): ";
    int modo1 = 0;
    cin >> modo1;
    cout << endl;

    if (modo1 == 1) {
        cout << "Ingrese el primer ID: ";
        if (!(cin >> id1)){ // valida si el cin es un entero
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cout << "\nEntrada invalida. Por favor ingresar un numero entero.\n\n";
			return;
		}
        cout << endl;
    } else if (modo1 == 2) {
        int res = buscarYSeleccionarUsuario(grafo, "Ingrese el nombre o un fragmento del nombre del primer usuario: ");   // <---
        if (res == -1) return;
        id1 = res;
    } else {
        cout << "Opcion invalida. Operacion cancelada.\n\n";
        return;
    }

    cout << "Como desea identificar al segundo usuario?\n"            // <---
         << "  [ 1 ] Buscar por ID\n"
         << "  [ 2 ] Buscar por nombre\n"
         << "Ingrese la opcion (1-2): ";
    int modo2 = 0;
    cin >> modo2;
    cout << endl;

    if (modo2 == 1) {
        cout << "Ingrese el segundo ID: ";
        if (!(cin >> id2)){ // valida si el cin es un entero
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cout << "\nEntrada invalida. Por favor ingresar un numero entero.\n\n";
			return;
		}
        cout << endl;
    } else if (modo2 == 2) {
        int res = buscarYSeleccionarUsuario(grafo, "Ingrese el nombre o un fragmento del nombre del segundo usuario: ");
        if (res == -1) return;
        id2 = res;
    } else {
        cout << "Opcion invalida. Operacion cancelada.\n\n";
        return;
    }

	// Verificar que no sean el mismo iD
	if (id1 == id2) {
		cout << "Error: No puede existir una amistad con uno mismo.\n\n";
		return;
	}

	// Verificar existencia de ambos usuarios
	if (grafo.find(id1) == grafo.end()) {
		cout << "Error: No existe un usuario con ID [" << id1 << "].\n\n";
		return;
	}
	if (grafo.find(id2) == grafo.end()) {
		cout << "Error: No existe un usuario con ID [" << id2 << "].\n\n";
		return;
	}

	// Verificar si NO son amigos
	if (grafo[id1].amigos.count(id2) == 0) {
		cout << "Los usuarios no son amigos.\n\n";
		return;
	}

	// Eliminar amistad mutua
	grafo[id1].amigos.erase(id2);
	grafo[id2].amigos.erase(id1);

	cout << "Amistad eliminada exitosamente entre "
		 << grafo[id1].nombre << " y " << grafo[id2].nombre << ".\n\n";
}

void consultarAmigos(const GrafoUsuarios& grafo) {  // consulta amistades de un solo usuario indicado
	int id = 0;  //  id del solo usuario
		cout << "Como desea identificar al usuario?\n" 		  // <---
         << "  [ 1 ] Buscar por ID\n"
         << "  [ 2 ] Buscar por nombre o fragmento de nombre\n"            // <---
         << "Ingrese la opcion (1-2): ";
    int modo = 0;
    cin >> modo;
    cout << endl;

    if (modo == 1) {
        cout << "Ingrese el ID del usuario: ";
        if (!(cin >> id)){ // valida si el cin es un entero
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cout << "\nEntrada invalida. Por favor ingresar un numero entero.\n\n";
			return;
		}
        cout << endl;
    } else if (modo == 2) {
        // usar la funcion de busqueda interactiva por nombre
        int resultado = buscarYSeleccionarUsuario(const_cast<GrafoUsuarios&>(grafo), "Ingrese el nombre o fragmento del usuario a buscar: ");
        if (resultado == -1) {
            // no se encontro o el usuario cancelo
            return;
        }
        id = resultado;
    } else {
        cout << "Opcion invalida. Volviendo al menu.\n\n";
        return;
    }
	
	auto it = grafo.find(id);  //  iterator
    
	if (it == grafo.end()) {
        cout << "Error: No existe un usuario con ID [" << id << "].\n\n";
        return;
    }

	const Usuario& usuario = it->second;  //  el nombre del usuario es el segundo miembro

	cout << "Amigos de " << usuario.nombre << " (ID " << usuario.iD << "):\n\n";

	if (usuario.amigos.empty()) {
		cout << "Este usuario no tiene amigos registrados.\n\n";
		return;
	}

	for (int idAmigo : usuario.amigos) {
		auto itA = grafo.find(idAmigo);

		if (itA != grafo.end()) {
			cout << " - ID: " << itA->second.iD
				 << " | Nombre: " << itA->second.nombre << '\n';
		}
	}

	cout << endl;
}

// | ---------- Sección de funcionalidad: 3. Búsqueda de Conexiones ---------- | //

vector<int> ruta; 
enum EstadoBFS { // manejo de salidas de BFS
    OK, // salida valida
	TRIVIAL, // el iD de destino es igual al iD de origen
    ID_INVALIDO, // alguno de los iD's ingresados no existe
    SIN_CONEXION // no existe una ruta entre los iD's ingresados
};

// ejecuta una busqueda BFS entre dos usuarios 
EstadoBFS bfsRutaMasCorta(const GrafoUsuarios& grafo, int origen, int destino) {
    ruta.clear();

    // validaciones basicas

    if (grafo.find(origen) == grafo.end() ||
        grafo.find(destino) == grafo.end()) {
        return ID_INVALIDO;
    }

    // si el origen y destino son iguales, la ruta es trivial
    if (origen == destino) {
        ruta.push_back(origen);
        return TRIVIAL;
    }

    queue<int> cola; // estructura FIFO para identificar orden de busqueda
    set<int> visitados; // guarda los nodos ya visitados
    unordered_map<int, int> padre;  // guarda el nodo padre para reconstruir el camino

    cola.push(origen); // agregar el nodo de origen a la cola
    visitados.insert(origen);  // marcar de una vez el nodo de origen como visitado

    bool encontrado = false; // solo cambia a "true" cuando el BFS esta completado

    while (!cola.empty()) {
        int actual = cola.front(); // nodo actual de busqueda empieza con el primero en la cola
        cola.pop(); // ^ se elimina de la cola

        // si llegamos al destino, termina BFS
        if (actual == destino) {
            encontrado = true;
            break;
        }

        // recorrer todos los amigos (adyacentes)
        for (int vecino : grafo.at(actual).amigos) { // amigos del nodo actual
            if (visitados.find(vecino) == visitados.end()) {
                visitados.insert(vecino); // marcar nodo 
                padre[vecino] = actual; // marca el nodo padre
                cola.push(vecino); // mover nodo al final de la cola
            }
        }
    }

    // si no se existe una conexion entre los usuarios
    if (!encontrado) {
        return SIN_CONEXION;
    }

    // reconstruir ruta desde destino hasta origen usando el mapa padre
    int nodo = destino;
    while (nodo != origen) {
        ruta.push_back(nodo);
        nodo = padre[nodo];
    }
    ruta.push_back(origen);

    reverse(ruta.begin(), ruta.end()); // invertir la ruta

    return OK;
}

// | ---------- Sección de la interfaz ---------- | //

// Prototipos de las funciones de la interfaz. 
void bienvenida();
void head();
void headGesUsuarios();
void headGesAmistades();
void headBusConexiones();
void headReportes();			// Estos permiten organizar mejor el código y evitar errores de compilación.
int  foot();

void menuPrincipal();
int  principal();
void switchPrincipal();

void menuGesUsuarios();
int  usuarios();
void switchGesUsuarios();

void menuConsultarUsuarios();
int  consultarUsuarios();
void switchConsultarUsuarios();

void menuModificarUsuarios();
int  modificarUsuarios();
void switchModificarUsuarios();

void menuEliminarUsuarios();
int  eliminarUsuarios();
void switchEliminarUsuarios();

void menuGesAmistades();
int  gesAmistades();
void switchGesAmistades();

void menuBusConexiones();
int busConexiones();
void switchBusConexiones();

void menuReportes();
int  reportes();
void switchReportes();

// Implementación de las funciones de la interfaz.
void bienvenida() {
	cout << "| ---------------------- Bienvenidos al simulador de una red social - Kentoya !!! ---------------------- |" << endl << endl;
	
	// grafo de usuarios de prueba:
	agregarUsuarioPRUEBA(grafoUsuarios, 1, "Walter");
	agregarUsuarioPRUEBA(grafoUsuarios, 2, "Jesse");
	agregarUsuarioPRUEBA(grafoUsuarios, 3, "Gus");
	agregarUsuarioPRUEBA(grafoUsuarios, 4, "Saul");
	agregarUsuarioPRUEBA(grafoUsuarios, 5, "Walter");

	system("pause");
	switchPrincipal();
}
void head() {
	system("cls");
	cout << "| -------------------------------------- - Red Social Kentoya - ---------------------------------------- |" << endl << endl;
}
void headGesUsuarios() {
	head();
	cout << "| ------------------------------------ [ 1 ] Gestion de usuarios --------------------------------------- |" << endl << endl;
}
void headConsultarUsuarios() {
	headGesUsuarios();
	cout << "| ------------------------------------ [ 2 ] Consultar un usuario -------------------------------------- |" << endl << endl;
}
void headModificarUsuarios() {
	headGesUsuarios();
	cout << "| ------------------------------------ [ 3 ] Modificar un usuario -------------------------------------- |" << endl << endl;
}
void headEliminarUsuarios() {
	headGesUsuarios();
	cout << "| ------------------------------------ [ 4 ] Eliminar un usuario --------------------------------------- |" << endl << endl;
}
void headGesAmistades() {
	head();
	cout << "| ------------------------------------ [ 2 ] Gestion de amistades -------------------------------------- |" << endl << endl;
}
void headBusConexiones() {
	head();
	cout << "| ----------------------------------- [ 3 ] Busqueda de conexiones ------------------------------------- |" << endl << endl;
}
void headReportes() {
	head();
	cout << "| ------------------------------------------ [ 4 ] Reportes -------------------------------------------- |" << endl << endl;
}
int foot() {
	int opcion;

	cout << "Digite el numero de opcion a la cual desea ingresar: ";
	if (!(cin >> opcion)) { // llama cin; si funciona correctamente, salta el if
		cin.clear(); //limpia la funcion
		cin.ignore(numeric_limits<streamsize>::max(), '\n'); //limpiar input buffer
		return -1; // llama default caso (error)
	}
	cout << endl;

	return opcion;
}

void menuPrincipal() {
	cout << "  [ 1 ] Gestion de usuarios."		<< endl
		 << "  [ 2 ] Gestion de amistades."		<< endl
		 << "  [ 3 ] Busqueda de conexiones."	<< endl
		 << "  [ 4 ] Reportes."					<< endl
		 << "  [ 0 ] Salir."			<< endl << endl
		 << "| ------------------------------------------------------------------------------------------------------ |" << endl << endl;
}
int principal() {
	head();
	menuPrincipal();

	return foot();
}
void switchPrincipal() {
	int opcion;

	do {
		opcion = principal();

		switch (opcion) {
		case 1: headGesUsuarios();		// Funcionalidad 1: CRUD de Gestión de Usuarios.
			switchGesUsuarios();

			system("pause");
			break;
		case 2: headGesAmistades();		// Funcionalidad 2: Gestión de Amistades.
			switchGesAmistades();
			
			system("pause");
			break;
		case 3: headBusConexiones();	// Funcionalidad 3: Búsqueda de Conexiones.
			switchBusConexiones();

			system("pause");
			break;
		case 4: headReportes();			// Funcionalidad 4: Reportes. 
			switchReportes();

			system("pause");
			break;
		case 0: cout << "Programa finalizado!" << endl << endl; // Funcionalidad 5: Persistencia de Datos.
			
			system("pause");
			exit(0);
		default: // Es una opción que se ejecuta si la opcion ingresada no coincide con ninguna de las anteriores.
			cout << "Opcion invalida, por favor intentelo de nuevo." << endl << endl;
			system("pause");
		}
	} while (opcion != 0);
}
void menuGesUsuarios() {
	cout << "  [ 1 ] Agregar un nuevo usuario."		<< endl
		 << "  [ 2 ] Consultar un usuario."			<< endl
		 << "  [ 3 ] Modificar un usuario."			<< endl
		 << "  [ 4 ] Eliminar un usuario."			<< endl
		 << "  [ 5 ] Mostrar todos los usuarios."	<< endl
		 << "  [ 6 ] Volver atras."			<< endl << endl
		 << "| ------------------------------------------------------------------------------------------------------ |" << endl << endl;
}
int usuarios() {
	head();
	headGesUsuarios();
	menuGesUsuarios();

	return foot();
}
void switchGesUsuarios() {
	int opcion;
	do {
		opcion = usuarios();
		switch (opcion) {
		case 1: headGesUsuarios();
			cout << "| ---------------------------------- [ 1 ] Agregar un nuevo usuario ------------------------------------ |" << endl << endl;
			agregarUsuario(grafoUsuarios);

			system("pause");
			break;
		case 2: headGesUsuarios(); // interesting note: this call to headGesUsuarios was previously missing, but it appears to behave identically with or without it
			cout << "| ------------------------------------ [ 2 ] Consultar un usuario -------------------------------------- |" << endl << endl;
			switchConsultarUsuarios();

			system("pause");
			break;
		case 3: headGesUsuarios();
			cout << "| ------------------------------------ [ 3 ] Modificar un usuario -------------------------------------- |" << endl << endl;
			switchModificarUsuarios();

			system("pause");
			break;
		case 4: headGesUsuarios();
			cout << "| ------------------------------------- [ 4 ] Eliminar un usuario -------------------------------------- |" << endl << endl;
			switchEliminarUsuarios();

			system("pause");
			break;
		case 5: headGesUsuarios();
			cout << "| --------------------------------- [ 5 ] Mostrar todos los usuarios ----------------------------------- |" << endl << endl;
			mostrarTodosUsuarios(grafoUsuarios);

			system("pause");
			break;
		case 6: // El usuario decide volver al menú principal.
			break;
		default:
			cout << "Opcion invalida, por favor intentelo de nuevo." << endl << endl;
			system("pause");
		}
	} while (opcion != 6);
}
void menuConsultarUsuarios() {
	cout << "  [ 1 ] Consultar por ID unico."	<< endl
		 << "  [ 2 ] Consultar por nombre."		<< endl
		 << "  [ 3 ] Volver atras."		<< endl << endl
		 << "| ------------------------------------------------------------------------------------------------------ |" << endl << endl;
}
int consultarUsuarios() {
	head();
	headGesUsuarios();
	headConsultarUsuarios();
	menuConsultarUsuarios();

	return foot();
}
void switchConsultarUsuarios() {
	int opcion;
	do {
		opcion = consultarUsuarios();
		switch (opcion) {
		case 1: headConsultarUsuarios();
			cout << "| ----------------------------------- [ 1 ] Consultar por ID unico ------------------------------------- |" << endl << endl;
			consultarUsuarioID(grafoUsuarios);

			system("pause");
			break;
		case 2: headConsultarUsuarios();
			cout << "| ------------------------------------ [ 2 ] Consultar por nombre -------------------------------------- |" << endl << endl;
			consultarUsuarioNombre(grafoUsuarios);

			system("pause");
			break;
		case 3: // El usuario decide volver al menú principal.
			break;
		default:
			cout << "Opcion invalida, por favor intentelo de nuevo." << endl << endl;
			system("pause");
		}

	} while (opcion != 3);
}
void menuModificarUsuarios() {
	cout << "  [ 1 ] Buscar por ID unico para modificarlo." << endl
		 << "  [ 2 ] Buscar por nombre para modificarlo." << endl
		 << "  [ 3 ] Volver atras." << endl << endl
		 << "| ------------------------------------------------------------------------------------------------------ |" << endl << endl;
}
int modificarUsuarios() {
	head();
	headGesUsuarios();
	headModificarUsuarios();
	menuModificarUsuarios();

	return foot();
}
void switchModificarUsuarios() {
	int opcion;
	do {
		opcion = modificarUsuarios();
		switch (opcion) {
		case 1: headModificarUsuarios();
			cout << "| ---------------------------- [ 1 ] Buscar por ID unico para modificarlo ------------------------------ |" << endl << endl;
			modificarUsuarioID(grafoUsuarios);

			system("pause");
			break;
		case 2: headModificarUsuarios();
			cout << "| ----------------------------- [ 2 ] Buscar por nombre para modificarlo ------------------------------- |" << endl << endl;
			modificarUsuarioNombre(grafoUsuarios);

			system("pause");
			break;
		case 3: // El usuario decide volver al menú principal.
			break;
		default:
			cout << "Opcion invalida, por favor intentelo de nuevo." << endl << endl;
			system("pause");
		}

	} while (opcion != 3);
}
void menuEliminarUsuarios() {
	cout << "  [ 1 ] Eliminar por ID unico." << endl
		 << "  [ 2 ] Eliminar por nombre."	 << endl
		 << "  [ 3 ] Volver atras."  << endl << endl
		 << "| ------------------------------------------------------------------------------------------------------ |" << endl << endl;
}
int eliminarUsuarios() {
	head();
	headGesUsuarios();
	headEliminarUsuarios();
	menuEliminarUsuarios();

	return foot();
}
void switchEliminarUsuarios() {
	int opcion;
	do {
		opcion = eliminarUsuarios();
		switch (opcion) {
		case 1: headEliminarUsuarios();
			cout << "| ----------------------------------- [ 1 ] Eliminar por ID unico -------------------------------------- |" << endl << endl;
			eliminarUsuarioID(grafoUsuarios);

			system("pause");
			break;
		case 2: headEliminarUsuarios();
			cout << "| ------------------------------------ [ 2 ] Eliminar por nombre --------------------------------------- |" << endl << endl;
			eliminarUsuarioNombre(grafoUsuarios);

			system("pause");
			break;
		case 3: // El usuario decide volver al menú principal.
			break;
		default:
			cout << "Opcion invalida, por favor intentelo de nuevo." << endl << endl;
			system("pause");
		}

	} while (opcion != 3);
}
void menuGesAmistades() {
	cout << "  [ 1 ] Establecer amistad entre dos usuarios."	<< endl
		 << "  [ 2 ] Mostrar amistades de un usuario."			<< endl
		 << "  [ 3 ] Eliminar amistad entre dos usuarios."		<< endl																							// ESTE ES OPCIONAL, SOLO HACER SI NOS DA LA GANA
		 << "  [ 4 ] Volver atras."						<< endl << endl
		 << "| ------------------------------------------------------------------------------------------------------ |" << endl << endl;
}
int gesAmistades() {
	head();
	headGesAmistades();
	menuGesAmistades();

	return foot();
}
void switchGesAmistades() {
	int opcion;
	do {
		opcion = gesAmistades();
		switch (opcion) {
		case 1: headGesAmistades();
			cout << "| --------------------------- [ 1 ] Establecer amistad entre dos usuarios ------------------------------ |" << endl << endl;
			agregarAmigo(grafoUsuarios);

			system("pause");
			break;
		case 2: headGesAmistades();
			cout << "| ------------------------------ [ 2 ] Mostrar amistades de un usuario --------------------------------- |" << endl << endl;
			consultarAmigos(grafoUsuarios);

			system("pause");
			break;
		case 3: headGesAmistades();
			cout << "| ----------------------------- [ 3 ] Eliminar amistad entre dos usuarios ------------------------------ |" << endl << endl;
			eliminarAmigo(grafoUsuarios);

			system("pause");
			break;
		case 4: // El usuario decide volver al menú principal.
			break;
		default:
			cout << "Opcion invalida, por favor intentelo de nuevo." << endl << endl;
			system("pause");
		}
	} while (opcion != 4);
}
void menuBusConexiones() {
	cout << "  [ 1 ] Buscar ruta de conexion entre dos usuarios."							<< endl
		 << "  [ 2 ] Volver atras."													<< endl << endl
		 << "| ------------------------------------------------------------------------------------------------------ |" << endl << endl;
}
int busConexiones() {
	head();
	headBusConexiones();
	menuBusConexiones();

	return foot();
}
void switchBusConexiones() {
	int opcion;
    do {
        opcion = busConexiones();

        switch (opcion) {
        case 1: {
            headBusConexiones();
            cout << "| ----------------------------- [ 1 ] Buscar ruta de conexion ------------------------------------------ |" << endl << endl;

            int origen = -1, destino = -1;
			int modoOrigen = 0, modoDestino = 0;

			    cout << "Seleccione el metodo de busqueda para el usuario de origen:\n"
                 << "  [1] Buscar por ID\n"
                 << "  [2] Buscar por nombre o fragmento de nombre\n\n"
                 << "Opcion: ";
            cin >> modoOrigen;
            cout << endl;

            if (modoOrigen == 1) {
                // ----- buscar origen por ID -----
                cout << "Ingrese el ID del usuario de origen: ";
                cin >> origen;
                cout << endl;

                if (grafoUsuarios.find(origen) == grafoUsuarios.end()) {
                    cout << "Error: No existe un usuario con ID [" << origen << "].\n\n";
                    system("pause");
                    break;
                }

            } else if (modoOrigen == 2) {
                // ----- buscar origen por nombre -----
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                string criterio;

                cout << "Ingrese el nombre o fragmento del usuario de origen: ";
                getline(cin, criterio);
                cout << endl;

                vector<int> matches = buscarCoincidenciasNombre(grafoUsuarios, criterio);
                origen = seleccionarUsuarioDeCoincidencias(grafoUsuarios, matches);

                if (origen == -1) {
                    cout << "Operacion cancelada.\n\n";
                    system("pause");
                    break;
                }

            } else {
                cout << "Opcion invalida.\n\n";
                system("pause");
                break;
            }
            cout << "Seleccione el metodo de busqueda para el usuario de destino:\n"
                 << "  [1] Buscar por ID\n"
                 << "  [2] Buscar por nombre o fragmento de nombre\n\n"
                 << "Opcion: ";
            cin >> modoDestino;
            cout << endl;

            if (modoDestino == 1) {
                // ----- buscar destino por ID -----
                cout << "Ingrese el ID del usuario de destino: ";
                cin >> destino;
                cout << endl;

                if (grafoUsuarios.find(destino) == grafoUsuarios.end()) {
                    cout << "Error: No existe un usuario con ID [" << destino << "].\n\n";
                    system("pause");
                    break;
                }

            } else if (modoDestino == 2) {
                // ----- buscar destino por nombre -----
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                string criterio;

                cout << "Ingrese el nombre o fragmento del usuario de destino: ";
                getline(cin, criterio);
                cout << endl;

                vector<int> matches = buscarCoincidenciasNombre(grafoUsuarios, criterio);
                destino = seleccionarUsuarioDeCoincidencias(grafoUsuarios, matches);

                if (destino == -1) {
                    cout << "Operacion cancelada.\n\n";
                    system("pause");
                    break;
                }

            } else {
                cout << "Opcion invalida.\n\n";
                system("pause");
                break;
            }

            EstadoBFS estado = bfsRutaMasCorta(grafoUsuarios, origen, destino);

            switch (estado) {
            case ID_INVALIDO:
                cout << "Error: Uno u ambos de los ID's ingresados no existen.\n\n";
                break;

            case SIN_CONEXION:
                cout << "No existe ninguna ruta de conexion entre los usuarios [" 
                     << origen << "] y [" << destino << "].\n\n";
                break;

            case TRIVIAL:
                cout << "Los usuarios son lo mismo. Ruta trivial:\n"
                     << "  [" << origen << "] " << grafoUsuarios[origen].nombre << "\n\n";
                break;

            case OK:
                cout << "Ruta mas corta encontrada entre [" << origen << "] y [" << destino << "]:\n\n  ";

                for (int i = 0; i < (int)ruta.size(); ++i) {
                    int id = ruta[i];
                    cout << "[" << id << "] " << grafoUsuarios[id].nombre;

                    if (i < (int)ruta.size() - 1) {
                        cout << "  ->  ";
                    }
                }

                cout << "\n\n";
                break;
            }

            system("pause");
            break;
        }

        case 2:
            // volver atras
            break;

        default:
            cout << "Opcion invalida, por favor intentelo de nuevo.\n\n";
            system("pause");
        }

    } while (opcion != 2);
}
void menuReportes() {
	cout << "  [ 1 ] Listar usuarios ordenados alfabeticamente."							<< endl
		 << "  [ 2 ] Listar usuarios ordenados por cantidad de amigos (de mayor a menor)."	<< endl
		 << "  [ 3 ] Volver atras."													<< endl << endl
		 << "| ------------------------------------------------------------------------------------------------------ |" << endl << endl;
}
int reportes() {
	head();
	headReportes();
	menuReportes();

	return foot();
}
void switchReportes() {
	int opcion;
	do {
		opcion = reportes();
		switch (opcion) {
		case 1: headReportes();
			cout << "| ------------------------- [ 1 ] Listar usuarios ordenados alfabeticamente ---------------------------- |" << endl << endl;
			
			system("pause");
			break;
		case 2: headReportes();
			cout << "| -------------- [ 2 ] Listar usuarios ordenados por cantidad de amigos (de mayor a menor) ------------- |" << endl << endl;

			system("pause");
			break;
		case 3: // El usuario decide volver al menú principal.
			break;
		default:
			cout << "Opcion invalida, por favor intentelo de nuevo." << endl << endl;
			system("pause");
		}
	} while (opcion != 3);
}

// | ---------- main ---------- | //

int main() {
	bienvenida();

	return 0;
}