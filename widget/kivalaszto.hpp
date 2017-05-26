#ifndef _KIVALASZTO_	//ujabb definíció és fordítási hiba elkerülésére
#define _KIVALASZTO_

#include "textbox.hpp"

using namespace genv;

#define ELEM_RAHAGYAS (TEXT_RAHAGYAS*5)

class KIVALASZTO : public ABLAK
{
	private:
		unsigned int size2,size; // Azért kellenek hogy újat lehessen hozzáadni.
		SZIN szin2;
	public:

		KIVALASZTO(double x, double y, SZIN szin, SZIN szin2, vector<string> lista, unsigned int size=0,unsigned int size2=8)
			: ABLAK(x,y,gout.twidth("A")*size2+TEXT_RAHAGYAS*4.5,
				(gout.cascent()+TEXT_RAHAGYAS*5)*( (size ? size : (lista.size()+1) ) +1)+2,
				szin2), szin2(szin), size2(size2), size(size)
		{	
			string gorget;
			while (gorget.size()<size2) gorget+='-';
			objektumok.push_back( new STATTEXT(2,2,szin,szin2,gorget) );
			objektumok.push_back( new STATTEXT(2,2+(gout.cascent()+TEXT_RAHAGYAS*5)*(lista.size()+1),szin,szin2,gorget) );
			for (int i = 0; i < lista.size(); ++i)
			{
				while (lista[i].size()>size2) lista[i] = lista[i].substr(0, lista[i].size()-1);
				while (lista[i].size()<size2) lista[i]+=' ';
				objektumok.push_back( new STATTEXT(2,2+(gout.cascent()+TEXT_RAHAGYAS*5)*(i+1),szin,szin2,lista[i]) );
			}
			if (lista.size()>0)
			{
				focusban=2;
				stringstream s("1"); // Színcsere
				objektumok[focusban]->ss(s);
			}
		}

		bool update(event ev, double X0, double Y0, KAMERA kamera); 
		void addObj(OBJ *obj) {}; // Nem lehet hozzáadni újabb objektumokat.
		void delObj(OBJ *obj) {}; // Nem lehet törölni objektumokat.
		void ss(stringstream &s);
};

bool KIVALASZTO::update(event ev, double X0, double Y0, KAMERA kamera)
{
	if (ev.type==ev_mouse)
	{
		if (ev.button==btn_left)
		{
			double ux,uy,uX0,uY0; // Kamera elmozdulás miatt kellenek.
			ux=x+X0;uy=y+Y0;
			uX0=X0;uY0=Y0;

			kamera.getCoords(ux,uy);
			kamera.getCoords(uX0,uY0);

			for (int i = objektumok.size()-1; i >= 2; i--)
			{
				if (objektumok[i]->BenneVan(ev.pos_x-ux,ev.pos_y-uy) and this->BenneVan(ev.pos_x-uX0,ev.pos_y-uY0)) {
					stringstream s("1"); // Színcsere
					if (focusban>1) objektumok[focusban]->ss(s);
					s.str("1");
					objektumok[i]->ss(s);
					focusban=i;
					return true;
				}
			}
			
		}
		else if (ev.button==btn_wheelup)
		{
			double ox,oy;
			objektumok[0]->getPosition(ox,oy); if (oy>0) return false;
			for (int i = objektumok.size()-1; i >= 0; i--)
			{
				objektumok[i]->getPosition(ox,oy);
				objektumok[i]->setPosition(ox,oy+(gout.cascent()+TEXT_RAHAGYAS*5));
			}
			return true;
		}
		else if (ev.button==btn_wheeldown)
		{
			double ox,oy;
			objektumok[1]->getPosition(ox,oy); 
			if (oy<sy) return false;
			for (int i = objektumok.size()-1; i >= 0; i--)
			{
				objektumok[i]->getPosition(ox,oy);
				objektumok[i]->setPosition(ox,oy-(gout.cascent()+TEXT_RAHAGYAS*5));
			}
			return true;
		}
	}
	return false; // Nem fogja el, (pl így el lehet húzni az alatta lévő ablakkal együtt.)
}

void torol(vector<OBJ*> &objektumok, int id, unsigned int &focusban)
{
	double ox1,oy1,ox2,oy2;
	objektumok[objektumok.size()-1]->getPosition(ox1,oy1); // bezáró elem új poz
	
	ox2=ox1; oy2=oy1; objektumok[1]->getPosition(ox1,oy1);
	objektumok[1]->setPosition(ox2,oy2);
	for (int i = id; i<=objektumok.size()-1 ; ++i) // visszatolja fentről a lyukba a lista elemeit
	{
		ox2=ox1; oy2=oy1; objektumok[i]->getPosition(ox1,oy1);
		objektumok[i]->setPosition(ox2,oy2);
	} 
	
	delete objektumok[id]; 
	objektumok.erase(objektumok.begin()+id); // És törlés rendeltetés szerűen.
	
	if (focusban==id)
	{
		if (focusban==objektumok.size()) focusban--;
		stringstream s("1");
		objektumok[focusban]->ss(s);
	}else if(focusban>id)
	{
		focusban--;
	}
}

void KIVALASZTO::ss(stringstream &s)
{
	int i=0;
	s >> i;

	if (i==1) // ADD
	{
		string uj;
		getline(s,uj);

		while (uj.size()>size2) uj = uj.substr(0, uj.size()-1);
		while (uj.size()<size2) uj+=' ';
	
		double ox,oy;
		objektumok[1]->getPosition(ox,oy);
		objektumok.push_back( new STATTEXT(ox,oy,szin2,szin,uj) );
		objektumok[1]->setPosition(ox,oy+(gout.cascent()+TEXT_RAHAGYAS*5));
		
	}
	else if (i==2) // DEL jelenlegit
	{
		torol(objektumok,focusban,focusban);
	}
	else if (i==3) // DEL ezt
	{
		int van=-1;

		string ezt;
		getline(s,ezt);

		while (ezt.size()>size2) ezt = ezt.substr(0, ezt.size()-1);
		while (ezt.size()<size2) ezt+=' ';
		
		for (int i = 2; i < objektumok.size(); ++i)
		{	
			stringstream ki;
			objektumok[i]->ss(ki);
			string regi;
			getline(ki,regi);
			if (regi==ezt) {van=i;break;}
		}
		if (van!=-1) torol(objektumok,van,focusban);
		
	}
	
	//GET
	s.clear(); s.str(string());
	objektumok[focusban]->ss(s);
}


#endif