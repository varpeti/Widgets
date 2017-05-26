#ifndef _OBJ_
#define _OBJ_

#include "graphics.hpp"
#include "vector"
#include "kamera.hpp"
#include "iostream"
#include <sstream>

using namespace genv;
using namespace std;

class OBJ // Az ősosztály
{
	protected:
		double x,y; //x,y - relatív koordináták
		double sx,sy; //méret
		vector<OBJ*> objektumok; // Pl ablakban lehetnek widgetek, de akár több ablak is, vagy egy legördülő widgetben több statikus szöveg widget.
		unsigned int focusban; // A focusban lévő objektum vectorban elfoglalt helye.

	public:
		OBJ (double x,double y,double sx,double sy) : x(x), y(y), sx(sx), sy(sy), focusban(0) {}
		~OBJ () {while(objektumok.size()>0){delete objektumok[objektumok.size()-1]; objektumok.pop_back(); }}; // Minden alosztály tartalmát is kiszedi a memóriából.
		virtual void rajzol(canvas &rajz, canvas &Tkepek, double XX, double YY, bool focus) const {}; // rajzfelület, sprite, meret, focus
		virtual bool update(event ev, double X0, double Y0, KAMERA kamera) {};
		virtual void ss(stringstream &s) {};
		virtual void addObj(OBJ* obj);
		virtual void delObj(OBJ *obj);

		// Azért nem hülyeség a getter és setter mert SEXYBB mint a mezők :D | Egyébként az adatok főleg párosával vannak.
		void setPosition(double x,double y);
		void getPosition(double &x,double &y);
		void setMeret(double sx,double sy);
		void getMeret(double &sx,double &sy);
		void setFocus(unsigned int focusban);
		void getFocus(unsigned int &focusban);

		bool BenneVan(double px, double py);
};

void OBJ::addObj(OBJ *obj)
{
	objektumok.push_back(obj);
}

void OBJ::setPosition(double ux,double uy)
{
	x=ux;
	y=uy;
}

void OBJ::getPosition(double &ux,double &uy)
{
	ux=x;
	uy=y;
}

void OBJ::setMeret(double usx,double usy)
{
	sx=usx;
	sy=usy;
}

void OBJ::getMeret(double &usx,double &usy)
{
	usx=sx;
	usy=sy;
}

void OBJ::setFocus(unsigned int ufocusban)
{
	focusban=ufocusban;
}

void OBJ::getFocus(unsigned int &ufocusban)
{
	ufocusban=focusban;
}

bool OBJ::BenneVan(double px, double py)
{
	if (px>=x and px<=x+sx and py>=y and py<=y+sy) return true;
	return false;
};

void OBJ::delObj(OBJ *obj)
{	
	if (!obj) return;

	int id;
	for (int i = 0; i < objektumok.size(); ++i) // Megekerssük az ID-jét
	{
		if (objektumok[i]==obj) {id=i;break;}
	}
	delete obj;
	objektumok.erase(objektumok.begin()+id);
}

#endif