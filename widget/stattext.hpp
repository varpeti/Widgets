#ifndef _STATTEXT_
#define _STATTEXT_

#include "ablak.hpp"

#define TEXT_RAHAGYAS 2


using namespace genv;

class STATTEXT : public ABLAK
{
	protected:

	string szoveg;
	SZIN szin2;
	unsigned int rahagyas;

	public:

	STATTEXT(double x, double y, SZIN szin, SZIN szin2, string szoveg, const int rahagyas=TEXT_RAHAGYAS)
		: ABLAK(x,y,gout.twidth(szoveg)+rahagyas*2.5,gout.cascent()+rahagyas*4,szin), szin2(szin2), szoveg(szoveg), rahagyas(rahagyas)
	{}	//                                A gout fontja alapján nézi a méretet, de egy idéglenes canvas szerint rajzol: Nem kell más font, jó a kép is.

	STATTEXT(double x, double y, double sx, double sy, double kx, double ky, string szoveg="") // Képként hozza létre, a stringben tárolt érték csak tárolva van.
		: ABLAK(x,y,sx,sy,kx,ky), szoveg(szoveg), rahagyas(0)
	{}

	virtual void rajzol(canvas &rajz, canvas &Tkepek, double XX, double YY, bool focus) const;
	virtual bool update(event ev, double X0, double Y0, KAMERA kamera) {}; // nem updatelődik
	virtual void addObj(OBJ *obj) {}; // Nem lehet hozzáadni újabb objektumokat.
	virtual void delObj(OBJ *obj) {}; // Nem lehet törölni objektumokat.
	virtual void ss(stringstream &s);
};

void STATTEXT::rajzol(canvas &rajz, canvas &Tkepek, double XX, double YY, bool focus) const
{
	rajz.open(sx,sy);
	rajz.transparent(true);

	if (rahagyas>0){
		rajz << color(szin.rr,szin.gg,szin.bb) << move_to(0,0) << box(sx,sy);
		rajz << color(szin2.rr,szin2.gg,szin2.bb) << move_to(rahagyas,rahagyas+gout.cascent()) << text(szoveg);
	}else{
		rajz << stamp(Tkepek,kx,ky,sx,sy,0,0); // Kép
	}
}

void STATTEXT::ss(stringstream &s)
{
	int i=0;
	s >> i;
	if (i==1) // Színcsere
	{
		SZIN seged = szin;
		szin = szin2;
		szin2 = seged;
	}
	else if (i==2) // Szöveg csere
	{
		getline(s,szoveg);		
		sx = gout.twidth(szoveg)+rahagyas*2.5;
	}
	s.clear(); s.str(string());
	s << szoveg; // Visszatér a szöveggel
}

#endif