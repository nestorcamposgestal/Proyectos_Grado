#include "Node.h"
#include "Moviment.h" 
#include <Windows.h>
#include <stdio.h>


class Cua 
{ 
public: 
Cua();
Moviment getPrimer(){return m_primer->getValor();}
Moviment getUltim() {return m_ultim->getValor();}
void afegeix(Moviment mov);
void treure(); 
bool esBuida() {if(movimentsfets==0) return true;else return false;}
int getMov() { return movimentsfets; }

private: 
	Node* m_primer;
	Node* m_ultim; 
	int movimentsfets;
};