#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <assert.h>
#include <exception>
#include <algorithm>
using namespace std;				//GEREKL� TANIMLAMALAR YAPILDI...


class Room;
class Player;						//CLASSLARI ����E KULLANMAM GEREKT��� ���N BURADA BAZILARININ TANIMLAMALARINI YAPTIM.			
class Objects;						


class Door{
protected:
	Room* room1;							
	Room* room2;
	bool is_Lock;
	int password;					//K�L�TL� KAPININ ��FRES� BURADA TUTULUYOR
public:
	Door(){is_Lock=false;}							

	void open(){
		cout<<"kapi acildi"<<endl;
	}

	void close(){
		cout<<"kapi kapandi"<<endl;
	}

	void setRooms(Room* r1,Room* r2){
	room1=r1;
	room2=r2;
	}

	int getPassword(){return password;}

	Room* getOtherRoom(Room* r1){
		if(room1 == r1)
			return room2;
		else return room1;
	}
};

class DoorWithLock: public Door{
public:
	DoorWithLock(){is_Lock=true;}
	
	void lock(int pass){					//ALINAN �NTEGER BURADA K�L�T ��FRES� YAPILIYOR.
		password=pass;
		is_Lock=true;}

	void unlock(int number){
		if(number==password)				//G�R�LEN SAYI DO�RUYSA KAPI A�ILIYOR.
		is_Lock=false;}

	bool is_lock(){return is_Lock;}			//KAPININ K�L�TL� OLUP OLMADI�ININ KONTROL� ���N BU FONKS�YONU YAZDIM
};


class Wall{
private:
	Door* door;
public:
	Wall(){door=NULL;}
	
	void addDoor(Room* rm1,Room* rm2){						
	door = new Door();
	door->setRooms(rm1,rm2);
	cout<<"kapi eklendi"<<endl;
	}
	
	void setDoor(Door* dor){door=dor;}

	void addDoorWithLock(Room* rm1,Room* rm2)	{				
	DoorWithLock* doorl = new DoorWithLock();
	doorl->lock(418);										//K�L�TL� KAPI EKLEND���NDE BURADAN G�NDER�LEN 
															//��FREYLE KAPI K�L�TLEN�YOR. BU OYUN ���N ��FRE 
															//ODALARDAK� OBJELER�N A�IKLAMALARINDA BULUNAN �NTEGER 
															//SAYILARIN TOPLAMIDIR.
	door=doorl;	
	door->setRooms(rm1,rm2);
	cout<<"kilitli kapi eklendi"<<endl;
	}

	Door* getDoor()			{	return door;	}



};












//////////////////////////////////////////////////////////////////////////////

class MapSite{
public:
	void enter();
	void exit();

};

class Room: public MapSite{							//ODALARIN OLU�TURULDU�U FACTORY PATTERN DE PRODUCT CLASSI 
private:											//AYRICA OBSERVER PATTERNDE SUBJECT CLASSIDIR.
	int number;
	string name;
	std::vector<Wall*> walls;
	vector<Objects*> objects;
	vector<Player*> players;
public:
	Room(int _number, string _name ):number(_number),name(_name){}

	void setWalls(std::vector<Wall*> wall){
		walls=wall;
	}
	
	Door* getExitDoor(){
		return walls[0]->getDoor();						//TASARLADI�IM ODADA 0 NOLU KAPI �IKI� KAPISI OLDU�UNDAN 
														//BURADA WALL0 DAK� KAPIYI D�ND�REN B�R FONKS�YON YAZDIM
	}

	int getNumber(){	return number;	}
	string getName(){	return name;	}

	void addObject(Objects* obj){objects.push_back(obj);}				//ODAYA NESNE EKLEN�YOR.
	Objects* getObject(int i){return objects[i];}

	void removeObject(Objects* obj){									//ODADAN NESNE KALDIRILIYOR.
		std::remove(objects.begin(),objects.end(),obj);
	}

