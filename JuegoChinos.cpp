#include <iostream>
#include <ctime>
#include <climits>
#include <cstdlib>
#include <fstream>
#include <string>
#include <cctype>
#include <iomanip>
using namespace std;
enum tGanador { NADIE, MAQUINA, HUMANO };

//Funcion que transforma minusculas a mayusculas
string mayusculas(string resp){
	for (int i = 0; i < resp.size(); ++i){
		resp.at(i) = toupper(resp.at(i));
	}
	return resp;
}

//Funcion que vuelve a pedir un valor despues de un fallo
int fallo(){
	int falla = 0; string aux = "";
	cin.clear(); cin.sync();
	cout << "\7\nPor favor, introduzca un valor valido: ";
	cin >> falla;
	cin.ignore(1000, '\n');
	return falla;
}

//Funcion que elimina el fallo al introducir un string
string fallotext(){
	string falla = " ";
	cin.clear(); cin.sync();
	cout << "\7\nPor favor, introduzca (SI/NO): ";
	getline(cin, falla);
	falla = mayusculas(falla);
	return falla;
}

//Funcion donde se abre el fichero configP1 para cargar reglas de partidas anteriores
void configuracion(int & marcador, int & nummonedas, int & numrondas){
	ifstream conf; bool cond = false;
	conf.open("configP1.txt");
	cond = conf.is_open();
	if (cond){
		conf >> marcador >> nummonedas >> numrondas;
		cout << "El marcador actual es " << marcador << ", el numero de chips es "
			<< nummonedas << "\ny podras abandonar la partida cada " << numrondas << " ronda(s)\n";
	}
	else cout << "No se ha podido abrir el archivo.\nSe ha establecido la configuracion predeterminada\n";
	conf.close();
}

// Menu con las diferentes opciones posibles
int menu(){
	int nummenu = 0;
	cout << setw(76) << setfill('-') << '\n';
	cout << "Juego de los Chinos:\n\n";
	cout << "1 - Cambiar el marcador\n";
	cout << "2 - Cambiar el numero de chips\n";
	cout << "3 - Cambiar numero de rondas para salir\n";
	cout << "4 - Acerca del juego de los chinos\n";
	cout << "5 - Jugar una partida\n";
	cout << "6 - Jugar una partida automatica\n\n";
	cout << "0 - Salir\n\n";
	cout << "Introduzca una opcion: ";
	cin >> nummenu;
	cin.ignore(1000, '\n');
	while (cin.fail() || nummenu < 0 || nummenu >6)	nummenu = fallo();
	return nummenu;
}

//Funcion que cambia la configuracion del marcador (Opcion 1 menu)
int metmarc(){
	int marcador = 0; string aux = "";
	cout << "\nElija nuevo valor de marcador: ";
	cin >> marcador;
	cin.ignore(1000, '\n');
	while (cin.fail() || marcador < 1) marcador = fallo();
	return marcador;
}

//Funcion que cambia la configuracion de chips (Opcion 2 menu)
int metmon(){
	int nummonedas = 0;
	cout << "\nElija nuevo maximo de chips: ";
	cin >> nummonedas;
	cin.ignore(1000, '\n');
	while (cin.fail() || nummonedas < 1) nummonedas = fallo();
	return nummonedas;
}

//Funcion que cambia el numero de rondas.
int metron(){
	int numrondas = 0;
	cout << "\nElija cada cuantas rondas puede abandonar la partida: ";
	cin >> numrondas;
	cin.ignore(1000, '\n');
	while (cin.fail() || numrondas < 1) numrondas = fallo();
	return  numrondas;
}

// Funcion que muestra las reglas al usuario si este lo pide (Opcion 3 menu)
bool mostrarReglas(){
	bool a = false;
	ifstream reglas;
	string texto = " ";
	reglas.open("Reglas.txt");
	a = reglas.is_open();
	if (a){
		getline(reglas, texto);
		while (texto != "XXX"){
			cout << texto << '\n';
			getline(reglas, texto);
		}
	}
	reglas.close();
	return a;
}

