#pragma once

///////////////////////////////////////////////////////////////////////////
//Llibreria grafica
#include "lib/libreria.h"
///////////////////////////////////////////////////////////////////////////

#include <conio.h>      /* getch */ 
#include <stdio.h>      /* printf */
#include <time.h>       /* time */
#include <stdlib.h>
#include "Moviment.h"


//========================================================================
// Mida Pantalla
const int MIDAX = 624;
const int MIDAY = 680;

//========================================================================
// Inici del taulell respecte la cantonada superior esquerre
const int INIPANTALLAX = 0;
const int INIPANTALLAY = 58;

//========================================================================
// Fi del taulell respecte la cantonada inferior dreta
const int FIPANTALLAX = 624;
const int FIPANTALLAY = 680;

const int MIDACASELLA = 78;
// Coordenades de pantalla on mostrar el jugador del torn actual const 
const int POSICIO_TORN_X = 120; 
const int POSICIO_TORN_Y = 10;
// Coordenades de pantalla on mostrar el missatge d'avís de  
// moviment invalid 
const int POSICIO_AVIS_X = 250;
const int POSICIO_AVIS_Y = 10;


int aleatori(int min, int max, int pas);

void joc();

void getPosMatrix(int x, int y, int(&fila), int(&columna));

/*----- END --------------*/