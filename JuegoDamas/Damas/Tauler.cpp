#include "Tauler.h"


void Tauler::inicialitza()
{
	//Creamos sprites
	DamaBlanca.create("data/dama_blanca.png");
	DamaNegra.create("data/dama_negra.png");
	DamaBlancaSel.create("data/dama_blanca_sel.png");
	DamaNegraSel.create("data/dama_negra_sel.png");
    fons.create("data/tauler.png");
	FitxaBlanca.create("data/fitxa_blanca.png");
	FitxaNegra.create("data/fitxa_negra.png");
	AvisError.create("data/avis_error.png");
	FitxaBlancaSel.create("data/fitxa_blanca_sel.png");
	FitxaNegraSel.create("data/fitxa_negra_sel.png");
	TornBlanc.create("data/torn_blanc.png");
	TornNegra.create("data/torn_negre.png");


	//Contamos fitxes
	fitxesBlanques = 0;
	fitxesNegres = 0;

	//Nombre Matriz Fichero
	nomFitxer = "data/matriu.txt";
	file = "data/moviments.txt";
	FitxerMatriu();
	

	for (int i = 0; i < 8; i++)
	{
		for (int a = 0; a < 8; a++)
		{
			if (MatriuDames[i][a] == 1)
				fitxesNegres++;
			if (MatriuDames[i][a] == 2)
				fitxesBlanques++;
			if (MatriuDames[i][a] == 111)
				fitxesNegres++;
			if (MatriuDames[i][a] == 222)
				fitxesBlanques++;
		}
	}

}
void Tauler::dibuixa(int torn, bool missatge)//Dibujamos todos los sprites en pantalla segun los valores
{
	fons.draw(0, 0);
	for (int i = 0; i < 8; i++)
	{
		for (int a = 0; a < 8; a++)
		{
			if (MatriuDames[i][a] == 1)
				FitxaNegra.draw(((a * MIDACASELLA)), ((INIPANTALLAY)+(i* MIDACASELLA)));
			else
			{
				if (MatriuDames[i][a] == 2)
					FitxaBlanca.draw(((a * MIDACASELLA)), ((INIPANTALLAY)+(i* MIDACASELLA)));
				else
				{
					if (MatriuDames[i][a] == 11)
						FitxaNegraSel.draw(((a * MIDACASELLA)), ((INIPANTALLAY)+(i* MIDACASELLA)));
					else
					{
						if (MatriuDames[i][a] == 22)
							FitxaBlancaSel.draw(((a * MIDACASELLA)), ((INIPANTALLAY)+(i* MIDACASELLA)));
						else
						{
							if (MatriuDames[i][a] == 111)
								DamaNegra.draw(((a * MIDACASELLA)), ((INIPANTALLAY)+(i* MIDACASELLA)));
							else
							{
								if (MatriuDames[i][a] == 222)
									DamaBlanca.draw(((a * MIDACASELLA)), ((INIPANTALLAY)+(i* MIDACASELLA)));
								else
								{
									if (MatriuDames[i][a] == 1111)
										DamaNegraSel.draw(((a * MIDACASELLA)), ((INIPANTALLAY)+(i* MIDACASELLA)));
									else
									{
										if (MatriuDames[i][a] == 2222)
											DamaBlancaSel.draw(((a * MIDACASELLA)), ((INIPANTALLAY)+(i* MIDACASELLA)));
									}
								}

							}
						}
					}

				}
			}
		}
	}
	// Dibujamos color de turno
	if (torn == 1)
		TornNegra.draw(POSICIO_TORN_X, POSICIO_TORN_Y);
	if (torn == 2)
		TornBlanc.draw(POSICIO_TORN_X, POSICIO_TORN_Y);
	// Dibujamos mensaje
	if (missatge == true)
	{
		AvisError.draw(POSICIO_AVIS_X, POSICIO_AVIS_Y);
		
	}

		



}
bool Tauler::validaClick(int x, int y, int &fila, int &columna) //A partir de la "x" y la "y" encontramos la fila y columna que les corresponde
{
	if ((x >= INIPANTALLAX) && (y >= INIPANTALLAY))
		{
			columna = (x - INIPANTALLAX) / MIDACASELLA;
			fila = (y - INIPANTALLAY) / MIDACASELLA;
			return true;
		}
	else
		{
			columna = 0;
			fila = 0;
			return false;
		}
}
bool Tauler::processaMoviment(int filaorigen, int columnaorigen, int filadesti, int columnadesti, int torn, bool &captura) //Hace el movimiento de las piezas
{ 
	//TURNO 1
	if (torn == 1)
	{
		//SI LA POSICION ESTA SELECCIONADA (Y ES FITXA)
		if ((MatriuDames[filaorigen][columnaorigen]) == 11)
		{
			//MOVIMIENTO NORMAL
			if ((filadesti == filaorigen + 1) && ((columnadesti == columnaorigen + 1) || (columnadesti == columnaorigen - 1)))
			{
				if (MatriuDames[filadesti][columnadesti] == 0)
				{
					MatriuDames[filaorigen][columnaorigen] = 0;
					MatriuDames[filadesti][columnadesti] = 1;
					return true;
				}
				else
					MatriuDames[filaorigen][columnaorigen] = torn;

			}
			//CAPTURA DE PIEZA (Y ES FITXA)
			else
			{
				if ((filadesti == filaorigen + 2) && ((columnadesti == columnaorigen + 2) || (columnadesti == columnaorigen - 2)))
				{
					if (MatriuDames[filadesti][columnadesti] == 0)
					{
						if ((MatriuDames[(filadesti + filaorigen) / 2][(columnadesti + columnaorigen) / 2] == 2)|| (MatriuDames[(filadesti + filaorigen) / 2][(columnadesti + columnaorigen) / 2] == 222))
						{
							MatriuDames[(filadesti + filaorigen) / 2][(columnadesti + columnaorigen) / 2] = 0;
							MatriuDames[filaorigen][columnaorigen] = 0;
							MatriuDames[filadesti][columnadesti] = 1;
							captura = true;
							return true;
						}
						else
							MatriuDames[filaorigen][columnaorigen] = torn;
					}
					else
						MatriuDames[filaorigen][columnaorigen] = torn;
				}
				else
					MatriuDames[filaorigen][columnaorigen] = torn;
			}
		}

		//SI LA POSICION ESTA SELECCIONADA (Y ES DAMA)
		if ((MatriuDames[filaorigen][columnaorigen]) == 1111)
		{
			//MOVIMIENTO NORMAL
			if (mouDiagonal(filaorigen,columnaorigen,filadesti,columnadesti)==true)
			{
				if (MatriuDames[filadesti][columnadesti] == 0)
				{
					if (mirarCami(filaorigen, columnaorigen, filadesti, columnadesti) == false)
					{
						MatriuDames[filaorigen][columnaorigen] = 0;
						MatriuDames[filadesti][columnadesti] = 111;
						return true;
					}
					else
					{
						//CAPTURA DAMA NEGRA
						if (capturaDamaNegra(filaorigen, columnaorigen, filadesti, columnadesti) == true)
						{
							if (mirarCamiDamaCaptura(filaorigen, columnaorigen, filadesti, columnadesti) == true)
							{
								MatriuDames[filaorigen][columnaorigen] = 0;
								MatriuDames[filadesti][columnadesti] = 111;
								return true;
							}
							else
								MatriuDames[filaorigen][columnaorigen] = 111;
						}
						else
							MatriuDames[filaorigen][columnaorigen] = 111;
					}
				}
				else
					MatriuDames[filaorigen][columnaorigen] = 111;

			}
			else
				MatriuDames[filaorigen][columnaorigen] = 111;
			
		}

	
	
	}
	//TURNO 2
	if (torn == 2)
	{
		//SI LA POSICION ESTA SELECCIONADA
		if ((MatriuDames[filaorigen][columnaorigen]) == 22)
		{
			//MOVIMIENTO NORMAL
			if ((filadesti == filaorigen - 1) && ((columnadesti == columnaorigen + 1) || (columnadesti == columnaorigen - 1)))
			{
				if (MatriuDames[filadesti][columnadesti] == 0)
				{
					MatriuDames[filaorigen][columnaorigen] = 0;
					MatriuDames[filadesti][columnadesti] = 2;
					return true;
				}
				else
					MatriuDames[filaorigen][columnaorigen] = torn;
			}
			//CAPTURA DE PIEZA
			else
			{
				if ((filadesti == filaorigen - 2) && ((columnadesti == columnaorigen + 2) || (columnadesti == columnaorigen - 2)))
				{
					if (MatriuDames[filadesti][columnadesti] == 0)
					{
						if ((MatriuDames[(filadesti + filaorigen) / 2][(columnadesti + columnaorigen) / 2] == 1) || (MatriuDames[(filadesti + filaorigen) / 2][(columnadesti + columnaorigen) / 2] == 111))
						{
							MatriuDames[(filadesti + filaorigen) / 2][(columnadesti + columnaorigen) / 2] = 0;
							MatriuDames[filaorigen][columnaorigen] = 0;
							MatriuDames[filadesti][columnadesti] = 2;
							captura = true;
							return true;
						}
						else
							MatriuDames[filaorigen][columnaorigen] = torn;
					}
					else
						MatriuDames[filaorigen][columnaorigen] = torn;
				}
				else
					MatriuDames[filaorigen][columnaorigen] = torn;
			}
		}
		//SI LA POSICION ESTA SELECCIONADA (Y ES DAMA)
		if ((MatriuDames[filaorigen][columnaorigen]) == 2222)
		{
			//MOVIMIENTO NORMAL
			if (mouDiagonal(filaorigen, columnaorigen, filadesti, columnadesti) == true)
			{
				if (MatriuDames[filadesti][columnadesti] == 0)
				{
					if (mirarCami(filaorigen, columnaorigen, filadesti, columnadesti) == false)
					{
						MatriuDames[filaorigen][columnaorigen] = 0;
						MatriuDames[filadesti][columnadesti] = 222;
						return true;
					}
					else
					{
						//CAPTURA DAMA BLANCA
						if (capturaDamaBlanca(filaorigen, columnaorigen, filadesti, columnadesti) == true)
						{
							if (mirarCamiDamaCaptura(filaorigen, columnaorigen, filadesti, columnadesti) == true)
							{
								MatriuDames[filaorigen][columnaorigen] = 0;
								MatriuDames[filadesti][columnadesti] = 222;
								return true;
							}
							else
								MatriuDames[filaorigen][columnaorigen] = 222;
						}
						else
							MatriuDames[filaorigen][columnaorigen] = 222;
					}
				}
				else
					MatriuDames[filaorigen][columnaorigen] = 222;

			}
			else
				MatriuDames[filaorigen][columnaorigen] = 222;
			
		}
	}
	return false;
	
}
void Tauler::marcaCasella(int fila, int columna, int torn) //Pintamos la casella cuando esta seleccionada
{
	if ((torn == 1)&&(MatriuDames[fila][columna]==1))
	{
		MatriuDames[fila][columna] = 11;
	}
	if ((torn == 2)&&(MatriuDames[fila][columna]==2))
	{
		MatriuDames[fila][columna] = 22;
	}
	if ((torn == 1) && (MatriuDames[fila][columna] == 111))
	{
		MatriuDames[fila][columna] = 1111;
	}
	if ((torn == 2) && (MatriuDames[fila][columna] == 222))
	{
		MatriuDames[fila][columna] = 2222;
	}
}
bool Tauler::checkOrigenValid(int filaOrigen, int columnaOrigen, int torn) //Miramos si el turno coincide con el color de la casilla
{
	if ((torn == 1) && ((MatriuDames[filaOrigen][columnaOrigen] == 1)||(MatriuDames[filaOrigen][columnaOrigen] == 111)))
		return true;
	if ((torn == 2) && ((MatriuDames[filaOrigen][columnaOrigen] == 2)|| (MatriuDames[filaOrigen][columnaOrigen] == 222)))
		return true;

	return false;
}

