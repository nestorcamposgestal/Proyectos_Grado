#include "Comunitat.h"

Comunitat::Comunitat(MatriuSparse* pMAdj)
{
	m_primComdeltaQ = -1;
	m_Q = 0;
	m_pMAdj = pMAdj;
}

Comunitat::~Comunitat()
{
	m_pMAdj = nullptr;	
}
void Comunitat::clear()
{
	m_pMAdj = nullptr;
	m_deltaQ.clear();
	m_indexComs.clear();
	m_maxDeltaQFil.clear();
	m_primComdeltaQ=-1;
	m_vDendrograms.clear();
	m_k.clear();
	m_A.clear();
	m_hTotal.clear();
	m_Q=0;
	m_M2=0;	
}

void Comunitat::calculaPosicion(int & pos1, int & pos2)
{

	map<pair<int, int>, double>::iterator it;


	for (int i = 0; i < m_deltaQ.size(); i++)
	{
		if (!m_deltaQ[i].empty())
		{
			it = m_deltaQ[i].begin();
			if ((*it).first.first == pos1)
			{
				pos1 = i;
			}
			if ((*it).first.first == pos2)
			{
				pos2 = i;
			}
		}
	}


}

void Comunitat::calculaVei(int & posVei)
{

	map<pair<int, int>, double>::iterator it;


	for (int i = 0; i < m_deltaQ.size(); i++)
	{
		if (!m_deltaQ[i].empty())
		{
			it = m_deltaQ[i].begin();
			if ((*it).first.first == posVei)
			{
				posVei = i;
			}
		}
	}
}

void Comunitat::calculaA()
{
	double valor;

	for (int i = 0; i < m_k.size(); i++)
	{
		//valor = double(m_k[i]) / m_M2;
		m_A.emplace_back(double(m_k[i]) / m_M2);
	}
	
}
void Comunitat::creaIndexComs()
{
	m_indexComs.resize(m_pMAdj->getNodes());

	for (int i = 0; i < m_indexComs.size(); i++)
	{
		m_indexComs[i].first = i + 1;
		m_indexComs[i].second = i - 1;
	}
	
	m_primComdeltaQ = 0;
}
void Comunitat::creaDeltaQHeap()
{
	m_pMAdj->creaMaps(m_deltaQ);
	//m_hTotal.resize(m_pMAdj->getNodes());
	m_hTotal.resize(m_deltaQ.size());
	m_maxDeltaQFil.resize(m_deltaQ.size());
	double ValorDeltaQ = 0;
	double ValorDeltaQMax = 0;
	int i, j;
	int jMax;

	for (int z = 0; z < m_deltaQ.size(); z++)
	{
		map<pair<int, int>, double>::iterator it;

		for (it = m_deltaQ[z].begin(); it != m_deltaQ[z].end(); it++)
		{
			if ((*it).first.second != -1)
			{


				i = (*it).first.first;
				j = (*it).first.second;

				ValorDeltaQ = (double(1) / double(m_M2)) - ((double(m_k[i]) * double(m_k[j])) / (double(m_M2)*double(m_M2)));

				(*it).second = ValorDeltaQ;

				if (ValorDeltaQ > ValorDeltaQMax || it == m_deltaQ[z].begin())
				{
					ValorDeltaQMax = ValorDeltaQ;
					jMax = j;
				}
			}
			else
			{
				ValorDeltaQMax = double(-2);
				jMax = -1;
			}

		}

		m_maxDeltaQFil[z].first = jMax;
		m_maxDeltaQFil[z].second = ValorDeltaQMax;
		ElemHeap elHeap(ValorDeltaQMax, {z,jMax});
		m_hTotal.insert(elHeap);
		
		ValorDeltaQMax = 0;
		jMax = 0;
	}

	vector<map<pair<int, int>, double>> ::iterator itV = m_deltaQ.begin();
	vector<map<pair<int, int>, double>> ::iterator itAux;
	bool borrar = false;
	int h = 0;

	while(h<m_deltaQ.size())
	{
		map<pair<int, int>, double>::iterator it;

		for (it = m_deltaQ[h].begin(); it != m_deltaQ[h].end(); it++)
		{
			if ((*it).first.second == -1 && (*it).second==-2)
				borrar = true;
		}
		itAux = itV;
		if (borrar)
		{
			itV = m_deltaQ.erase(itAux);
			//h++;
			borrar = false;
		}
		else
		{
			itV++;
			h++;
		}
		
	}


}