//Funcion que carga los datos de la partida anterior
void cargarpartida(int & partidasganadash, int & partidasganadasm){
	ifstream conf; bool cond = false; int aux1 = 0, aux2 = 0;
	conf.open("Partidaguardada.txt");
	cond = conf.is_open();
	if (cond){
		conf >> aux1 >> aux2;
		partidasganadash = aux1;
		partidasganadasm = aux2;
		cout << "El numero de partidas ganadas por el humano es " << partidasganadash
			<< "\ny el numero de partidas ganadas por la maquina es " << partidasganadasm << '\n';
	}
	else cout << "\nNo se ha podido abrir el archivo.\nSe ha establecido la configuracion predeterminada\n";
	conf.close();

}

//Funcion que pregunta si desea cargar los datos de la partida anterior
void cargardatos(int & partidasganadash, int & partidasganadasm){
	string resp = " ";
	cout << "\nDesea cargar los datos de la partida anterior?(SI/NO): ";
	cin >> resp;
	cin.ignore(1000, '\n');
	resp = mayusculas(resp);
	while (resp != "SI" && resp != "NO") resp = fallotext();
	if (resp == "SI") cargarpartida(partidasganadash, partidasganadasm);
}

//Funcion que comprueba si los marcadores de cada uno superan el marcador general
void comprobacion(int & partidasganadash, int & partidasganadasm, int marcador){
	if (partidasganadash >= marcador || partidasganadasm >= marcador){
		partidasganadash = 0;
		partidasganadasm = 0;
		cout << "No se ha podido cargar los datos de partidas anteriores. Se iniciara con el contador de ambos a 0.\n";
	}
}

/*Funcion donde se establece la cantidad de chips que escoge el humano,
de manera que no introduzca valores erroneos y tipos equivocados*/
int monedasHumano(int nummonedas){
	int monh = 0;
	cout << "Introduzca el numero de chips que saca de 0 a " << nummonedas << ": \n";
	cin >> monh;
	cin.ignore(1000, '\n');
	while (cin.fail() || monh > nummonedas || monh<0){
		monh = fallo();
	}
	return monh;
}

//Funcion que aleatoriamente determina el numero de chips de la maquina con un valor coherente
int monedasMaquina(int nummonedas){
	int monm = 0;
	monm = rand() % (nummonedas + 1);
	return monm;
}

//Funcion que aleatoriamente determina la apuesta de la maquina, asegurando que su valor sea coherente
int apuestaMaquina(int monm, int nummonedas){
	int apum = 0;
	apum = monm + rand() % (1 + nummonedas);
	return apum;
}

/*Funcion donde se introduce la apuesta del humano,
determinando las posibles apuestas y evitando que hagas trampa o metas tipos equivocados */
int apuestaHumano(int apum, int monh, int nummonedas){
	int apuh = 0;
	cout << "Introduzca su apuesta con un valor comprendido entre " << monh << " y " << monh + nummonedas << ": \n";
	cin >> apuh;
	cin.ignore(1000, '\n');
	while (cin.fail() || apuh >= (monh + 1 + nummonedas) || apuh < 0 || apuh<monh || apuh == apum){
		apuh = fallo();
	}
	return apuh;
}

//Funcion que calcula de forma "inteligente" una apuesta para maquina 2.
int apuestaMaquina2(int monh, int nummonedas, int apum){
	int apum2 = apum;
	while (apum2 == apum){
		if (apum == 0 && monh != 0)  apum2 = monh;
		else if (apum == 0 && monh == 0) apum2 = 1;
		else if (apum > nummonedas)  apum2 = (monh + apum - nummonedas - 1) + rand() % (nummonedas + 1);
		else if (apum <= nummonedas) apum2 = monh + rand() % (apum + 1);

	}
	return apum2;
}

