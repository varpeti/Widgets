#ifndef _ABLAK_	
#define _ABLAK_

#define ABLAK_RAHAGYAS 5

#include "szin.hpp"
#include "env.hpp"

using namespace genv;

class ABLAK : public OBJ
{
	protected:

	SZIN szin; //szín
	unsigned int kx,ky; // kép x,y

	public:

	ABLAK(double x, double y, double sx, double sy, unsigned int kx, unsigned int ky)
		:  OBJ(x,y,sx,sy), kx(kx), ky(ky)
	{
		szin = SZIN(0,0,0);
	}
	ABLAK(double x, double y, double sx, double sy, SZIN szin)
		: OBJ(x,y,sx,sy), szin(szin)
	{
		kx=56789; // Az 56789 az új -1! unsigned és ekkora képet nem tudna beolvasni.
		ky=56789;
	}

	virtual void rajzol(canvas &rajz, canvas &Tkepek, double XX, double YY, bool focus) const;
	virtual bool update(event ev, double X0, double Y0, KAMERA kamera);
};

void ABLAK::rajzol(canvas &rajz, canvas &Tkepek, double XX, double YY, bool focus) const
{
	rajz.open(sx,sy);
	rajz.transparent(true);

	if (kx==56789 and ky==56789)
		rajz << color(szin.rr,szin.gg,szin.bb) << move_to(0,0) << box(sx,sy); // szín
	else 
		rajz << stamp(Tkepek,kx,ky,sx,sy,0,0); // Kép
	
	for (int i = 0; i < objektumok.size();++i) // Saját objektumai kirajzolása
	{
		canvas c;
		objektumok[i]->rajzol(c,Tkepek,sx,sy,(i==focusban&&focus)); // Ha focusban van és az ablak is: átadja neki.
		double ux,uy;
		objektumok[i]->getPosition(ux,uy);
		rajz << stamp(c,ux,uy);
	}
}

bool ABLAK::update(event ev, double X0, double Y0, KAMERA kamera)
{
	if (objektumok.size()==0) return false; // Ha nincs benne semmi, az alatta lévők updetelődhetnek.

	double ux,uy,uX0,uY0; // Kamera elmozdulás miatt kellenek.
	ux=x+X0;uy=y+Y0;
	uX0=X0;uY0=Y0;

	if (objektumok[focusban]->update(ev,ux,uy,kamera)) return true; // Ha benne lévőt updatelődik, akkor visszamenőleg nem.

	kamera.getKamCoords(ux,uy);
	kamera.getKamCoords(uX0,uY0);

	if (ev.type==ev_mouse)
	{
		if (ev.button==btn_left)
			for (int i = objektumok.size()-1; i >= 0; i--)
				if (objektumok[i]->BenneVan(ev.pos_x-ux,ev.pos_y-uy) and this->BenneVan(ev.pos_x-uX0,ev.pos_y-uY0)) // Az egér a belsőre kattint, mozgatható a belső, és a külsőben is benne van az egér
				{
					setFocus(i); // focusban rakja
					if (objektumok[focusban]->update(ev,ux,uy,kamera)) return true; // Rögtön updatelődik
					break;
				}
	}
	return false;
}

#endif