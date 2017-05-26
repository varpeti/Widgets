#include "szambeallito.hpp"
#include "kivalaszto.hpp"

class szgep : public ABLAK
{
	int s1,s2,muv;

	public:
		szgep() : ABLAK(10,10,500,400,SZIN(50,80,0))
		{
			for (int i = 0; i < 10; ++i)
			{
				stringstream s;
				s << i;
				objektumok.push_back( new STATTEXT(10+20*i,10,SZIN(100,50,0),SZIN(255,255,255),s.str()) ); // 0-9
			}
			
			objektumok.push_back( new STATTEXT(0*20+10,50,SZIN(0,0,1),SZIN(255,255,255),"+") ); //10
			objektumok.push_back( new STATTEXT(1*20+10,50,SZIN(0,0,1),SZIN(255,255,255),"-") );
			objektumok.push_back( new STATTEXT(2*20+10,50,SZIN(0,0,1),SZIN(255,255,255),"*") );
			objektumok.push_back( new STATTEXT(3*20+10,50,SZIN(0,0,1),SZIN(255,255,255),"/") );
			objektumok.push_back( new STATTEXT(4*20+10,50,SZIN(0,0,1),SZIN(255,255,255),"=") ); //14
	
			objektumok.push_back( new STATTEXT(10,90,SZIN(100,20,80),SZIN(255,255,255)," 0 ")); // 15

			vector<string> lista;
			objektumok.push_back( new KIVALASZTO(300,10,SZIN(0,255,255),SZIN(0,0,0),lista,10,20)); //16
	
			s1=0;s2=0;muv=1;
		}

		virtual bool update(event ev, double X0, double Y0, KAMERA kamera);
};

bool szgep::update(event ev, double X0, double Y0, KAMERA kamera)
{
	if (ev.type==ev_mouse)
	{
		objektumok[16]->update(ev,x+X0,y+Y0,kamera);

		if (ev.button==btn_left)
		{
			double ux,uy; // Kamera elmozdulás miatt kellenek.
			ux=x+X0;uy=y+Y0;
			kamera.getKamCoords(ux,uy);
			for (int i = objektumok.size()-1; i >= 0; i--)
			{
				if (objektumok[i]->BenneVan(ev.pos_x-ux,ev.pos_y-uy)) {
					if (i<10){
						s1=s1*10+i;
					} 
					else if (i>=10 and i<=14) {

						stringstream s;
						if (muv!=5) s << "1 " << s1 << " " << s2 << " " << (muv==1?"+":muv==2?"-":muv==3?"*":muv==4?"/":"=");

							if 		(muv==1) {s2=s2+s1;s1=0;}
							else if (muv==2) {s2=s2-s1;s1=0;}
							else if (muv==3) {s2=s2*s1;s1=0;}
							else if (muv==4) {s2=s2/s1;s1=0;}
							else if (muv==5) {s2=s1;s1=0;}
						

						if (muv!=5) {s << " " << s2; objektumok[16]->ss(s);}

							muv=i-9;
							if (muv==5) {s1=s2;s2=0;}

					}
					stringstream s;
					s << "2 " << s1 << " "; // Kiír
					objektumok[15]->ss(s);
					return true;
				}
			}
			
		}
	}
	return false; // Nem fogja el, (pl így el lehet húzni az alatta lévő ablakkal együtt.)
}

int main()
{
	ENV env (800,600,false);
	if(!env.kepek_beolvas("sp.bmp")) cout << "Nem talalhato a kep!" << endl;
	env.timer(20);

	OBJ *szamg = new szgep(); env.addObj(szamg);


	while(gin >> env.ev and env.ev.keycode!=key_escape) {
		env.UpdateDraw();
	}

	return 0;
}
