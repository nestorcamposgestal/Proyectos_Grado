#include "lib/libreria.h"
#include <stdio.h>      /* printf */
#include <time.h>       /* time */
#include <stdlib.h>
#include "Moviment.h"
#include "Joc.h"
#include <Windows.h>
#include <conio.h>
#include <iostream>
#include <fstream> //fichero
using namespace std; 

class Tauler
{
public:
	void inicialitza();
	void dibuixa(int torn, bool missatge);
	bool validaClick(int x, int y, int &fila, int &columna);
	bool processaMoviment(int filaorigen, int columnaorigen, int filadesti, int columnadesti, int torn, bool &captura);
	void marcaCasella(int fila, int columna, int torn);
	bool checkOrigenValid(int filaOrigen, int columnaOrigen, int torn);
	int fitxaCasella(int fila, int columna) { return MatriuDames[fila][columna]; }
	int comptadorMatriu(int torn);
	bool transformarDama(int torn);
	bool mirarCami(int filaorigen, int columnaorigen, int filadesti, int columnadesti);
	bool mouDiagonal(int filaorigen, int columnaorigen, int filadesti, int columnadesti);
	bool capturaDamaNegra(int filaorigen, int columnaorigen, int filadesti, int columnadesti);
	bool mirarCamiDamaCaptura(int filaorigen, int columnaorigen, int filadesti, int columnadesti);
	bool capturaDamaBlanca(int filaorigen, int columnaorigen, int filadesti, int columnadesti);
	void FitxerMatriu();
	//void carregaMoviments();
	//void ferMoviments(int i);
private:
	int MatriuDames[8][8];
	//Moviment *MatriuMoviments;
	Sprite fons;
	Sprite DamaBlancaSel;
	Sprite DamaNegraSel;
	Sprite DamaNegra;
	Sprite DamaBlanca;
	Sprite FitxaBlanca;
	Sprite FitxaNegra;
	Sprite AvisError;
	Sprite FitxaBlancaSel;
	Sprite FitxaNegraSel;
	Sprite TornBlanc;
	Sprite TornNegra;

	int fitxesBlanques;
	int fitxesNegres;
	string nomFitxer;
	string file;
};