int Tauler::comptadorMatriu(int torn) //Contamos el numero de fitxes en el tablero y retornamos segun el turno
{
	fitxesNegres = 0;
	fitxesBlanques = 0;
	for (int i = 0; i < 8; i++)
	{
		for (int a = 0; a < 8; a++)
		{
			if ((MatriuDames[i][a] == 1)||(MatriuDames[i][a]==11)|| (MatriuDames[i][a] == 111)|| (MatriuDames[i][a] == 1111))
				fitxesNegres++;
			if ((MatriuDames[i][a] == 2)||(MatriuDames[i][a] == 22)|| (MatriuDames[i][a] == 222)|| (MatriuDames[i][a] == 2222))
				fitxesBlanques++;
		}
	}
	if (torn == 1)
		return fitxesNegres;
	if (torn == 2)
		return fitxesBlanques;
}

bool Tauler::transformarDama(int torn)
{
	if (torn == 1)
	{
		for (int i = 0; i < 8; i++)
		{
			if (MatriuDames[0][i] == 2)
			{
				MatriuDames[0][i] = 222;
				return true;
			}
				
		}
	}
	if (torn == 2)
	{
		for (int i = 0; i < 8; i++)
		{
			if (MatriuDames[7][i] == 1)
			{
				MatriuDames[7][i] = 111;
				return true;
			}
		}
	}
	return false;
}

