#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <assert.h>
#include <exception>
#include <algorithm>
using namespace std;				//GEREKLÝ TANIMLAMALAR YAPILDI...


class Room;
class Player;						//CLASSLARI ÝÇÝÇE KULLANMAM GEREKTÝÐÝ ÝÇÝN BURADA BAZILARININ TANIMLAMALARINI YAPTIM.			
class Objects;						


class Door{
protected:
	Room* room1;							
	Room* room2;
	bool is_Lock;
	int password;					//KÝLÝTLÝ KAPININ ÞÝFRESÝ BURADA TUTULUYOR
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
	
	void lock(int pass){					//ALINAN ÝNTEGER BURADA KÝLÝT ÞÝFRESÝ YAPILIYOR.
		password=pass;
		is_Lock=true;}

	void unlock(int number){
		if(number==password)				//GÝRÝLEN SAYI DOÐRUYSA KAPI AÇILIYOR.
		is_Lock=false;}

	bool is_lock(){return is_Lock;}			//KAPININ KÝLÝTLÝ OLUP OLMADIÐININ KONTROLÜ ÝÇÝN BU FONKSÝYONU YAZDIM
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
	doorl->lock(418);										//KÝLÝTLÝ KAPI EKLENDÝÐÝNDE BURADAN GÖNDERÝLEN 
															//ÞÝFREYLE KAPI KÝLÝTLENÝYOR. BU OYUN ÝÇÝN ÞÝFRE 
															//ODALARDAKÝ OBJELERÝN AÇIKLAMALARINDA BULUNAN ÝNTEGER 
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

class Room: public MapSite{							//ODALARIN OLUÞTURULDUÐU FACTORY PATTERN DE PRODUCT CLASSI 
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
		return walls[0]->getDoor();						//TASARLADIÐIM ODADA 0 NOLU KAPI ÇIKIÞ KAPISI OLDUÐUNDAN 
														//BURADA WALL0 DAKÝ KAPIYI DÖNDÜREN BÝR FONKSÝYON YAZDIM
	}

	int getNumber(){	return number;	}
	string getName(){	return name;	}

	void addObject(Objects* obj){objects.push_back(obj);}				//ODAYA NESNE EKLENÝYOR.
	Objects* getObject(int i){return objects[i];}

	void removeObject(Objects* obj){									//ODADAN NESNE KALDIRILIYOR.
		std::remove(objects.begin(),objects.end(),obj);
	}

	void enter(Player* p){
		cout<<name<<" odasina girildi."<<endl;				//PLAYER ODAYA GÝRDÝÐÝNDE GEREKLÝ DÜZENLEMELER YAPILIYOR
																		
		walls[0]->getDoor()->open();						//HER ODADA KAPILAR O ODANIN WALL VEKTORÜNÜN 0. ELEMANINDA TUTULUYOR.
															//BU YÜZDEN ODAYA GÝRMEK ÝÇÝN BU DUVARDAKÝ KAPI AÇILIYOR.	
		players.push_back(p);								//ODAYA GÝREN PLAYER VECTORE EKLENÝYOR.
	}

