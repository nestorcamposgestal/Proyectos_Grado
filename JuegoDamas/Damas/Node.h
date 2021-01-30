#include "Moviment.h"

class Node 
{ 
public:
	Moviment & getValor() { return m_valor; }
	Node* getNext() { return m_next; }
	void setValor(const Moviment& valor) { m_valor = valor; }
	void setNext(Node* next) { m_next = next; } 
private:  
	Moviment m_valor; 
	Node* m_next; 
};
