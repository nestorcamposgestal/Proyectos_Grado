 #include "MatriuSparse.h"


MatriuSparse::MatriuSparse()
{
	m_matriu.resize(0);
	//m_Nlongitud = 0;

	
}

MatriuSparse::MatriuSparse(const int & files, const int & columnes)
{
	//int MIDA = files * columnes;
	//m_columnes.resize(MIDA);
	//m_files.resize(MIDA);
	//m_valors.resize(MIDA,-1); 
	m_matriu.resize(files);
	//m_Nlongitud = files;
	
}

MatriuSparse::MatriuSparse(const MatriuSparse & m)
{
	m_matriu = m.m_matriu;
	m_Nlongitud = m.m_Nlongitud;
}

MatriuSparse::MatriuSparse(string nomFitxer)
{
	ifstream fitxer;
	fitxer.open(nomFitxer);
	if (fitxer.is_open())
	{
		int fila, columna;
		int valorMAX=m_Nlongitud;
		fitxer >> fila >> columna;

		while (!fitxer.eof())
		{
			if (fila > valorMAX)
				valorMAX = fila;
			if (columna > valorMAX)
				valorMAX = columna;

			Triplet t;
			t.filas = fila;
			t.columnas = columna;
			t.valores = 1;
			m_matriu.push_back(t);
			fitxer >> fila >> columna;
		}
		fitxer.close();
		m_Nlongitud = valorMAX;
	}
}

MatriuSparse::~MatriuSparse()
{
	m_matriu.clear();
}

void MatriuSparse::init(const int files, const int columnes)
{
	/*int numC = 0;
	int numF = 0;
	int valorF = 0;
	int MIDA = files * columnes;
	m_columnes.resize(MIDA);
	m_files.resize(MIDA);
	m_valors.resize(MIDA,-1);

	for (int i = 0; i < MIDA; i++)
	{
		if(numF==files)
		{
			numF = 0;
			valorF++;
		}
		if (numC == columnes)
			numC = 0;
		m_columnes[i] = numC;
		m_files[i] = valorF;
		numC++;
		numF++;
	}*/
	m_matriu.clear();
	m_Nlongitud = files;
}

bool MatriuSparse::getVal(const int fila, const int col, float & valor)
{
	int MIDA = fila * col;
	bool trobatfila = false;
	bool trobatcol = false;
	bool trobatvalor = false;
	bool para = false;
	int i = 0;
	int index_ini = 0;
	int index_fin=0;
	int filaencontrada = 0;
	if((fila>m_Nlongitud-1)||(col>m_Nlongitud-1))
	{
		return false;
	}
	else
	{
		while ((i < m_matriu.size()) && (!para))
		{
			if ((m_matriu[i].filas == fila) && (!trobatfila))
			{
				index_ini = i;
				trobatfila = true;
				filaencontrada = m_matriu[i].filas;
			}
			if ((trobatfila == true) && ((m_matriu[i].filas != fila) || (i == m_matriu.size() - 1)))
			{
				index_fin = i;
				para = true;
			}
			i++;
		}
		i = index_ini;
		while ((i <= index_fin) && (!trobatcol) && (m_matriu[i].filas==filaencontrada)&&(trobatfila))//mirar para implementar cerca binaria en la busqueda de la columna
		{
			if (m_matriu[i].columnas == col)
			{
				trobatcol = true;
				valor = m_matriu[i].valores;
			}
			else
				i++;
		}

		if (trobatcol == false)
			valor = 0;


		return true;
	
	}

}

void MatriuSparse::setVal(const int fila, const int col, float valor)
{
	if (valor != 0)
	{
		vector<Triplet>::iterator it = m_matriu.begin();
		bool trobat = false;
		bool cambiovalor = false;
		Triplet t;
		t.filas = fila;
		t.columnas = col;
		t.valores = valor;

		while (it != m_matriu.end() && trobat != true)
		{
			if (((*it).filas) == fila && ((*it).columnas == col))
			{
				(*it).valores = valor;
				trobat = true;
				cambiovalor = true;
			}
			else
			{
				if (((*it).filas == fila && (*it).columnas > col) || (*it).filas > fila)
				{
					trobat = true;
				}
				else
				{
					it++;
				}
			}

		}
		if (cambiovalor == false)
		{
			if (trobat == true)
			{

				m_matriu.insert(it, t);
			}
			else
			{
				m_matriu.push_back(t);
			}
		}
		if (col + 1 > m_Nlongitud)
		{
			m_Nlongitud = col + 1;
		}

		if (fila + 1 > m_Nlongitud)
		{
			m_Nlongitud = fila + 1;
		}

	}

}

int MatriuSparse::getNodes() const
{
	/*int mida = 1;
	int numero = m_matriu[0].filas;

	for (int i = 0; i < m_matriu.size(); i++)
	{
		if (m_matriu[i].filas != numero)
		{
			numero = m_matriu[i].filas;
			mida++;
		}


	}

	return mida;*/

	return (m_matriu.back().filas+1);
}

int MatriuSparse::getNValues() const
{
	return m_matriu.size();
}

void MatriuSparse::calculaGrau(vector<int>& graus) const
{
	graus.resize(getNodes());
	int numero = 0;
	int grado = 0;

	numero = m_matriu[0].filas;


	for (int i=0; i<m_matriu.size();i++)
	{
		
		if (m_matriu[i].filas != numero || i==m_matriu.size())
		{
			graus[numero] = grado;//En caso de fallo, quitar resize y añadir con emplace back
			//numero++;
			numero = m_matriu[i].filas;
			grado = 0;
		}
		grado++;
	}
	graus[numero] = grado;
/*
	int grado = 0;
	int numero = m_matriu[0].filas;

	for (int i = 0; i < m_matriu.size(); i++)
	{
		if (numero != m_matriu[i].filas)
		{
			graus.emplace_back(grado);
			numero = m_matriu[i].filas;
			grado = 0;
		}
		grado++;
	}
	graus.emplace_back(grado);
	*/

}

