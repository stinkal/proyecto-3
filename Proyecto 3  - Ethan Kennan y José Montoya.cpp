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

// | ---------- Sección de funcionalidad: 1. Gestión de Usuarios  ---------- | //

void agregarUsuario(GrafoUsuarios& grafo) { 
	int id = 0;
	string nombre = "";

	cout << "Ingrese el ID unico del nuevo usuario: "; cin >> id; cout << endl;

	if (grafo.find(id) != grafo.end()) { // Si lo encuentra y no es end(), significa que ya existe. end() indica que llegó al final del mapa.
		cout << "Error: Ya existe un usuario con ID [" << id << "]. Intentelo de nuevo." << endl;
	}
	else {
		cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Se limpia el buffer antes de usar getline().

		cout << "Ingrese el nombre del nuevo usuario: "; getline(cin, nombre); cout << endl;

		grafo.emplace(id, Usuario(id, nombre)); // emplace() construye el objeto Usuario directamente en el mapa, aprovechándose del constructor.

		cout << "El usuario con ID [" << id << "] y nombre '" << nombre << "' fue agregado exitosamente.\n\n";
	}
}
// Agregar usuario: devuelve true si se pudo, false si el ID ya existe.
bool agregarUsuario(GrafoUsuarios& grafo, int id, const string& nombre) {																						// ESTE ES SOLO DE PRUEBA
	if (grafo.find(id) != grafo.end()) {
		return false; // ID repetido
	}
	grafo.emplace(id, Usuario{ id, nombre });
	return true;
}
void consultarUsuarioID(GrafoUsuarios& grafo) { 
	int id = 0;

	cout << "Ingrese el ID unico del usuario a buscar: "; cin >> id; cout << endl;

	if (grafo.find(id) != grafo.end()) {
		cout << "El usuario con el ID [" << id << "] fue encontrado exitosamente y es el siguiente:\n\n"
			 << toStringUsuario(grafo[id]) << endl; // Se muestra la información del usuario encontrado.
	}
	else {
		cout << "Error: No existe un usuario con ID [" << id << "]. Intentelo de nuevo.\n\n";
	}
}
void consultarUsuarioNombre(GrafoUsuarios& grafo) {
	string nombre = "";
	vector<int> coincidentes; // Vector para almacenar los IDs de los usuarios que coinciden.
	bool encontrado = false;
	int opcion = 0;

	cin.ignore(numeric_limits<streamsize>::max(), '\n');

	cout << "Ingrese el nombre del usuario a buscar: "; getline(cin, nombre); cout << endl;

	// Se hace una búsqueda lineal por nombre.
	for (const auto& par : grafo) { // par.first hace referencia al ID, y par.second al nombre del usuario.
		const Usuario& usuario = par.second;

		if (usuario.nombre == nombre) {
			coincidentes.push_back(usuario.iD); // push_back() agrega el ID al final del vector.
		}
	}
	if (coincidentes.empty()) { // Si el vector queda vacío, no se encontraron coincidencias.
		cout << "No se encontro ningun usuario con el nombre '" << nombre << "'. Intentelo de nuevo.\n\n";

		return; // Se sale de la función.
	}
	// Si solo hay uno que coincide, se muestra. 
	if (coincidentes.size() == 1)  {
		cout << "El usuario con ese nombre fue encontrado exitosamente y es el siguiente:\n\n"
			 << toStringUsuario(grafo[coincidentes[0]]) << endl;

		return;
	}
	// Si hay varios que coinciden, se listan todos y se deja elegir al usuario el que desea editar. 
	cout << "Se encontraron " << coincidentes.size() << " usuarios con el nombre '" << nombre << "':\n\n";

	for (int i = 0; i < coincidentes.size(); ++i) {
		const Usuario& usuario = grafo[coincidentes[i]]; // Se obtiene el usuario correspondiente al ID almacenado en el vector.

		cout << "  [ " << (i + 1) << " ] ID: " << usuario.iD << " - " << usuario.nombre << '\n';
	}
	cout << "\nIngrese el numero del usuario que desea consultar (1-" << coincidentes.size() << "): "; // Del 1 a n.

	while (true) { // Bucle hasta que se ingrese una opción válida.
		cin >> opcion;
		if (opcion < 1 || opcion > coincidentes.size()) { // Validación de entradas.
			
			cout << "\nOpcion invalida. Ingrese un numero entre 1 y " << coincidentes.size() << ": ";

			// Se limpia el estado de error y se descarta la entrada inválida.
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
		}
		else {
			break;
		}
	}
	const Usuario& seleccionado = grafo[coincidentes[opcion - 1]];

	cout << "\n" << toStringUsuario(seleccionado) << endl; // Al final se muestra la información del usuario seleccionado.
}
void modificarUsuarioID(GrafoUsuarios& grafo) {
	int id = 0, idNuevo = 0;
	string nombre = "";

	cout << "Ingrese el ID unico del usuario por modificar: "; cin >> id; cout << endl;

	if (grafo.find(id) != grafo.end()) {
		cout << "El usuario con el ID [" << id << "] fue encontrado exitosamente y es el siguiente:\n\n"
			 << toStringUsuario(grafo[id]) << endl; // Se muestra la información del usuario encontrado.

		while (true) {
			cout << "Ingrese el nuevo ID para el usuario: "; cin >> idNuevo; cout << endl;

			if (grafo.find(idNuevo) != grafo.end()) {
				cout << "Error: Ya existe un usuario con ID [" << idNuevo << "]. Intentelo de nuevo." << endl;
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
	int id = 0, idNuevo = 0;
	string nombre = "", nombreNuevo = "";
	vector<int> coincidentes; // Vector para almacenar los IDs de los usuarios que coinciden.
	bool encontrado = false;
	int opcion = 0;

	cin.ignore(numeric_limits<streamsize>::max(), '\n');

	cout << "Ingrese el nombre del usuario por modificar: "; getline(cin, nombre); cout << endl;

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
	// Si solo hay uno que coincide, se muestra. 
	if (coincidentes.size() == 1) {
		cout << "El usuario con ese nombre fue encontrado exitosamente y es el siguiente:\n\n"
			<< toStringUsuario(grafo[coincidentes[0]]) << endl;

		while (true) {
			cout << "\nIngrese el nuevo id para el usuario: "; cin >> idNuevo; cout << endl;

			if (grafo.find(idNuevo) != grafo.end()) {
				cout << "Error: Ya existe un usuario con ID [" << idNuevo << "]. Intentelo de nuevo." << endl;
			}
			else {
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
				cout << "Ingrese el nuevo nombre para el usuario: "; cin >> nombreNuevo; cout << endl;

				Usuario usuario = grafo[id];	 // Se copia el usuario actual.
				grafo.erase(id);				 // Se elimina el usuario con el ID antiguo.
				usuario.iD = idNuevo;			 // Se actualiza el ID.
				usuario.nombre = nombreNuevo;	 // Se actualiza el nombre.
				grafo.emplace(idNuevo, usuario); // Se agrega el usuario con el nuevo ID y nombre.

				cout << "El usuario ha sido modificado exitosamente.\n\n";

				return;
			}
		}
	}
	// Si hay varios que coinciden, se listan todos y se deja elegir al usuario el que desea editar. 
	cout << "Se encontraron " << coincidentes.size() << " usuarios con el nombre '" << nombre << "':\n\n";

	for (int i = 0; i < coincidentes.size(); ++i) {
		const Usuario& usuario = grafo[coincidentes[i]]; // Se obtiene el usuario correspondiente al ID almacenado en el vector.

		cout << "  [ " << (i + 1) << " ] ID: " << usuario.iD << " - " << usuario.nombre << '\n';
	}
	cout << "\nIngrese el numero del usuario que desea modificar (1-" << coincidentes.size() << "): "; // Del 1 a n.

	while (true) { // Bucle hasta que se ingrese una opción válida.
		cin >> opcion;
		if (opcion < 1 || opcion > coincidentes.size()) { // Validación de entradas.

			cout << "\nOpcion invalida. Ingrese un numero entre 1 y " << coincidentes.size() << ": ";

			// Se limpia el estado de error y se descarta la entrada inválida.
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
		}
		else {
			break;
		}
	}
	const Usuario& seleccionado = grafo[coincidentes[opcion - 1]];

	while (true) {
		cout << "\nIngrese el nuevo id para el usuario: "; cin >> idNuevo; cout << endl;

		if (grafo.find(idNuevo) != grafo.end()) {
			cout << "Error: Ya existe un usuario con ID [" << idNuevo << "]. Intentelo de nuevo." << endl;
		}
		else {
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cout << "Ingrese el nuevo nombre para el usuario: "; cin >> nombreNuevo; cout << endl;

			Usuario usuario = grafo[id];	 // Se copia el usuario actual.
			grafo.erase(id);				 // Se elimina el usuario con el ID antiguo.
			usuario.iD = idNuevo;			 // Se actualiza el ID.
			usuario.nombre = nombreNuevo;	 // Se actualiza el nombre.
			grafo.emplace(idNuevo, usuario); // Se agrega el usuario con el nuevo ID y nombre.

			cout << "El usuario ha sido modificado exitosamente.\n\n";

			return;
		}
	}
}
void eliminarUsuarioID(GrafoUsuarios& grafo) {
	int id = 0, idNuevo = 0;
	string nombre = "", decision = "";

	cout << "Ingrese el ID unico del usuario por eliminar: "; cin >> id; cout << endl;

	if (grafo.find(id) != grafo.end()) {
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
void switchGesUsuarios(int);

void menuConsultarUsuarios();
int  consultarUsuarios();
void switchConsultarUsuarios(int);

void menuModificarUsuarios();
int  modificarUsuarios();
void switchModificarUsuarios(int);

void menuEliminarUsuarios();
int  eliminarUsuarios();
void switchEliminarUsuarios(int);

void menuGesAmistades();
int  amistades();
void switchGesAmistades(int);

void menuReportes();
int  reportes();
void switchReportes(int);

// Implementación de las funciones de la interfaz.
void bienvenida() {
	cout << "| ---------------------- Bienvenidos al simulador de una red social - Kentoya !!! ---------------------- |" << endl << endl;
	
	// grafo de usuarios de prueba:
	agregarUsuario(grafoUsuarios, 1, "Walter");
	agregarUsuario(grafoUsuarios, 2, "Jesse");
	agregarUsuario(grafoUsuarios, 3, "Gus");
	agregarUsuario(grafoUsuarios, 4, "Saul");
	agregarUsuario(grafoUsuarios, 5, "Walter");

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
	cout << "| ------------------------------------ [ 3 ] Eliminar un usuario --------------------------------------- |" << endl << endl;
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

	cout << "Digite el numero de opcion a la cual desea ingresar: "; cin >> opcion; cout << endl;

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
			switchGesUsuarios(opcion);

			system("pause");
			break;
		case 2: headGesAmistades();		// Funcionalidad 2: Gestión de Amistades.
			switchGesAmistades(opcion);
			
			system("pause");
			break;
		case 3: headBusConexiones();	// Funcionalidad 3: Búsqueda de Conexiones.

			system("pause");
			break;
		case 4: headReportes();			// Funcionalidad 4: Reportes. 
			switchReportes(opcion);

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
void switchGesUsuarios(int opcion) {
	do {
		opcion = usuarios();
		switch (opcion) {
		case 1: headGesUsuarios();
			cout << "| ---------------------------------- [ 1 ] Agregar un nuevo usuario ------------------------------------ |" << endl << endl;
			agregarUsuario(grafoUsuarios);

			system("pause");
			break;
		case 2:
			cout << "| ------------------------------------ [ 2 ] Consultar un usuario -------------------------------------- |" << endl << endl;
			switchConsultarUsuarios(opcion);

			system("pause");
			break;
		case 3: headGesUsuarios();
			cout << "| ------------------------------------ [ 3 ] Modificar un usuario -------------------------------------- |" << endl << endl;
			switchModificarUsuarios(opcion);

			system("pause");
			break;
		case 4: headGesUsuarios();
			cout << "| ------------------------------------- [ 4 ] Eliminar un usuario -------------------------------------- |" << endl << endl;
			switchEliminarUsuarios(opcion);

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
void switchConsultarUsuarios(int opcion) {
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
void switchModificarUsuarios(int opcion) {
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
void switchEliminarUsuarios(int opcion) {
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
int amistades() {
	head();
	headGesAmistades();
	menuGesAmistades();

	return foot();
}
void switchGesAmistades(int opcion) {
	do {
		opcion = amistades();
		switch (opcion) {
		case 1: headGesAmistades();
			cout << "| --------------------------- [ 1 ] Establecer amistad entre dos usuarios ------------------------------ |" << endl << endl;

			system("pause");
			break;
		case 2: headGesAmistades();
			cout << "| ------------------------------ [ 2 ] Mostrar amistades de un usuario --------------------------------- |" << endl << endl;

			system("pause");
			break;
		case 3: headGesAmistades();
			cout << "| ----------------------------- [ 3 ] Eliminar amistad entre dos usuarios ------------------------------ |" << endl << endl;

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
void switchReportes(int opcion) {
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