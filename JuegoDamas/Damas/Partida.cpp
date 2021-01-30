#include "Partida.h"


Partida::Partida(int opcio) //Inicializamos todos los valores
{
	torn = 2;
	missatge = false;
	movValid = true;
	x = 0;
	y = 0;
	columnaorigen = 0;
	filaorigen = 0;
	columnadesti = 0;
	filadesti = 0;
	captura = false;
	nomFitxer = "data/moviments.txt";
	movimentsfets = 0;
	

}

void Partida::processaClick(int xMouse, int yMouse, bool &click) //Conjunto de funciones para procesar cuando hacemos click en pantalla
{
    if (click == false)
	{
			if (T.validaClick(xMouse, yMouse, filaorigen, columnaorigen) == true)
			{
				if (T.checkOrigenValid(filaorigen, columnaorigen, torn) == true)
				{
					click = true;
					missatge = false;
					T.marcaCasella(filaorigen, columnaorigen, torn);
				}
				else
				{
					missatge = true;
					click = false;
				}
			}
	}
	else 
	{
		if (click == true)
		{
			if (T.validaClick(xMouse, yMouse, filadesti, columnadesti) == true)
				click = false;
		}
	}
}

void Partida::processaClickMoviments(int Xini, int Yini, bool &click) //Conjunto de funciones para procesar los datos que nos llegan desde el fichero
{
	if (click == false)
	{
			if (T.checkOrigenValid(Xini, Yini, torn) == true)
			{
				click = true;
				missatge = false;
				T.marcaCasella(Xini, Yini, torn);
			}
			else
			{
				missatge = true;
				click = false;
			}
	}
	
	else
	{
		if (click == true)
		{
			click = false;
			/*
			if (T.validaClick(Xini, Yini, filadesti, columnadesti) == true)
				click = false;
			*/
		}
	}
}

bool Partida::ferMoviment(int opcio) //Hace el movimiento y si no es correcto muestra el mensaje de error
{
	if (T.processaMoviment(filaorigen, columnaorigen, filadesti, columnadesti, torn, captura) == true)
	{
		missatge = false;
		if (opcio == 2)
		{
			Moviment movimentaux;
			movimentaux.setXini(filaorigen);
			movimentaux.setYini(columnaorigen);
			movimentaux.setXfin(filadesti);
			movimentaux.setYfin(columnadesti);
			cua.afegeix(movimentaux);

		}
		return true;
	}
		
	else
	{
		missatge = true;
		return false;
	}
}

bool Partida::ferMovimentMoviments(int Xini, int Yini, int Xfin, int Yfin, int torn, bool captura) //Hace el movimiento del fitxero y si no es correcto muestra el mensaje de error
{
	if (T.processaMoviment(Xini, Yini, Xfin, Yfin, torn, captura) == true)
	{
		missatge = false;
		
		Moviment movimentaux;
		movimentaux.setXini(Xini);
		movimentaux.setYini(Yini);
		movimentaux.setXfin(Xfin);
		movimentaux.setYfin(Yfin);
		cua.afegeix(movimentaux);
		
		return true;
	}
	else
	{
		missatge = true;
		return false;
	}
}

void Partida::canviaTorn() //Cambia el turno
{ 
	if (torn == 2) 
		torn = 1; 
	else 
		torn = 2; 
}
bool Partida::comptadorPeces() //Mira si ja no queden peces per acabar el joc
{
	if (T.comptadorMatriu(torn) == 0)
	{
		return true;
		cout << T.comptadorMatriu(torn) << endl;
	}
		
	else
		return false;
}

void Partida::carregaMoviments(Screen &jocLib)//Carrega el fitxer de moviments
{
	ifstream file;
	file.open(nomFitxer);

	int NMoviments;
	int Xini, Yini, Xfin, Yfin;

	bool click = false;

	if (file.is_open())
	{
		file >> NMoviments;
		if (!file.eof())
		{
			MatriuMoviments = new Moviment[NMoviments];

			for (int i = 0; i <NMoviments; i++)
			{
				file >> Xini >> Yini >> Xfin >> Yfin;
				MatriuMoviments[i].setXini(Xini);
				MatriuMoviments[i].setYini(Yini);
				MatriuMoviments[i].setXfin(Xfin);
				MatriuMoviments[i].setYfin(Yfin);
				ferMoviments(i,click,jocLib);
				visualitza();
				jocLib.update();
			}

			delete MatriuMoviments;
		}
		file.close();
	}
}

void Partida::ferMoviments(int i, bool &click, Screen &jocLib)//Auxiliar de carregaMovimets
{
	
	if (!click)
	{
		processaClickMoviments(MatriuMoviments[i].getXini(), MatriuMoviments[i].getYini(), click);
		cout << "-----INI----" << MatriuMoviments[i].getXini()+1 << "----" << MatriuMoviments[i].getYini()+1 << endl;
		Sleep(1000);
	}


	visualitza();
	jocLib.update();


	if(click==true)
	{
		processaClickMoviments(MatriuMoviments[i].getXfin(), MatriuMoviments[i].getYfin(), click);
		Sleep(1000);
		if (ferMovimentMoviments(MatriuMoviments[i].getXini(), MatriuMoviments[i].getYini(), MatriuMoviments[i].getXfin(), MatriuMoviments[i].getYfin(),torn,captura) == true)
			canviaTorn();

		transformacioDama();

		cout << "-----Fin----" << MatriuMoviments[i].getXfin()+1 << "----" << MatriuMoviments[i].getYfin()+1 << endl;
		Sleep(1000);

	}
}

void Partida::EscriureMoviments()//Escrivim els moviments que hem anat fent si la opcio es 2
{
	ofstream file;
	file.open(nomFitxer);

	int tamany = cua.getMov();

	if (file.is_open())
	{
		file << tamany << endl;;
		for (int i = 0; i <tamany; i++)
		{
			file << cua.getPrimer().getXini() << " ";
			file << cua.getPrimer().getYini() << " ";
			file << cua.getPrimer().getXfin() << " ";
			file << cua.getPrimer().getYfin() << " " << endl;
			
			cua.treure();
		}

		file << endl;
		file.close();
	}
}
void Partida::BorrarMoviments() //Borrem el que hi ha al fitxer
{
	ofstream file;
	file.open(nomFitxer);

	if (file.is_open())
	{
		file << "" << endl;

		file.close();
	}

}

Partida::~Partida()
{
}