bool Tauler::mirarCami(int filaorigen, int columnaorigen, int filadesti, int columnadesti) //Mira que el cami que ha de recorrer sigui l'adequat per poder dur a terme el moviment
{
	if ((filadesti>filaorigen) && (columnadesti>columnaorigen)) //Abajo derecha
	{
		for (int i = filaorigen; i < filadesti; i++)
		{
			for (int a = columnaorigen; a < columnadesti; a++)
			{
				if ((filadesti - i) == (columnadesti - a))
				{
					if ((MatriuDames[i][a] == 1) || (MatriuDames[i][a] == 111) || (MatriuDames[i][a] == 2) || (MatriuDames[i][a] == 222))
						return true;
				}
			}
		}
	}
	if ((filadesti>filaorigen) && (columnadesti<columnaorigen))//Abajo izquierda
	{
		for (int i = filaorigen; i < filadesti; i++)
		{
			for (int a = columnaorigen; a > columnadesti; a--)
			{
				if ((filadesti - i) == (a-columnadesti))
				{
					if ((MatriuDames[i][a] == 1) || (MatriuDames[i][a] == 111) || (MatriuDames[i][a] == 2) || (MatriuDames[i][a] == 222))
						return true;
				}
			}
		}
	}
	if ((filadesti<filaorigen) && (columnadesti>columnaorigen))//Arriba derecha
	{
		for (int i = filaorigen; i > filadesti; i--)
		{
			for (int a = columnaorigen; a < columnadesti; a++)
			{
				if ((i-filadesti) == (columnadesti-a))
				{
					if ((MatriuDames[i][a] == 1) || (MatriuDames[i][a] == 111) || (MatriuDames[i][a] == 2) || (MatriuDames[i][a] == 222))
						return true;
				}
			}
		}
	}
	if ((filadesti<filaorigen) && (columnadesti<columnaorigen))//Arriba izquierda
	{
		for (int i = filaorigen; i > filadesti; i--)
		{
			for (int a = columnaorigen; a > columnadesti; a--)
			{
				if ((i-filadesti) == (a - columnadesti))
				{
					if ((MatriuDames[i][a] == 1) || (MatriuDames[i][a] == 111) || (MatriuDames[i][a] == 2) || (MatriuDames[i][a] == 222))
						return true;
				}
			}
		}
	}
	
	return false;
}

