//	Minden objektumban úlyabb objektumok lehetne. És mivel volt gyerekválalási tanácsdónál ezért különbőző típúsú objektumok lehetnek egymásba.
//	Az alobjektumok relatív koordinátával rendelkeznek, a "felettük" lévő bal felső sarka a (0,0) pont.
//	Egy példa, ahol az OBJ alat bármilyen leszármaztatott osztályt lehet érteni.
//
//		___OBJ[0]___________________ 		___OBJ[1]________________
//		|	___OBJ[0].OBJ[0]_____ 	|		|	___OBJ[1].OBJ[1]_	|
//		|	|					|	|		|	|				|	|
//		|	|OBJ[0].OBJ[0].OBJ_ |	|		|	|_______________|	|
//		|	||________________|	|	|		|	___OBJ[1].OBJ[2]_	|
//		|	|___________________|	|		|	|_______________|	|
//		|___________________________|		|_______________________|
//
// Rajzolás paraméterei:
//		Tkepek	- A képeket tartalmazó canvas
//      rajz    - Az a canvas amire kirajzolja magát, aki meghívta az jeleníti meg.
//		X0,Y0	- A felette lévő OBJEKTUM (vagy képernyő) bal felső sarka. Azért kell külön ez is mert relatív koordináták vannak.
// 		kamera 	- A kamera

#ifndef _ENV_	//ujabb definíció és fordítási hiba elkerülésére
#define _ENV_

#include "obj.hpp"


class ENV : public OBJ
{
	public:
		canvas Tkepek;	//az összes OBJ képét ide olvasom be
	
		unsigned int XX; // Képernyő szélesség
		unsigned int YY; // magasság
	
		event ev;
		KAMERA kamera;

		ENV(unsigned int szelesseg, unsigned int magassag, bool teljeskepernyo) : OBJ(0,0,szelesseg,magassag)
		{
			YY=magassag;
			XX=szelesseg;
			gout.open(XX,YY,teljeskepernyo);
			Tkepek.transparent(true);

		}
		ENV(unsigned int szelesseg, unsigned int magassag) : OBJ(0,0,szelesseg,magassag)
		{
			YY=magassag;
			XX=szelesseg;
			gout.open(XX,YY);
			Tkepek.transparent(true);
		}

		void UpdateDraw();

		bool kepek_beolvas(const char *fname); // BMP-ből Tkepek-be

		void timer(int ido){gin.timer(ido);} // Csak
	
};

void ENV::UpdateDraw()
{
	if (objektumok.size()<1) return;

	static bool lenyomva=false; 
	static double ex=0,ey=0;

	if (ev.type==ev_timer)
	{
		gout << color(0,0,0) << move_to(0,0) << box(XX,YY); // Háttér

		for (int i = 0; i < objektumok.size();++i) // Saját objektumait kirajzolja.
		{
			canvas c;
			objektumok[i]->rajzol(c,Tkepek,XX,YY,i==focusban);
			double ux,uy;
			objektumok[i]->getPosition(ux,uy);
			kamera.getCoords(ux,uy);
			gout << stamp(c,ux,uy);
		}

		gout << refresh;
	}else if (ev.type==ev_mouse)
	{
		if (ev.button==btn_left)
		{
			double uX0=0,uY0=0;
			kamera.getCoords(uX0,uY0);
			for (int i = objektumok.size()-1; i >= 0; i--)
				if (objektumok[i]->BenneVan(ev.pos_x-uX0,ev.pos_y-uY0)) 
				{
					objektumok[i]->getPosition(ex,ey);
					ex-=ev.pos_x;
					ey-=ev.pos_y;
					lenyomva=true;

					objektumok.push_back(objektumok[i]);
					objektumok.erase(objektumok.begin()+i); // Felülre rakja

					focusban=objektumok.size();
					break;
				}
		}
		if (-ev.button==btn_left) {lenyomva=false; ex=0; ey=0;}

		if (lenyomva) objektumok[focusban]->setPosition(ev.pos_x+ex,ev.pos_y+ey);
	}

	objektumok[focusban]->update(ev,0,0,kamera);
}

bool ENV::kepek_beolvas(const char *fname) // CSAK azért is BMPből.
{
	FILE* f = fopen(fname, "rb"); if (!f) return false;
	char info[54] = {0}; // 54 byte: az infók
	fread(info, sizeof(char), 54, f); 

	unsigned int width = *(int*)&info[18]; //18. szélesség
	unsigned int height = *(int*)&info[22]; //22. magasság

	unsigned int size = 3 * width * height; // 3 byte pixelenként
	char* data = new char[size]; // lefoglalás
	fread(data, sizeof(char), size, f); // beolvasás egyszerre / csak ha nincsenek színtérinformációk, különben csúszik / 1280x608
	fclose(f);

	Tkepek.open(width,height); // canvas megnyitása
	unsigned int k = 0;
	for (unsigned int i = 0; i < height; i++) //y
	{
		for (unsigned int j = 0; j < width; j++) //x
		{
			unsigned int bb = data[k]; k++; // BBGGRR formátumot
			unsigned int gg = data[k]; k++;
			unsigned int rr = data[k]; k++;
			Tkepek << move_to(j,height-i) << color(rr,gg,bb) << dot; // fejjel lefele
		}
	}
	delete data; //Nem használt memória felszabadítása
	return true;
} 

#endif