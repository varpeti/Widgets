#ifndef _TEXTBOX_	//ujabb definíció és fordítási hiba elkerülésére
#define _TEXTBOX_

#include "stattext.hpp"

class TEXTBOX : public STATTEXT
{
	protected:

	unsigned int size;
	string szoveg2;

	public:

	TEXTBOX(double x, double y, unsigned int size, SZIN szin, SZIN szin2, string szoveg="")
		: STATTEXT(x,y,szin,szin2,szoveg,TEXT_RAHAGYAS), size(size), szoveg2(szoveg)
	{
		sx=gout.twidth("A")*(size)+rahagyas*2.5;
	}

	virtual bool update(event ev, double X0, double Y0, KAMERA kamera);
	virtual void ss(stringstream &s);
};

bool TEXTBOX::update(event ev, double X0, double Y0, KAMERA kamera)
{
	
	if (ev.type==ev_key)
	{
		if (31<ev.keycode and ev.keycode<128) szoveg2+=ev.keycode; // Írás
		else if (ev.keycode==key_backspace) szoveg2 = szoveg2.substr(0, szoveg2.size()-1); // Törlés

		szoveg = szoveg2;
		while (szoveg.size()>size) szoveg = szoveg.substr(1, szoveg.size()); // Nincs több karakter hely, az elejéről "töröl" (csak a kiírást)
		
		return true; // Elfogja a karaktereket.
	}
	return false;
}

void TEXTBOX::ss(stringstream &s)
{
	s.str(string()); s.clear(); // Takarítás
	s << szoveg;
}


#endif