//Funcion que compara quien gana la ronda (Humano vs Maquina)
tGanador quienGana(int monm, int monh, int apum, int apuh){
	int suma = 0; tGanador ganador = NADIE;
	suma = monm + monh;
	if (suma == apuh){
		ganador = HUMANO;
		cout << "Felicidades. Has ganado la ronda.\n";
	}
	else if (suma == apum){
		ganador = MAQUINA;
		cout << "Has perdido la ronda\n";
	}
	else cout << "Nadie ha ganado la ronda. Intente de nuevo\n";
	return ganador;
}

//Funcion que compara quien gana la ronda (Maquina vs Maquina)
tGanador quienGana2(int monm, int monh, int apum, int apuh){
	int suma = 0; tGanador ganador = NADIE;
	suma = monm + monh;
	if (suma == apuh){
		ganador = HUMANO;
		cout << "La Maquina 2 gana la ronda\n";
	}
	else if (suma == apum){
		ganador = MAQUINA;
		cout << "La Maquina 1 gana la ronda\n";
	}
	else cout << "Ninguna Maquina gana la ronda\n";
	return ganador;
}

//Funcion que suma uno al marcador de uno de los contrincantes
void ganadorprovisional(int & partidasganadash, int & partidasganadasm, tGanador vencedor, int & contador){
	++contador;
	if (vencedor == HUMANO)  ++partidasganadash;
	else if (vencedor == MAQUINA) ++partidasganadasm;
}

//Funcion que pregunta al jugador si desea abandonar la partida cada cierto numero de rondas
string salidajuego(int contador, int numrondas, int marcador, int partidasganadash, int partidasganadasm){
	string resp = " ";
	if (partidasganadash != marcador && partidasganadasm != marcador && contador % numrondas == 0){
		cout << "aQuieres salir del juego?(SI/NO): ";
		cin >> resp;
		cin.ignore(1000, '\n');
		resp = mayusculas(resp);
		while (resp != "SI" && resp != "NO") resp = fallotext();
	}
	return resp;
}

/*Funcion que guarda los valores de partidasganadash y partidasganadasm
en el caso en el que el jugador abandone con anterioridad la partida*/
void guardarpartida(int partidasganadash, int partidasganadasm){
	ofstream guardar; bool aux = false;
	guardar.open("Partidaguardada.txt");
	aux = guardar.is_open();
	if (aux){
		guardar << partidasganadash << " " << partidasganadasm;
	}
	else cout << "\nNo se ha podido guardar el archivo. Intentelo mas tarde\n";
	guardar.close();
}

//Funcion que pregunta si deseamos guardar los datos
void guardarmarcador(int partidasganadash, int partidasganadasm){
	string resp = " ";
	cout << "Desea guardar la partida actual?(SI/NO): ";
	cin >> resp;
	cin.ignore(1000, '\n');
	resp = mayusculas(resp);
	while (resp != "SI" && resp != "NO") resp = fallotext();
	if (resp == "SI") guardarpartida(partidasganadash, partidasganadasm);
}

//Funcion en el que se lleva a cabo la conduccion de todo el juego
void juego(int marcador, int nummonedas, int numrondas){
	int monm = 0, apum = 0, partidasganadash = 0, partidasganadasm = 0, apuh = 0, monh = 0, contador = 0;
	tGanador vencedor = NADIE; string resp = " ";
	cargardatos(partidasganadash, partidasganadasm);
	comprobacion(partidasganadash, partidasganadasm, marcador);
	while (partidasganadash != marcador && partidasganadasm != marcador && resp != "SI"){
		monh = monedasHumano(nummonedas);
		monm = monedasMaquina(nummonedas);
		apum = apuestaMaquina(monm, nummonedas);
		cout << "Creo que hemos sacado " << apum << " entre los dos\nAhora es tu turno...\n";
		apuh = apuestaHumano(apum, monh, nummonedas);
		cout << "La maquina ha sacado " << monm << " chip(s)\n";
		cout << "Tu has sacado " << monh << " chip(s)\n";
		vencedor = quienGana(monm, monh, apum, apuh);
		ganadorprovisional(partidasganadash, partidasganadasm, vencedor, contador);
		cout << setw(76) << setfill('-') << '\n';
		resp = salidajuego(contador, numrondas, marcador, partidasganadash, partidasganadasm);
	}
	if (partidasganadash == marcador) cout << "Has ganado la partida! Enhorabuena!\nGracias a ti la humanidad no pasa a ser una especie extinta\n";
	else if (partidasganadasm == marcador) cout << "Ha ganado la maquina.\nDebido a tu derrota la humanidad ha sido esclavizada\n";
	else {
		cout << "Suerte la proxima, la guerra aun no ha acabado\n";
		guardarmarcador(partidasganadash, partidasganadasm);
	}
}

