#ifndef _SZAMBEALLITO_	//ujabb definíció és fordítási hiba elkerülésére
#define _SZAMBEALLITO_

#include "szambox.hpp"

using namespace genv;

class SZAMBEALLITO : public ABLAK
{
	public:

		SZAMBEALLITO(double x, double y, double A, double B, SZIN szin, SZIN szin2, double szam=0, unsigned int size=8)
			: ABLAK(x,y,gout.twidth("0")*(size+2)+TEXT_RAHAGYAS*5+gout.cascent(),gout.cascent()*2+TEXT_RAHAGYAS*11,szin)
		{
			objektumok.push_back( new STATTEXT(gout.twidth("0")*size+TEXT_RAHAGYAS*4,2,szin2,szin," + ") );
			objektumok.push_back( new STATTEXT(gout.twidth("0")*size+TEXT_RAHAGYAS*4,gout.cascent()+TEXT_RAHAGYAS*6,szin2,szin," - ") );
			objektumok.push_back( new SZAMBOX(2,gout.cascent()/2+TEXT_RAHAGYAS*3.5,A,B,szin,szin2,szam,size) );
		}

		bool update(event ev, double X0, double Y0, KAMERA kamera); 
		void addObj(OBJ *obj) {}; // Nem lehet hozzáadni újabb objektumokat.
		void delObj(OBJ *obj) {}; // Nem lehet törölni objektumokat.
		virtual void ss(stringstream &s);
};

bool SZAMBEALLITO::update(event ev, double X0, double Y0, KAMERA kamera)
{
	if (ev.type==ev_key)
	{
		objektumok[2]->update(ev,X0+x,Y0+y,kamera); // A SZAMBOX
		return true; // Elfogja a karaktereket.
	}else if (ev.button==btn_left) // Egér
	{
		double ux,uy; // Kamera elmozdulás miatt kellenek.
		ux=x+X0;uy=y+Y0;
		kamera.getKamCoords(ux,uy);
		if (objektumok[0]->BenneVan(ev.pos_x-ux,ev.pos_y-uy)) {ev.type=ev_key; ev.keycode=key_up; objektumok[2]->update(ev,X0+x,Y0+y,kamera);return true;} //Hmm ez se rossz megoldás :D
		if (objektumok[1]->BenneVan(ev.pos_x-ux,ev.pos_y-uy)) {ev.type=ev_key; ev.keycode=key_down; objektumok[2]->update(ev,X0+x,Y0+y,kamera);return true;}
	}
	return false; // Nem fogja el
}

void SZAMBEALLITO::ss(stringstream &s)
{
	s.str(string()); s.clear(); // Takarítás
	objektumok[2]->ss(s); // Szamboxtól kéri el az értékét (double)
}


#endif