	void exit(Player* p){
		walls[0]->getDoor()->close();
		
		std::remove(players.begin(),players.end(),p);			//ÇIKAN PLAYER KALDIRILIYOR
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

	void createRoom(){											//FACTORY PATTERN UYGULAMASINDA CREATE FONKSÝYONLARI 
																//BURADA UYGULANIYOR 4 ADET ODA MAP E EKLENÝYOR
		Room* room1 = new Room(1,"ROOM1");
		locations.push_back(room1);
		
		room1= new Room(2,"ROOM2");
		locations.push_back(room1);
		
		room1= new Room(3,"ROOM3");
		locations.push_back(room1);

		room1= new Room(4,"ROOM4");
		locations.push_back(room1);
	
	}

	void createDoor(){												//ODALAR ÝÇÝN GEREKLÝ KAPI NESNELERÝ BURADA TANIMLANIYOR.

		cout<<locations[0]->getName()<<" odasina ";
		walls[0]->addDoorWithLock(locations[0],locations[0]);		//TASARLADIÐIM OYUN PLATFORMUNDA TÜM KAPILAR 0 NOLU ODAYA AÇILDIÐINDAN 
		doors.push_back(walls[0]->getDoor());						//TÜM KAPILAR LOCATÝONS[0](ROOM1) ÝLE DÝÐER ODA ARASINDADIR.
		
		
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
		
												//ODA TASARIMIMDA 13 ADET DUVAR GEREKÝYORDU. DUVARLARI BURADA OLUÞTURDUM
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

		locations[0]->setWalls(wal);				//OLUÞTURULAN DUVARLARI ÝLGÝLÝ ODALARA EKLEDÝM.

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

class Objects{							//GEREKLÝ OBJELERÝ OLUÞTURMAK ÝÇÝN COMPOSÝTE PATTERNÝ KULLANDIM.
protected:								
	string name;						//BU CLASS COMPOSÝTE PATTERNÝN COMPONENT CLASSININ UYGULAMASIDIR.
	int description;					//AYRICA OBSERVER PATTERN- OBSERVER CLASSI METODLARI KULLANILMIÞTIR.
	Room* room;
	Player* player;
public:
	void setRoom(Room* r){				
		room=r;
		room->addObject(this);			//OBJENÝN ODASINI SET EDERKEN AYNI ZAMANDA OBJENÝN GÝRDÝÐÝ ODAYI DA BUNDAN HABERDAR ETTÝM.

	}
	Room* getRoom(){return room;}

	virtual string getName()=0;

	int getDescription(){return description;}

	virtual void add(Objects* ){}

	Player* getPlayer(){return player;}

	void rremove(){}

};

class Things:public Objects{					//GENEL NESNELERÝ BU CLASSTAN OLUÞTURDUM 
public:											//COMPOSÝTE PATTERN-LEAF CLASSI 
	Things(string nm, int desc){name= nm;
	description=desc;}
	string getName(){return name;}

};

class Bag:public Objects{						//LAPTOP GÝBÝ DÝÐER NESNELERÝ KAPSAYAN NESNELERÝ ÝSE BU CLASSTAN OLUÞTURDUM.
private:										//COMPOSÝTE PATTERN-COMPOSÝTE CLASSI 
	std::vector<Objects*> objects;
public:
	Bag(string nm, int desc){name= nm; description= desc;}
	string getName(){return name;}

	void add(Objects* obj){objects.push_back(obj);
	description+= obj->getDescription();				//PLAYERIN SAHÝP OLDUÐU NESNELERÝN TÜMÜNÜN DEÐERLERÝNÝN TOPLAMINA ULAÞABÝLMESÝ ÝÇÝN 
	}													//ÝÇÝÇE NESNELERÝN HEPSÝNÝN TOPLAMINI DIÞTAKÝ NESNEDE TUTTUM. 
														//BUNUN ÝÇÝN BURADA EKLENEN NESNENÝN AÇIKLAMASINDAKÝ ÝNT DEÐÝÞKENÝ BAG NESNESÝNÝN ÝNT DEÐÝÞKENÝNE EKLEDÝM.
	
	void rremove(Objects* obj){
		std::remove(objects.begin(),objects.end(),obj);
		description-=obj->getDescription();				//COMPOSÝTEDEN HERHANGÝ BÝR NESNE KALDIRILDIÐINDA ÝNT DEÐERÝN AZALMASI GEREKÝYOR.
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
	bool hasItem(Objects* obj){						//PLAYERIN HERHANGÝ BÝR NESNEYE SAHÝP OLUP OLMADIÐININ ÖLÇÜLMESÝ ÝÇÝN GEREKLÝ FONKSÝYON.
		for(int i =0; i< items.size(); i++)
		{
			if(items[i]==obj)
				return true;
		}
		return false;
	}

	void takeItem(Objects* obj){
		if(obj->getRoom()->getNumber() == room->getNumber())			//PLAYER VE OBJENÝN AYNI ODADA OLUP OLMADIÐI KONTROL EDÝLÝYOR.
		{
			if(!(this->hasItem(obj))){									//PLAYERIN O NESNEYE ZATEN SAHÝP OLUP OLMADIÐI KONTROL EDÝLÝYOR.
			items.push_back(obj);
			password+=obj->getDescription();							//PLAYER BÝR OBJEYÝ ALDIÐINDA PASSWORD DEÐERÝ ARTIRILIYOR.
			
			cout<<"player "<<name<<" , "<<obj->getName()<<" objesini aldi"<<endl;
			}
			else 
				cout<<"nesne zaten oyuncuda..."<<endl;
		
		}
		else 
			cout<<"player ve obje ayni odada degil"<<endl;
	}

	int viewÝtem(int i){
		return items[i]->getDescription();
	}


	void dropItem(Objects* obj){								//PLAYERIN BÝR NESNEYÝ BIRAKMASINI SAÐLAYAN METOD.
		if(obj->getRoom()->getNumber()==room->getNumber()){		//OYUNCU ÝLE NESNENÝN AYNI ODADA OLUP OLMADIÐI KONTROL EDÝLÝYOR.
		if(hasItem(obj)){										//OYUNCU NESNEYE SAHÝPSE KALDIRILMA ÝÞLEMÝ YAPILIYOR.
		std::remove(items.begin(), items.end(), obj);
		items.pop_back();
		cout<<obj->getName()<<" objesi "<<name<<" oyuncusundan kaldirildi."<<endl;
		}
		else cout<<"obje bu playera ait degil"<<endl;			//DEÐÝLSE HATA MESAJI GÖNDERÝLÝYOR.
		}
		else
			cout<<"obje ile player ayni odada degil"<<endl;
	}


	Room* moveToRoom(Room* r){					//OBSERVER PATTERNÝN METODLARININ BÝR UYGULAMASIDIR.

	for(int i = 0;i<items.size();i++)
		{this->items[0]->setRoom(r);
		 this->room->removeObject(items[i]);	//PLAYER ODA DEÐÝÞTÝRDÝÐÝNDE SAHÝP OLDUÐU OBJELER TERKEDÝLEN ODADAN KALDIRILIR. 
		}
	Room* lastroom= room;
	room= r;
	return lastroom;
	}


	bool tryLoginPassword(){					//PLAYERIN TOPLADIÐI NESNELERLE ÞÝFREYÝ DENEMESÝNÝ SAÐLAYAN METOD.
		if(room->getNumber()==1){				//EÐER PLAYER ÇIKIÞ KAPISININ OLDUÐU ODADAYSA DENEMEYE ÝZÝN VERÝLÝYOR.
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




class AdventureGame{				//BU CLASS OYUNUN GERÇEKLEÞTÝÐÝ TÜM BÝLEÞENLERÝNÝN TUTULDUÐU CLASSTIR.
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


	void buildStructure(){							//BURADA ODA DUVAR VE KAPI BÝLEÞENLERÝ OLUÞTURULUYOR
		map= new Map();
		map->createRoom();
		map->createWall();
		map->createDoor();
	}

	void buildObjects(){						//OBJELER OLUÞTURULUYOR ÝSÝMLERÝ VE AÇIKLAMALARI BELÝRTÝLÝYOR.

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


		objects[0]->setRoom(map->getLocation(0));			//OBJELERÝN BULUNDUKLARI ODALAR SET EDÝLÝYOR.
		objects[1]->setRoom(map->getLocation(0));
		objects[2]->setRoom(map->getLocation(0));
		objects[3]->setRoom(map->getLocation(1));
		objects[4]->setRoom(map->getLocation(1));
		objects[5]->setRoom(map->getLocation(2));
		objects[6]->setRoom(map->getLocation(2));
		objects[7]->setRoom(map->getLocation(3));
		
	
	} 


	void buildPlayers(){							//OYUNCULAR OLUÞTURULUYOR. ÝSÝMLERÝ VE ODALARI GÖNDERÝLÝYOR.
		Player* pl1= new Player("zeynep", map->getLocation(0));
		players.push_back(pl1);
		Player* pl2= new Player("nur", map->getLocation(0));
		players.push_back(pl2);
	
	}

	void build(){					//OYUNUN KURULUMUNUN DAHA KOLAY BÝR ARAYÜZE SAHÝP OLMASI ÝÇÝN YAZILAN BÝR FONKSÝYON

	buildStructure();
	buildObjects();
	buildPlayers();
	
	}


};










int main(){

	AdventureGame* game=new AdventureGame();				

	game->build();												//oyun tüm altyapýsý oluþturuluyor...
	
	//aþaðýda bazý metodlar deneniyor.

	game->getPlayer(0)->takeItem(game->getObjects(2));			//0 nolu oyuncu 2 nolu objeyi alýyor.
	game->getPlayer(0)->takeItem(game->getObjects(1));
	game->getPlayer(0)->takeItem(game->getObjects(0));

	game->getPlayer(0)->moveToRoom(game->getMap()->getLocation(1));		//oyuncu oda deðiþtiriyor.
	game->getPlayer(0)->takeItem(game->getObjects(3));					//o odada bulunan nesneleri alýyor.
	game->getPlayer(0)->takeItem(game->getObjects(4));
	game->getPlayer(0)->dropItem(game->getObjects(2));					//oyuncu önceden aldýðý 2 nolu objeyi býrakýyor.
	game->getPlayer(0)->moveToRoom(game->getMap()->getLocation(2));
	game->getPlayer(0)->takeItem(game->getObjects(5));
	game->getPlayer(0)->takeItem(game->getObjects(7));					//odada olmayan nesne alýnmaya çalýþýlýyor..
	game->getPlayer(0)->takeItem(game->getObjects(6));
	game->getPlayer(0)->takeItem(game->getObjects(6));					//oyuncunun sahip olduðu nesne tekrar alýnmaya çalýþýlýyor fakat hata veriyor.

	game->getPlayer(0)->moveToRoom(game->getMap()->getLocation(0));
	game->getPlayer(0)->tryLoginPassword();								//þifre deneniyor fakat tüm objeler alýnmadýðý için doðru deðil kapý açýlmýyor.

	game->getPlayer(0)->moveToRoom(game->getMap()->getLocation(3));		//son kalan obje alýnýyor.
	game->getPlayer(0)->takeItem(game->getObjects(7));

	
	game->getPlayer(0)->moveToRoom(game->getMap()->getLocation(0));		
	game->getPlayer(0)->tryLoginPassword();								//þifre tekrar deneniyor kapý açýlýyor




  return 0;
}