bool Tauler::mouDiagonal(int filaorigen, int columnaorigen, int filadesti, int columnadesti)
{
	if ((filadesti>filaorigen) && (columnadesti>columnaorigen))//Abajo derecha
	{
		if ((filadesti - filaorigen) == (columnadesti - columnaorigen))
			return true;
	}
	if ((filadesti>filaorigen) && (columnadesti<columnaorigen))//Abajo izquierda
	{
		if ((filadesti - filaorigen) == (columnaorigen-columnadesti))
			return true;
	}
	if ((filadesti<filaorigen) && (columnadesti>columnaorigen))//Arriba derecha
	{
		if ((filaorigen-filadesti) == (columnadesti - columnaorigen))
			return true;
	}
	if ((filadesti<filaorigen) && (columnadesti<columnaorigen))//Arriba izquierda
	{
		if ((filaorigen-filadesti) == (columnaorigen-columnadesti))
			return true;
	}
	
	return false;
}

bool Tauler::capturaDamaNegra(int filaorigen, int columnaorigen, int filadesti, int columnadesti)
{
	if ((filadesti>filaorigen) && (columnadesti>columnaorigen)) //Abajo derecha
	{
		if ((MatriuDames[filadesti - 1][columnadesti - 1] == 2) || (MatriuDames[filadesti - 1][columnadesti - 1] == 222))
			return true;
	}
	if ((filadesti>filaorigen) && (columnadesti<columnaorigen))//Abajo izquierda
	{
		if ((MatriuDames[filadesti - 1][columnadesti + 1] == 2) || (MatriuDames[filadesti - 1][columnadesti + 1] == 222))
			return true;
	}
	if ((filadesti<filaorigen) && (columnadesti>columnaorigen))//Arriba derecha
	{
		if ((MatriuDames[filadesti + 1][columnadesti - 1] == 2) || (MatriuDames[filadesti + 1][columnadesti - 1] == 222))
			return true;
	}
	if ((filadesti<filaorigen) && (columnadesti<columnaorigen))//Arriba izquierda
	{
		if ((MatriuDames[filadesti + 1][columnadesti + 1] == 2) || (MatriuDames[filadesti + 1][columnadesti + 1] == 222))
			return true;
	}

	return false;
}

