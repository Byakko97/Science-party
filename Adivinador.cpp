#include <iostream>
#include <string>
#include <fstream>
#define trace(x) cerr<< #x << ": " << x << endl;
using namespace std;

class Arbol;
class Nodo{
private:
	string pregunta;
	Nodo* izq;
	Nodo* der;
public:
	Nodo(){ pregunta="", izq=NULL, der=NULL;}
	bool isLeaf(){ return izq==NULL && der== NULL;}
	void ask(){ cout<<"¿Piensas en "<<pregunta<<"?"<<endl; }
	friend class Arbol;
};

bool si(){
	string ans; 
	while(1){
		getline(cin, ans);
		if(ans[0]=='S' || ans[0]=='s' || ans[0]=='n' || ans[0]=='N') break;
		cout<< "Ingrese una respuesta válida." << endl;
	}
	if(ans[0]=='S' || ans[0]=='s') return 1;
	else return 0;
}

class Arbol{
private:
	Nodo* raiz;
	void clear(Nodo* cur){
		if(cur->izq) clear(cur->izq);
		if(cur->der) clear(cur->der);
		delete cur;
	}
	void preorder(ofstream& out, Nodo* cur){
		out<<cur->pregunta<<endl;
		if(!cur->isLeaf()){
			out<<1<<endl;
			preorder(out, cur->izq);
			preorder(out, cur->der);
		}else out<<0<<endl;
	}
	Nodo* desPriv(ifstream& in){
		Nodo* nodo;
		int hijos;
		nodo = new Nodo;
		getline(in, nodo->pregunta);
		in>>hijos; in>>ws;
		if(hijos){
			nodo->izq=desPriv(in);
			nodo->der=desPriv(in);
		}
		return nodo;
	}
public:
	Arbol(){
		raiz=new Nodo;
		raiz->pregunta = "perro";
	}
	virtual ~Arbol(){
		if(raiz) clear(raiz);
	}
	void jugar(){
		Nodo *cur, *nuevo, *padre=NULL, *nuevoPadre;
		string preg;
		cur = raiz;
		bool left;
		while(!cur->isLeaf()){
			cout<<cur->pregunta<<endl;
			padre=cur;
			if(si()){
				left=1;
				cur = cur->izq;
			}else{
				left=0;
				cur=cur->der;
			}
		}
		cur->ask();
		if(si()) cout<< "¡Adiviné!" << endl;
		else{
			cout<<":( ¿En qué estabas pensando?"<<endl;
			getline(cin, preg); 
			nuevo = new Nodo;
			nuevo->pregunta = preg;
			cout<<"¿Con qué pregunta distinguirías "<<cur->pregunta<<" de "<<preg<<"?"<<endl;
			getline(cin, preg); 
			nuevoPadre = new Nodo;
			nuevoPadre->pregunta = preg;
			cout<<"¿Y cuál sería la respuesta para que sea "<<nuevo->pregunta<<"?"<<endl;
			if(si()){
				nuevoPadre->izq = nuevo;
				nuevoPadre->der=cur;
			}else{
				nuevoPadre->der = nuevo;
				nuevoPadre->izq=cur;			
			}
			if(!padre) raiz=nuevoPadre;
			else if(left) padre->izq=nuevoPadre;
				else padre->der=nuevoPadre;
		}
	}
	void serialize(){
		ofstream out("BD.DAT", ios::out);
		if(!out){
			cerr<<"No se pudo escribir en el archivo"<<endl;
			exit(1);
		}
		if(raiz) preorder(out,raiz);
	}
	void deserialize(){
		ifstream in("BD.DAT", ios::in);
		if(!in){
			cerr<<"No se pudo leer en el archivo"<<endl;
			exit(1);
		}	
		bool hijos;
		in>>ws;
		if(!in.eof()){
			raiz = new Nodo;
			getline(in, raiz->pregunta);
			in>>hijos; in>>ws;
			if(hijos){
				raiz->izq=desPriv(in);
				raiz->der=desPriv(in);
			}
		}
	}
};

int main(){
	Arbol adivina;
	adivina.deserialize();
	cout<<"Voy a adivinar en qué piensas"<<endl;
	while(1){
		adivina.jugar();
		cout<<"¿Quieres volver a jugar?"<<endl;
		if(si()) cout<<endl;
		else break;
	}
	adivina.serialize();
	return 0;
}