	void enter(Player* p){
		cout<<name<<" odasina girildi."<<endl;				//PLAYER ODAYA G�RD���NDE GEREKL� D�ZENLEMELER YAPILIYOR
																		
		walls[0]->getDoor()->open();						//HER ODADA KAPILAR O ODANIN WALL VEKTOR�N�N 0. ELEMANINDA TUTULUYOR.
															//BU Y�ZDEN ODAYA G�RMEK ���N BU DUVARDAK� KAPI A�ILIYOR.	
		players.push_back(p);								//ODAYA G�REN PLAYER VECTORE EKLEN�YOR.
	}

	void exit(Player* p){
		walls[0]->getDoor()->close();
		
		std::remove(players.begin(),players.end(),p);			//�IKAN PLAYER KALDIRILIYOR
	}

};











///////////////////////////////////////////////////////////////////////////////
class Map{
private:

	std::vector<Room*> locations;
	std::vector<Door*> doors;
	std::vector<Wall*> walls;

public:

	Map(){}

	Room* getLocation(int i){return locations[i];}

	void createRoom(){											//FACTORY PATTERN UYGULAMASINDA CREATE FONKS�YONLARI 
																//BURADA UYGULANIYOR 4 ADET ODA MAP E EKLEN�YOR
		Room* room1 = new Room(1,"ROOM1");
		locations.push_back(room1);
		
		room1= new Room(2,"ROOM2");
		locations.push_back(room1);
		
		room1= new Room(3,"ROOM3");
		locations.push_back(room1);

		room1= new Room(4,"ROOM4");
		locations.push_back(room1);
	
	}

	void createDoor(){												//ODALAR ���N GEREKL� KAPI NESNELER� BURADA TANIMLANIYOR.

		cout<<locations[0]->getName()<<" odasina ";
		walls[0]->addDoorWithLock(locations[0],locations[0]);		//TASARLADI�IM OYUN PLATFORMUNDA T�M KAPILAR 0 NOLU ODAYA A�ILDI�INDAN 
		doors.push_back(walls[0]->getDoor());						//T�M KAPILAR LOCAT�ONS[0](ROOM1) �LE D��ER ODA ARASINDADIR.
		
		
		cout<<locations[0]->getName()<<" ve "<<locations[1]->getName()<<" arasina ";
		walls[1]->addDoor(locations[0],locations[1]);
		doors.push_back(walls[1]->getDoor());
		
		cout<<locations[0]->getName()<<" ve "<<locations[2]->getName()<<" arasina ";
		walls[2]->addDoor(locations[0],locations[2]);
		doors.push_back(walls[2]->getDoor());
		
		cout<<locations[0]->getName()<<" ve "<<locations[3]->getName()<<" arasina ";
		walls[3]->addDoor(locations[0],locations[3]);
		doors.push_back(walls[3]->getDoor());
		

	}

	void createWall(){
		
												//ODA TASARIMIMDA 13 ADET DUVAR GEREK�YORDU. DUVARLARI BURADA OLU�TURDUM
		for(int i = 0; i<13; i++)
			{
				Wall* wall= new Wall();
				walls.push_back(wall);
			}

		std::vector<Wall*> wal;
		
		wal.push_back(walls[0]);
		wal.push_back(walls[1]);
		wal.push_back(walls[2]);
		wal.push_back(walls[3]);

		locations[0]->setWalls(wal);				//OLU�TURULAN DUVARLARI �LG�L� ODALARA EKLED�M.

		wal.clear();
		wal.push_back(walls[1]);
		wal.push_back(walls[4]);
		wal.push_back(walls[5]);
		wal.push_back(walls[6]);
		locations[1]->setWalls(wal);

		wal.clear();
		wal.push_back(walls[2]);
		wal.push_back(walls[7]);
		wal.push_back(walls[8]);
		wal.push_back(walls[9]);
		locations[2]->setWalls(wal);

		wal.clear();
		wal.push_back(walls[3]);
		wal.push_back(walls[10]);
		wal.push_back(walls[11]);
		wal.push_back(walls[12]);
		locations[3]->setWalls(wal);
	
	}

};