bool Tauler::capturaDamaBlanca(int filaorigen, int columnaorigen, int filadesti, int columnadesti)
{
	if ((filadesti>filaorigen) && (columnadesti>columnaorigen)) //Abajo derecha
	{
		if ((MatriuDames[filadesti - 1][columnadesti - 1] == 1) || (MatriuDames[filadesti - 1][columnadesti - 1] == 111))
			return true;
	}
	if ((filadesti>filaorigen) && (columnadesti<columnaorigen))//Abajo izquierda
	{
		if ((MatriuDames[filadesti - 1][columnadesti + 1] == 1) || (MatriuDames[filadesti - 1][columnadesti + 1] == 111))
			return true;
	}
	if ((filadesti<filaorigen) && (columnadesti>columnaorigen))//Arriba derecha
	{
		if ((MatriuDames[filadesti + 1][columnadesti - 1] == 1) || (MatriuDames[filadesti + 1][columnadesti - 1] == 111))
			return true;
	}
	if ((filadesti<filaorigen) && (columnadesti<columnaorigen))//Arriba izquierda
	{
		if ((MatriuDames[filadesti + 1][columnadesti + 1] == 1) || (MatriuDames[filadesti + 1][columnadesti + 1] == 111))
			return true;
	}

	return false;
}
bool Tauler::mirarCamiDamaCaptura(int filaorigen, int columnaorigen, int filadesti, int columnadesti)
{
	if ((filadesti>filaorigen) && (columnadesti>columnaorigen)) //Abajo derecha
	{
		filadesti = filadesti - 1;
		columnadesti = columnadesti - 1;

		if (filadesti - 1 == filaorigen)
		{
			MatriuDames[filadesti][columnadesti] = 0;
			return true;
		}
			

		for (int i = filaorigen; i < filadesti; i++)
		{
			for (int a = columnaorigen; a < columnadesti; a++)
			{
				if ((filadesti - i) == (columnadesti - a))
				{
					if (MatriuDames[i][a] == 0)
					{
						MatriuDames[filadesti][columnadesti] = 0;
						return true;
					}
				}
			}
		}
	}
	if ((filadesti>filaorigen) && (columnadesti<columnaorigen))//Abajo izquierda
	{
		filadesti = filadesti - 1;
		columnadesti = columnadesti + 1;

		if (filadesti - 1 == filaorigen)
		{
			MatriuDames[filadesti][columnadesti] = 0;
			return true;
		}

		for (int i = filaorigen; i < filadesti; i++)
		{
			for (int a = columnaorigen; a > columnadesti; a--)
			{
				if ((filadesti - i) == (a - columnadesti))
				{
					if (MatriuDames[i][a] == 0)
					{
						MatriuDames[filadesti][columnadesti] = 0;
						return true;
					}
				}
			}
		}
	}
	if ((filadesti<filaorigen) && (columnadesti>columnaorigen))//Arriba derecha
	{
		filadesti = filadesti + 1;
		columnadesti = columnadesti - 1;

		if (filadesti + 1 == filaorigen)
		{
			MatriuDames[filadesti][columnadesti] = 0;
			return true;
		}

		for (int i = filaorigen; i > filadesti; i--)
		{
			for (int a = columnaorigen; a < columnadesti; a++)
			{
				if ((i - filadesti) == (columnadesti - a))
				{
					if (MatriuDames[i][a] == 0)
					{
						MatriuDames[filadesti][columnadesti] = 0;
						return true;
					}
				}
			}
		}
	}
	if ((filadesti<filaorigen) && (columnadesti<columnaorigen))//Arriba izquierda
	{
		filadesti = filadesti + 1;
		columnadesti = columnadesti + 1;

		if (filadesti + 1 == filaorigen)
		{
			MatriuDames[filadesti][columnadesti] = 0;
			return true;
		}

		for (int i = filaorigen; i > filadesti; i--)
		{
			for (int a = columnaorigen; a > columnadesti; a--)
			{
				if ((i - filadesti) == (a - columnadesti))
				{
					if (MatriuDames[i][a] == 0)
					{
						MatriuDames[filadesti][columnadesti] = 0;
						return true;
					}
						
				}
			}
		}
	}

	return false;
}

void Tauler::FitxerMatriu() //Llegeix el fitxer de posicions inicials
{
	ifstream fitxer;
	fitxer.open(nomFitxer);
	int i = 0; int j = 0;
	if (fitxer.is_open())
	{
		int valor;
		fitxer >> MatriuDames[i][j];
		while (!fitxer.eof())
		{
			while (!fitxer.eof())
			{
				j++;
				fitxer >> MatriuDames[i][j];
			}
			i++;
		}
		fitxer.close();
	}
}