void Comunitat::modificaVei(int com1, int com2, int vei, int cas)
{
	//caso 1 --> veins en i y j (veins comunes)
	//caso 2 --> veins en i
	//caso 3 --> veins en j
	int pos1 = com1;
	int pos2 = com2;
	int posVei = vei;
	calculaPosicion(pos1, pos2);
	calculaVei(posVei);

	if (com1 != vei && com2 != vei)
	{
		map<pair<int, int>, double>::iterator itJ = m_deltaQ[pos2].find({ com2,vei });
		map<pair<int, int>, double>::iterator itI = m_deltaQ[pos1].find({ com1,vei });
		map<pair<int, int>, double>::iterator itXJ = m_deltaQ[posVei].find({ vei,com2 });
		map<pair<int, int>, double>::iterator itXI = m_deltaQ[posVei].find({ vei,com1 });

		switch (cas)
		{

		case 1:
			(*itXJ).second += (*itXI).second;
			m_deltaQ[posVei].erase(itXI);
			(*itJ).second += (*itI).second;
			break;

		case 2:
			m_deltaQ[posVei].insert({ {vei,com2},(*itXI).second - double(2) * m_A[com2] * m_A[vei] });
			//(*itXJ).second = (*itXI).second - 2 * m_A[com2] * m_A[vei];
			m_deltaQ[posVei].erase(itXI);
			m_deltaQ[pos2].insert({ {com2,vei},(*itI).second - double(2) * m_A[com2] * m_A[vei] });
			break;

		case 3:
			(*itXJ).second -= double(2) * m_A[com1] * m_A[vei];
			(*itJ).second -= double(2) * m_A[com1] * m_A[vei];
			break;


		default:

			break;
		}

	}
}

void Comunitat::fusiona(int com1, int com2)
{
	//caso 1 --> veins en i y j (veins comunes)
	//caso 2 --> veins en i
	//caso 3 --> veins en j


	int pos1=com1;
	int pos2 = com2;
	calculaPosicion(pos1, pos2);

	int caso1 = 1;
	int caso2 = 2;
	int caso3 = 3;

	int posVectorMaxDeltaQ = m_PrimeraPosicionDeltaQ;

	int jMax;
	double MaxDeltaQ;

	map<int, pair<int,double>> MapModificados;
	map<int, pair<int, double>> ::iterator itMOD;


	map<pair<int, int>, double>::iterator itI = m_deltaQ[pos1].begin();
	map<pair<int, int>, double>::iterator itJ = m_deltaQ[pos2].begin();
	map<pair<int, int>, double>::iterator itFIND;
	map<pair<int, int>, double>::iterator itFIND_Delete;

	map<pair<int, int>, double>::iterator itMAX;

	//Calculos de las nuevas DeltaQ

	//Miramos los veins comunes entre la J y I, y si no, modificamos los veisn de J
	for (itJ; itJ != m_deltaQ[pos2].end(); itJ++)
	{	
		itFIND = m_deltaQ[pos1].find({com1,(*itJ).first.second});
		if (itFIND != m_deltaQ[pos1].end())
			modificaVei(com1, com2, (*itJ).first.second, caso1);
		else
			modificaVei(com1, com2, (*itJ).first.second, caso3);
	}

	//Modificamos los veins de I
	for (itI; itI != m_deltaQ[pos1].end(); itI++)
	{
		itFIND = m_deltaQ[pos2].find({ com2,(*itI).first.second });
		if(itFIND==m_deltaQ[pos2].end())
			modificaVei(com1, com2, (*itI).first.second, caso2);
	}


	itFIND_Delete=m_deltaQ[pos2].find({ com2,com1 });
	m_deltaQ[pos2].erase(itFIND_Delete);
	//m_deltaQ.erase(m_deltaQ.begin()+com1);

	//Miramos en el heap si hay algun valor pereneciente al nodo eliminado
	//m_hTotal.delElem(com1);
	
	//Calculamos DeltaQMax y modificamos Heap
	for (int i = 0; i < m_deltaQ.size(); i++)
	{
		if (!m_deltaQ[i].empty())
		{
			jMax = (*m_deltaQ[i].begin()).first.second;
			MaxDeltaQ = (*m_deltaQ[i].begin()).second;
			for (itMAX = m_deltaQ[i].begin(); itMAX != m_deltaQ[i].end(); itMAX++)
			{
				if ((*itMAX).second > MaxDeltaQ)
				{
					MaxDeltaQ = (*itMAX).second;
					jMax = (*itMAX).first.second;
				}
			}

			//Recordar que la condicion del jMax era por lo que pasaba con el caso del 18,2 y 22,2
			if (MaxDeltaQ > m_maxDeltaQFil[posVectorMaxDeltaQ].second || m_maxDeltaQFil[posVectorMaxDeltaQ].first==com1||jMax==m_maxDeltaQFil[posVectorMaxDeltaQ].first)
			{
				//Modificamos Max_DeltaQ
				m_maxDeltaQFil[posVectorMaxDeltaQ].first = jMax;
				m_maxDeltaQFil[posVectorMaxDeltaQ].second = MaxDeltaQ;

				MapModificados.insert({ posVectorMaxDeltaQ,{jMax,MaxDeltaQ} });

			}
		}
		posVectorMaxDeltaQ++;

	}

	//Modificamos Heap

	//Primero modificamos en el heap los veins del nodo que se va
	for (itMAX = m_deltaQ[pos1].begin(); itMAX != m_deltaQ[pos1].end(); itMAX++)
	{
		itMOD = MapModificados.find((*itMAX).first.second);
		if (((*itMAX).first.second != com2) && itMOD != MapModificados.end())
		{
			ElemHeap nouElHeap((*itMOD).second.second, { (*itMOD).first,(*itMOD).second.first });
			m_hTotal.modifElem(nouElHeap);
			MapModificados.erase(itMOD);
		}
	}

	//Despues modificamos en el heap los veins del nodo que se queda
	for (itMAX = m_deltaQ[pos2].begin(); itMAX != m_deltaQ[pos2].end(); itMAX++)
	{
		itMOD = MapModificados.find((*itMAX).first.second);
		if (itMOD != MapModificados.end())
		{
			ElemHeap nouElHeap((*itMOD).second.second, { (*itMOD).first,(*itMOD).second.first });
			m_hTotal.modifElem(nouElHeap);
			MapModificados.erase(itMOD);
		}
	}

	//Modificamos en el heap el resto
	for (itMOD = MapModificados.begin(); itMOD != MapModificados.end(); itMOD++)
	{
		ElemHeap nouElHeap((*itMOD).second.second, { (*itMOD).first,(*itMOD).second.first });
		m_hTotal.modifElem(nouElHeap);
	}


	m_deltaQ[pos1].clear();

	m_comunidadesActivas--;

	//Modificamos IndexComs
	if (com1 == m_primComdeltaQ)
		m_primComdeltaQ = m_indexComs[com1].first;
	
	if(m_indexComs[com1].first<m_indexComs.size())
		m_indexComs[m_indexComs[com1].first].second = m_indexComs[com1].second;
	if(m_indexComs[com1].second!=-1)
		m_indexComs[m_indexComs[com1].second].first = m_indexComs[com1].first;

	//Modificamos Dendogramas
	m_Q += m_incrementoQ;
	generaDendrogram(com1, com2, m_Q);
	
	//Recalculo de A
	m_A[com2] += m_A[com1];

}

