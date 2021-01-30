#include "Joc.h"
#include "Tauler.h"
#include "Moviment.h"
#include "Cua.h"
class Partida
{
public:
	Partida(int opcio);
	void inicialitza() { T.inicialitza();}
	void visualitza() { T.dibuixa(torn,missatge); }
	void processaClick(int xMouse, int yMouse, bool &click);
	void processaClickMoviments(int xMouse, int yMouse, bool &click);
	void canviaTorn();
	bool ferMoviment(int opcio);
	bool ferMovimentMoviments(int Xini, int Yini, int Xfin, int Yfin, int torn, bool captura);
	int getColumnaOrigen()const { return columnaorigen+1; }
	int getColumnaDesti()const { return columnadesti+1; }
	int getFilaOrigen()const { return filaorigen+1; }
	int getFilaDesti()const { return filadesti+1; }
	bool comptadorPeces();
	void transformacioDama() { T.transformarDama(torn); }
	void carregaMoviments(Screen &jocLib);	
	void ferMoviments(int i, bool &click, Screen &jocLib);
	void EscriureMoviments();
	void BorrarMoviments();
	int getTorn() { return torn; }

	~Partida();

private:
	Tauler T;
	int torn;
	bool missatge;
	bool movValid;
	int x;
	int y;
	int columnaorigen;
	int filaorigen;
	int columnadesti;
	int filadesti;
	bool captura;
	string nomFitxer;
	Moviment *MatriuMoviments;
	Cua cua;
	int movimentsfets;
};

