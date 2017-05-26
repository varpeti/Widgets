#include "env.hpp"
#include "ablak.hpp"
#include "stattext.hpp"
#include "textbox.hpp"
#include "szambox.hpp"
#include "szambeallito.hpp"
#include "kivalaszto.hpp"

int main()
{
	ENV env (800,600,false);
	if(!env.kepek_beolvas("sp.bmp")) cout << "Nem talalhato a kep!" << endl;
	env.timer(20);

	OBJ *ablak = new ABLAK(-30,10,100,70,SZIN(0,255,0)); env.addObj(ablak);
	OBJ *abla2 = new ABLAK(-30,10,500,400,0,0); env.addObj(abla2);
	OBJ *statt = new STATTEXT(15,15,SZIN(0,0,0),SZIN(150,100,0),"Az almaspitet szeretem a legjobban, de a kortes is finom!"); abla2->addObj(statt);
	OBJ *tebox = new TEXTBOX(10,10,10,SZIN(100,100,0),SZIN(255,255,255)); ablak->addObj(tebox);
	OBJ *szamb = new SZAMBOX(15,70,-900.12,3.141592e12,SZIN(0,0,1),SZIN(255,255,255),333,13); abla2->addObj(szamb);
	OBJ *szam2 = new SZAMBEALLITO(400,400,-100,100,SZIN(200,150,100),SZIN(0,0,1)); env.addObj(szam2);

	vector<string> lista; lista.push_back("almaspite");lista.push_back("barack");lista.push_back("citrom");lista.push_back("barack");lista.push_back("dinnye");lista.push_back("barack");lista.push_back("1");lista.push_back("26435");lista.push_back("szilva");
	OBJ *kival = new KIVALASZTO(10,110,SZIN(0,0,1),SZIN(255,255,255),lista,3,8); abla2->addObj(kival);

	while(gin >> env.ev and env.ev.keycode!=key_escape) {
		env.UpdateDraw();
		if (env.ev.keycode=='q') {
			stringstream s;
			s << ("2CSAK AZ ALMASPITE!!!!");
			statt->ss(s);
		}
		else if (env.ev.keycode=='w') {
			stringstream s;
			tebox->ss(s);
			string ki;
			getline(s,ki);
			cout << ki << endl;
		}
		else if (env.ev.keycode=='e') {
			stringstream s;
			szam2->ss(s);
			double ki;
			s >> ki;
			cout << ki << endl;
		}
		else if (env.ev.keycode=='r') {
			env.kamera.rset(0,15);
		}
		else if (env.ev.keycode=='t') {
			stringstream s;
			kival->ss(s);
			string ki;
			getline(s,ki);
			cout << ki << endl;
		}
		else if (env.ev.keycode=='z') {
			stringstream s;
			s << "1Csirek" << rand()%99 << endl;
			kival->ss(s);
			string ki;
			getline(s,ki);
			cout << ki << endl;
		}
		else if (env.ev.keycode=='u') {
			stringstream s("3barack");
			kival->ss(s);
			string ki;
			getline(s,ki);
			cout << ki << endl;
		}
		else if (env.ev.keycode=='i') {
			stringstream s("2");
			kival->ss(s);
			string ki;
			getline(s,ki);
			cout << ki << endl;
		}
	}

	return 0;
}
