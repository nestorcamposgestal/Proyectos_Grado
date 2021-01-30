#include "Partida.h"
#include "Joc.h"
#include <Windows.h>
#include <conio.h>
#include <iostream>
using namespace std;

//BUENO
//-----------------------------------------
// joc: porta el control de tot el joc
//-----------------------------------------
void joc()
{
	// ********************************************
	// Inicialització de la part gràfica del joc
	// *******************************************
	
	//Inicialitza Part grafica 
	Screen jocLib(MIDAX, MIDAY);
	
	//Sprites Fi Partida
	Sprite BlanquesGuanyen;
	Sprite NegresGuanyen;
	BlanquesGuanyen.create("data/blanquesguanyen.png");
	NegresGuanyen.create("data/negresguanyen.png");
	
	//Mostrem finestra
	jocLib.show();
	
	// Variables d'ajuda per agafar posició ratolí
	int xMouse = 0;
	int yMouse = 0;

	//Variable de coordenadas en el tablero del raton INICIALES
	int xMatriuIni = 0;
	int yMatriuIni = 0;

	//Variable de coordenadas en el tablero del raton FINALES
	int xMatriuFin = 0;
	int yMatriuFin = 0;

	bool click = false;
	bool movimentsPrincipi = true;

	

	cout << "MENU D'INICI DEL JOC:" << endl << endl;
	cout << " 1. Inici partida normal " << endl;
	cout << " 2. Guardar partida " << endl;
	cout << " 3. Sortir partida " << endl;
	cout << " 4. Borrar partida guardada " << endl;

	int opcio = 0;
	do
	{
		cin >> opcio;
	} while ((opcio != 1) && (opcio != 2) && (opcio != 3)&&(opcio!=4));

	Partida Partida(opcio);
	Partida.inicialitza();
	if (opcio == 4)
	{
		Partida.BorrarMoviments();
		cout << "-------------------------------------" << endl;
		cout << "La partida s'ha esborrat correctament" << endl;
		cout << "-------------------------------------" << endl << endl;
		Sleep(1000);
		cout << "Torna a introduir la teva opcio:" << endl;
		do
		{
			cin >> opcio;
		} while ((opcio != 1) && (opcio != 2) && (opcio != 3));
	}


	

	
	

	
	
	if ((opcio != 3)&&(opcio!=4))//Només entra si la opció és 1 o 2
	{
		do
		{
			jocLib.processEvents();

			if (Mouse_getButLeft())
			{
				xMouse = Mouse_getX();
				yMouse = Mouse_getY();

				if (!click)
				{
					Partida.processaClick(xMouse, yMouse, click);
					cout << "-----INI----" << Partida.getFilaOrigen() << "----" << Partida.getColumnaOrigen() << endl;

				}
				else
				{
					Partida.processaClick(xMouse, yMouse, click);
					if (Partida.ferMoviment(opcio) == true)
						Partida.canviaTorn();

					Partida.transformacioDama();

					cout << "-----Fin----" << Partida.getFilaDesti() << "----" << Partida.getColumnaDesti() << endl;


				}



			}

			Partida.visualitza();

			// Actualitza la pantalla
			jocLib.update();


			//Hacemos movimientos del inicio
			if ((movimentsPrincipi)&&(opcio==2))
			{
				Partida.carregaMoviments(jocLib);
				movimentsPrincipi = false;

			}


			// *************************
			// Sortim del bucle si pressionem ESC
			// *************************


		} while ((!Keyboard_GetKeyTrg(KEYBOARD_ESCAPE)) && (!Partida.comptadorPeces()));
	}
	if (Partida.comptadorPeces())
	{
		int torn;
		torn = Partida.getTorn();
		if (torn == 1)
			BlanquesGuanyen.draw(0, 0);
		else
			NegresGuanyen.draw(0, 0);
		jocLib.update();
		Sleep(5000);

	}

	if (opcio==2)
		Partida.EscriureMoviments();

	

}