//Funcion que lleva a cabo el juego automatico.
void juegoautomatico(int marcador, int nummonedas){
	int monm = 0, monh = 0, apum = 0, apuh = 0, partidasganadasm = 0, partidasganadash = 0, contador = 0, numrondas = 5; tGanador vencedor = NADIE;
	string resp = "";
	while (partidasganadasm != marcador && partidasganadash != marcador && resp != "SI"){
		monm = monedasMaquina(nummonedas);
		cout << "\nLa maquina 1 eligio: " << monm << '\n';
		monh = monedasMaquina(nummonedas);
		cout << "La maquina 2 eligio: " << monh << '\n';
		apum = apuestaMaquina(monm, nummonedas);
		cout << "\nLa maquina 1 aposto: " << apum << '\n';
		apuh = apuestaMaquina2(monh, nummonedas, apum);
		cout << "La maquina 2 aposto: " << apuh << '\n';
		vencedor = quienGana2(monm, monh, apum, apuh);
		ganadorprovisional(partidasganadash, partidasganadasm, vencedor, contador);
		cout << '\n';
		cout << "Pulse cualquier enter para continuar\n";
		getchar();
		cin.ignore(INT_MAX,'\n');
		cin.clear();
		resp = salidajuego(contador, numrondas, marcador, partidasganadash, partidasganadasm);
	}
	if (partidasganadash == marcador) cout << "La maquina 2 ha ganado la partida! Enhorabuena!\nLas maquinas perversas han sido destruidas\n";
	else if (partidasganadasm == marcador) cout << "Ha ganado la maquina 1.\nDebido a tu derrota la humanidad ha sido esclavizada\n";
	else cout << "Las dos maquinas han decidido aliarse y han acabado con el mundo.\nGracias por abandonar, humano\n";
}

//Funcion que guarda la configuracion de la partida en caso que el usuario lo desee
void guardar(int marcador, int nummonedas, int numrondas){
	ofstream guardado; bool aux = false;
	guardado.open("configP1.txt");
	aux = guardado.is_open();
	if (aux){
		guardado << marcador << " " << nummonedas << " " << numrondas;
	}
	else cout << "\nNo se ha podido guardar el archivo. Intentelo mas tarde\n";
	guardado.close();
}

//Funcion que pregunta si desea guardar datos
void guardardatos(int marcador, int nummonedas, int numrondas){
	string resp = " ";
	cout << "Desea guardar la configuracion actual para la proxima partida?(SI/NO): ";
	cin >> resp;
	cin.ignore(1000, '\n');
	resp = mayusculas(resp);
	while (resp != "SI" && resp != "NO") resp = fallotext();
	if (resp == "SI") guardar(marcador, nummonedas, numrondas);
}

int main(){
	srand(time(NULL));  int marcador = 2, nummonedas = 3, nummenu = 0, numrondas = 3; ifstream reglas; string texto = " "; bool b = false;
	configuracion(marcador, nummonedas, numrondas);
	nummenu = menu();
	while (nummenu != 0){
		switch (nummenu){
		case 1:marcador = metmarc(); break;
		case 2:nummonedas = metmon(); break;
		case 3:numrondas = metron(); break;
		case 4:b = mostrarReglas();
			if (!b) cout << "No se ha podido abrir el archivo\n"; break;
		case 5:juego(marcador, nummonedas, numrondas); break;
		case 6:juegoautomatico(marcador, nummonedas);
		}
		nummenu = menu();
	}
	guardardatos(marcador, nummonedas, numrondas);
	return 0;
}
