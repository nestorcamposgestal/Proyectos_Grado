//Definicio necesaria per poder incloure la llibreria i que trobi el main
#define SDL_MAIN_HANDLED

#include <windows.h>
#include "Joc.h" //Aquest inclou llibreria grafica
#include <conio.h>      /* getch */ 


//Arguments necesaris per poder incloure la llibreria i que trobi el main
void main(int argc, char* argv[])
{
	//Instruccions necesaries per poder incloure la llibreria i que trobi el main
	SDL_SetMainReady();
	SDL_Init(SDL_INIT_VIDEO);
	///////////////////////////////////////////////////////////////////////////

	joc();


	//Instruccio necesaria per poder incloure la llibreria i que trobi el main
	SDL_Quit();
	///////////////////////////////////////////////////////////////////////////

}