////////////////////////////////////////////////////////////////////////////////////

class Objects{							//GEREKL� OBJELER� OLU�TURMAK ���N COMPOS�TE PATTERN� KULLANDIM.
protected:								
	string name;						//BU CLASS COMPOS�TE PATTERN�N COMPONENT CLASSININ UYGULAMASIDIR.
	int description;					//AYRICA OBSERVER PATTERN- OBSERVER CLASSI METODLARI KULLANILMI�TIR.
	Room* room;
	Player* player;
public:
	void setRoom(Room* r){				
		room=r;
		room->addObject(this);			//OBJEN�N ODASINI SET EDERKEN AYNI ZAMANDA OBJEN�N G�RD��� ODAYI DA BUNDAN HABERDAR ETT�M.

	}
	Room* getRoom(){return room;}

	virtual string getName()=0;

	int getDescription(){return description;}

	virtual void add(Objects* ){}

	Player* getPlayer(){return player;}

	void rremove(){}

};

class Things:public Objects{					//GENEL NESNELER� BU CLASSTAN OLU�TURDUM 
public:											//COMPOS�TE PATTERN-LEAF CLASSI 
	Things(string nm, int desc){name= nm;
	description=desc;}
	string getName(){return name;}

};

class Bag:public Objects{						//LAPTOP G�B� D��ER NESNELER� KAPSAYAN NESNELER� �SE BU CLASSTAN OLU�TURDUM.
private:										//COMPOS�TE PATTERN-COMPOS�TE CLASSI 
	std::vector<Objects*> objects;
public:
	Bag(string nm, int desc){name= nm; description= desc;}
	string getName(){return name;}

	void add(Objects* obj){objects.push_back(obj);
	description+= obj->getDescription();				//PLAYERIN SAH�P OLDU�U NESNELER�N T�M�N�N DE�ERLER�N�N TOPLAMINA ULA�AB�LMES� ���N 
	}													//����E NESNELER�N HEPS�N�N TOPLAMINI DI�TAK� NESNEDE TUTTUM. 
														//BUNUN ���N BURADA EKLENEN NESNEN�N A�IKLAMASINDAK� �NT DE���KEN� BAG NESNES�N�N �NT DE���KEN�NE EKLED�M.
	
	void rremove(Objects* obj){
		std::remove(objects.begin(),objects.end(),obj);
		description-=obj->getDescription();				//COMPOS�TEDEN HERHANG� B�R NESNE KALDIRILDI�INDA �NT DE�ER�N AZALMASI GEREK�YOR.
		}

};











///////////////////////////////////////////////////////////////////////



class Player{										//OBSERVER PATTERN DE SUBJECT CLASSIDIR.
private:
	string name;
	Room* room;
	mutable std::vector<Objects*> items;
	int password;

public:
	Player(string n,Room* r){
		name=n;
		room=r;
		password=0;
	}

	void setRoom(Room* r){room=r;}
	
	Room* getRoom(){return room;}

	//void items(){}
	bool hasItem(Objects* obj){						//PLAYERIN HERHANG� B�R NESNEYE SAH�P OLUP OLMADI�ININ �L��LMES� ���N GEREKL� FONKS�YON.
		for(int i =0; i< items.size(); i++)
		{
			if(items[i]==obj)
				return true;
		}
		return false;
	}

