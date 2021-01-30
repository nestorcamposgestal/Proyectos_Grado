#include "Cua.h"



Cua::Cua()
{
    movimentsfets=0;
}





void Cua::afegeix(Moviment mov) //Afegim un nou node a la cua que el posarem al final de tot
{   
    Node *aux1;
    aux1=new Node;
    aux1 -> setValor(mov);
    aux1->setNext(NULL);

    
    if (esBuida())
    {
        m_primer=aux1;
        m_ultim=aux1;
    }
    else
    {
		m_ultim->setNext(aux1);
		
        m_ultim = aux1;
    }

	movimentsfets++;

}

void Cua::treure()//Eliminem el primer element de la cua
{
    Node *aux;

    if (m_primer->getNext()!=NULL)
    {
        aux=m_primer;
        m_primer=m_primer->getNext();
		delete aux;
    }
	else
    {
        aux=m_primer;
		delete aux;
    }
}