void Comunitat::generaDendrogram(int pos1, int pos2, double deltaQp1p2)
{ 
	Tree<double>* ArbolI;
	Tree<double>* ArbolJ;
	Tree<double>* ArbolDeltaQ = new Tree<double>(deltaQp1p2);

	ArbolI = m_vDendrograms[pos1];
	ArbolJ = m_vDendrograms[pos2];
	
	ArbolDeltaQ->setLeft(ArbolJ);
	ArbolDeltaQ->setRight(ArbolI);
	m_vDendrograms[pos2] = ArbolDeltaQ;
	m_vDendrograms[pos1] = NULL;
	
}

void Comunitat::calculaComunitats(list<Tree<double>*>& listDendrogram)
{
	calculaM2();
	calculaK();
	calculaA();
	creaIndexComs();
	m_primComdeltaQ = 0;
	creaDeltaQHeap();
	InicialitzaDendrograms();
	ElemHeap Arrel;
	Arrel = m_hTotal.max();
	m_incrementoQ = m_hTotal.max().getVal();
	m_comunidadesActivas = m_deltaQ.size();
	m_PrimeraPosicionDeltaQ = (*m_deltaQ[0].begin()).first.first;

	while (m_comunidadesActivas>1 && Arrel.getVal()>0)
	{
		m_hTotal.delMax();
		fusiona(Arrel.getPos().first, Arrel.getPos().second);
		Arrel = m_hTotal.max();
		m_incrementoQ = m_hTotal.max().getVal();
	}

	//Hacer lista

	int i = m_primComdeltaQ;
	while (i < m_indexComs.size())
	{
		listDendrogram.push_back(m_vDendrograms[i]);
		i = m_indexComs[i].first;
	}
}