	void takeItem(Objects* obj){
		if(obj->getRoom()->getNumber() == room->getNumber())			//PLAYER VE OBJEN�N AYNI ODADA OLUP OLMADI�I KONTROL ED�L�YOR.
		{
			if(!(this->hasItem(obj))){									//PLAYERIN O NESNEYE ZATEN SAH�P OLUP OLMADI�I KONTROL ED�L�YOR.
			items.push_back(obj);
			password+=obj->getDescription();							//PLAYER B�R OBJEY� ALDI�INDA PASSWORD DE�ER� ARTIRILIYOR.
			
			cout<<"player "<<name<<" , "<<obj->getName()<<" objesini aldi"<<endl;
			}
			else 
				cout<<"nesne zaten oyuncuda..."<<endl;
		
		}
		else 
			cout<<"player ve obje ayni odada degil"<<endl;
	}

	int view�tem(int i){
		return items[i]->getDescription();
	}


	void dropItem(Objects* obj){								//PLAYERIN B�R NESNEY� BIRAKMASINI SA�LAYAN METOD.
		if(obj->getRoom()->getNumber()==room->getNumber()){		//OYUNCU �LE NESNEN�N AYNI ODADA OLUP OLMADI�I KONTROL ED�L�YOR.
		if(hasItem(obj)){										//OYUNCU NESNEYE SAH�PSE KALDIRILMA ��LEM� YAPILIYOR.
		std::remove(items.begin(), items.end(), obj);
		items.pop_back();
		cout<<obj->getName()<<" objesi "<<name<<" oyuncusundan kaldirildi."<<endl;
		}
		else cout<<"obje bu playera ait degil"<<endl;			//DE��LSE HATA MESAJI G�NDER�L�YOR.
		}
		else
			cout<<"obje ile player ayni odada degil"<<endl;
	}


	Room* moveToRoom(Room* r){					//OBSERVER PATTERN�N METODLARININ B�R UYGULAMASIDIR.

	for(int i = 0;i<items.size();i++)
		{this->items[0]->setRoom(r);
		 this->room->removeObject(items[i]);	//PLAYER ODA DE���T�RD���NDE SAH�P OLDU�U OBJELER TERKED�LEN ODADAN KALDIRILIR. 
		}
	Room* lastroom= room;
	room= r;
	return lastroom;
	}


	bool tryLoginPassword(){					//PLAYERIN TOPLADI�I NESNELERLE ��FREY� DENEMES�N� SA�LAYAN METOD.
		if(room->getNumber()==1){				//E�ER PLAYER �IKI� KAPISININ OLDU�U ODADAYSA DENEMEYE �Z�N VER�L�YOR.
			if(password==room->getExitDoor()->getPassword())
			{
				room->getExitDoor()->open();
				cout<<"tebrikler!! cikmayi basardiniz..."<<endl;
				return true;
			
			}
			else {
				cout<<"yanlis sifre."<<endl;
				return false;
			}
		}
		else{
			cout<<"player cikis kapisinin bulundugu odada degil"<<endl;
		return false;	
		}
	}

};










//////////////////////////////////////////////////////////////////////////////////////////




class AdventureGame{				//BU CLASS OYUNUN GER�EKLE�T��� T�M B�LE�ENLER�N�N TUTULDU�U CLASSTIR.
private:
	Map* map;
	std::vector<Player*> players;
	std::vector<Objects*> objects;

public:
	AdventureGame(){
		map=NULL;
	}

	Player* getPlayer(int i){return players[i];}
	Objects* getObjects(int i) {return objects[i];}
	Map* getMap(){return map;}


	void buildStructure(){							//BURADA ODA DUVAR VE KAPI B�LE�ENLER� OLU�TURULUYOR
		map= new Map();
		map->createRoom();
		map->createWall();
		map->createDoor();
	}

