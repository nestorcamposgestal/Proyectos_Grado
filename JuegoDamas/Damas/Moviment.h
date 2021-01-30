#pragma once
class Moviment
{
public:      
	void setXini(int Xini) { m_Xini = Xini; } 
	void setYini(int Yini) { m_Yini = Yini; }
	void setXfin(int Xfin) { m_Xfin = Xfin; }
	void setYfin(int Yfin) { m_Yfin = Yfin; }
	int getXini()const { return m_Xini; }
	int getYini()const { return m_Yini; }
	int getXfin()const { return m_Xfin; }
	int getYfin()const { return m_Yfin; }

private:
	int m_Xini;
	int m_Yini;
	int m_Xfin;
	int m_Yfin;
};

