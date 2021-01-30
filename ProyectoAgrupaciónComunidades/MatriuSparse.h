#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <math.h>
#include <algorithm>
#include <map>
#include "Tree.hpp"
using namespace std;

struct Triplet
{
	int filas, columnas;
	float valores;
};

class MatriuSparse
{
public:
	//METODO SIN AnADIR 0
	//constructors i destructors
	MatriuSparse();
	MatriuSparse(const int& files, const int& columnes);
	MatriuSparse(const MatriuSparse& m);
	MatriuSparse(string nomFitxer);
	~MatriuSparse();

	//metodes
	void init(const int files, const int columnes);
	int getNFiles () const;
	int getNColumnes () const;
	bool getVal(const int fila, const int col, float&valor);
	void setVal(const int fila, const int col, float valor);
	int getNodes() const;
	int getPrimerElement() const { return m_matriu.front().filas; }
	
	//Nuevas Declaraciones 2a Parte Proyecto
	int getNValues() const;
	void calculaGrau(vector<int>& graus) const;
	void creaMaps(vector<map<pair<int, int>, double>>& vMaps) const;
	void calculaDendrograms(vector<Tree<double>*>& vDendograms) const;
	void clear();



	//operadors
	MatriuSparse operator =(MatriuSparse& m);
	MatriuSparse operator *(float num);
	vector<float> operator *(vector<float>& v);
	MatriuSparse operator /(float num);
	
	friend ostream &operator<< (ostream &output, const MatriuSparse &m);

private:
	vector <Triplet> m_matriu;
	int m_Nlongitud;
};