	void buildObjects(){						//OBJELER OLU�TURULUYOR �S�MLER� VE A�IKLAMALARI BEL�RT�L�YOR.

		Objects* laptop= new Bag("laptop",0);
		
		Objects* obj= new Things("kalem",10);
		objects.push_back(obj);
		obj= new Things("kitap",20);
		objects.push_back(obj);

		obj= new Things("not", 30);
		objects.push_back(obj);
		
		obj= new Things("anahtarlik", 40);
		objects.push_back(obj);
		
		obj= new Things("cicek", 50);
		objects.push_back(obj);
		obj= new Things("saksi", 60);
		objects.push_back(obj);
		obj= new Things("dergi", 70);
		objects.push_back(obj);

		obj= new Things("dosya1", 28);
		laptop->add(obj);
		obj= new Things("dosya2", 35);
		laptop->add(obj);
		obj= new Things("dosya3", 75);
		laptop->add(obj);
		objects.push_back(laptop);


		objects[0]->setRoom(map->getLocation(0));			//OBJELER�N BULUNDUKLARI ODALAR SET ED�L�YOR.
		objects[1]->setRoom(map->getLocation(0));
		objects[2]->setRoom(map->getLocation(0));
		objects[3]->setRoom(map->getLocation(1));
		objects[4]->setRoom(map->getLocation(1));
		objects[5]->setRoom(map->getLocation(2));
		objects[6]->setRoom(map->getLocation(2));
		objects[7]->setRoom(map->getLocation(3));
		
	
	} 


	void buildPlayers(){							//OYUNCULAR OLU�TURULUYOR. �S�MLER� VE ODALARI G�NDER�L�YOR.
		Player* pl1= new Player("zeynep", map->getLocation(0));
		players.push_back(pl1);
		Player* pl2= new Player("nur", map->getLocation(0));
		players.push_back(pl2);
	
	}

	void build(){					//OYUNUN KURULUMUNUN DAHA KOLAY B�R ARAY�ZE SAH�P OLMASI ���N YAZILAN B�R FONKS�YON

	buildStructure();
	buildObjects();
	buildPlayers();
	
	}


};










int main(){

	AdventureGame* game=new AdventureGame();				

	game->build();												//oyun t�m altyap�s� olu�turuluyor...
	
	//a�a��da baz� metodlar deneniyor.

	game->getPlayer(0)->takeItem(game->getObjects(2));			//0 nolu oyuncu 2 nolu objeyi al�yor.
	game->getPlayer(0)->takeItem(game->getObjects(1));
	game->getPlayer(0)->takeItem(game->getObjects(0));

	game->getPlayer(0)->moveToRoom(game->getMap()->getLocation(1));		//oyuncu oda de�i�tiriyor.
	game->getPlayer(0)->takeItem(game->getObjects(3));					//o odada bulunan nesneleri al�yor.
	game->getPlayer(0)->takeItem(game->getObjects(4));
	game->getPlayer(0)->dropItem(game->getObjects(2));					//oyuncu �nceden ald��� 2 nolu objeyi b�rak�yor.
	game->getPlayer(0)->moveToRoom(game->getMap()->getLocation(2));
	game->getPlayer(0)->takeItem(game->getObjects(5));
	game->getPlayer(0)->takeItem(game->getObjects(7));					//odada olmayan nesne al�nmaya �al���l�yor..
	game->getPlayer(0)->takeItem(game->getObjects(6));
	game->getPlayer(0)->takeItem(game->getObjects(6));					//oyuncunun sahip oldu�u nesne tekrar al�nmaya �al���l�yor fakat hata veriyor.

	game->getPlayer(0)->moveToRoom(game->getMap()->getLocation(0));
	game->getPlayer(0)->tryLoginPassword();								//�ifre deneniyor fakat t�m objeler al�nmad��� i�in do�ru de�il kap� a��lm�yor.

	game->getPlayer(0)->moveToRoom(game->getMap()->getLocation(3));		//son kalan obje al�n�yor.
	game->getPlayer(0)->takeItem(game->getObjects(7));

	
	game->getPlayer(0)->moveToRoom(game->getMap()->getLocation(0));		
	game->getPlayer(0)->tryLoginPassword();								//�ifre tekrar deneniyor kap� a��l�yor




  return 0;
}

