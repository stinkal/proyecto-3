// Instituto Tecnológico de Costa Rica
// Carrera de Ingeniería Electrónica
// Curso de Análisis y Diseño de Algoritmos
// Grupo 21
// Proyecto 3
// Estudiantes programadores: Ethan Kennan y José Montoya
// Profesor: Bryan Hernández Sibaja
// Alajuela, Costa Rica
// II Semestre, 2025

#include <iostream> // Bibloteca para flujo de datos de entrada y salida.
#include <string>	// Bibloteca para hacer uso del getline().
#include <sstream>  // Bibloteca para usar stringstream en toString().

using namespace std;

// | ---------- Seccion de la interfaz ---------- | //

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

void menuGesAmistades();
int  amistades();
void switchGesAmistades(int);

void menuReportes();
int  reportes();
void switchReportes(int);

// Implementación de las funciones de la interfaz.
void bienvenida() {
	cout << "| ---------------------- Bienvenidos al simulador de una red social - Kentoya !!! ---------------------- |" << endl << endl;
	
	// Aquí va la inicialización de las estructuras, arreglos, y demás..

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

			system("pause");
			break;
		case 2: headGesUsuarios();
			cout << "| ------------------------------------ [ 2 ] Consultar un usuario -------------------------------------- |" << endl << endl;
			
			system("pause");
			break;
		case 3: headGesUsuarios();
			cout << "| ------------------------------------ [ 3 ] Modificar un usuario -------------------------------------- |" << endl << endl;

			system("pause");
			break;
		case 4: headGesUsuarios();
			cout << "| ------------------------------------- [ 4 ] Eliminar un usuario -------------------------------------- |" << endl << endl;

			system("pause");
			break;
		case 5: headGesUsuarios();
			cout << "| --------------------------------- [ 5 ] Mostrar todos los usuarios ----------------------------------- |" << endl << endl;

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