void MatriuSparse::creaMaps(vector<map<pair<int, int>, double>>& vMaps) const
{
	vMaps.resize(getNodes());

	vector<map<pair<int, int>, double>> ::iterator it;

	int numero = 0;
	double valor = 0;
	pair<int, int> pair;


	numero = m_matriu[0].filas;
	for (int i = 0; i < m_matriu.size(); i++)
	{
		pair.first = m_matriu[i].filas;
		pair.second = m_matriu[i].columnas;

		if (m_matriu[i].filas != numero)
		{
			numero = m_matriu[i].filas;
			//numero++;
		}

		vMaps[numero].insert({ pair, valor });
	}


	/*if (vMaps.front().empty())
		vMaps.erase(vMaps.begin());*/

	for (it=vMaps.begin(); it !=vMaps.end(); it++)
	{
		if ((*it).empty())
		{
			(*it).insert({ {0,-1}, double(-2) });
		}
	}



}

void MatriuSparse::calculaDendrograms(vector<Tree<double>*>& vDendograms) const
{

	double valor = 0;
	vDendograms.resize(getNodes());

	for (int i = 0; i < vDendograms.size(); i++)
	{	
		valor = i;
		Tree<double>* Arbol = new Tree<double>(valor);
		vDendograms[i]=Arbol;
	}
}

void MatriuSparse::clear()
{
	m_matriu.clear();
	m_Nlongitud = 0;

}

MatriuSparse MatriuSparse::operator=(MatriuSparse & m)
{
	m_matriu = m.m_matriu;
	m.m_matriu.clear();
	return *this;
}

MatriuSparse MatriuSparse::operator*(float num)
{
	MatriuSparse mF;
	mF.m_matriu = m_matriu;
	for (int i = 0; i < m_matriu.size(); i++)
	{
		mF.m_matriu[i].valores = mF.m_matriu[i].valores*num;
	}

	return mF;
}

vector<float> MatriuSparse::operator*(vector<float>& v)
{


	vector<float> vF;
	vF.resize(v.size());
	int z = 0;
	bool error = false;

	if (v.size() <= 0) {
		return vF;
	}

	else {
		for (int i = 0; i < vF.size(); i++) {
			float aux = 0;

			while (m_matriu[z].filas == i && error != true) {
				aux = aux + (v[i] * m_matriu[z].valores);
				z++;
				if (z == m_matriu.size())
				{
					error = true; z = 0;
				}
			}

			if (aux != 0)
				vF[i] = aux;
		}

	}
	return vF;


	/*
	vector <float> vF;
	vF.resize(v.size());
	vector<Triplet> ::iterator it = m_matriu.begin();

	int res = 0;
	if (vF.size() <= 0)
	{
		return vF;
	}
	else
	{
		while (it != m_matriu.end())
		{
			res += (*it).valores*v[(*it).columnas];
			if (((*(it + 1)).filas) != ((*it).filas))
			{
				vF[(*it).filas] = res;
				res = 0;
			}
			it++;
		}
		return vF;
	}
	//|| ((*it).filas == m_Nlongitud)
	*/
}

MatriuSparse MatriuSparse::operator/(float num)
{
	MatriuSparse mF;
	mF.m_matriu = m_matriu;
	if (num > 0)
	{
		for (int i = 0; i < m_matriu.size(); i++)
		{
			mF.m_matriu[i].valores = (m_matriu[i].valores / num);
		}

		return mF;
	}
}

ostream & operator<<(ostream & output, const MatriuSparse & m)
{

	int valorFilas = m.getNColumnes();
	int valorColumnas = m.getNFiles();

	if (valorFilas > valorColumnas) {
		output << "MATRIU DE (FILES: " << valorFilas << "  COLUMNES: " << valorFilas << " )" << endl;
	}

	else {
		output << "MATRIU DE (FILES: " << valorColumnas << "  COLUMNES: " << valorColumnas << " )" << endl;
	}

	output << "VALORS (FILA::COL::VALOR)" << endl;

	for (int i = 0; i < m.m_matriu.size(); i++) {
		output << "( " << m.m_matriu[i].filas << " :: " << m.m_matriu[i].columnas << " :: " << m.m_matriu[i].valores << " ) " << endl;
	}

	return output;

	/*
	output << "MATRIU DE (FILES: " << m.m_Nlongitud+1 << " COLUMNES: " << m.m_Nlongitud+1 << " )" << endl;
	output << "VALORS (FILA::COL::VALOR)" << endl;
	for (int i = 0; i < m.m_matriu.size(); i++)
	{
		if (m.m_matriu[i].valores > 0)
			output << "( " << m.m_matriu[i].filas << " :: " << m.m_matriu[i].columnas << " :: " << m.m_matriu[i].valores << " ) " << endl;
	}
	output << endl;
	return output;
	*/
}

int MatriuSparse::getNFiles() const {

	int masGrande = 0;
	int indice = 0;

	while (indice < m_matriu.size()) {

		if ((m_matriu[indice].filas > masGrande))
			masGrande = m_matriu[indice].filas;
		indice++;
	}

	return masGrande + 1;
}

int MatriuSparse::getNColumnes() const {


	int masGrande = 0;
	int indice = 0;

	while (indice < m_matriu.size()) {

		if ((m_matriu[indice].columnas > masGrande))
			masGrande = m_matriu[indice].columnas;
		indice++;
	}

	return masGrande + 1;
}
