#include <iostream>
#include <conio.h>      /* getch() y kbhit() */
#include <windows.h>    /* Sleep() y gotoxy() */
#include <limits>       /* cin.good */
#include <ctime>        /* time() para srand() */
#include <cmath>
using namespace std;

template <typename objeto>
class listaDL{
private:
    struct nodo{
        objeto dato;
        nodo *next, *prev;
        nodo(const objeto &d=objeto{}, nodo *n=nullptr):dato{d}, next{n}, prev{n}{}
        nodo(objeto &&d=objeto{}, nodo *n=nullptr):dato{move(d)}, next{move(n)}, prev{move(n)}{}
    };
    nodo *head, *tail;
    int tamanio, capacidad;

public:
    listaDL(const listaDL <objeto> &rhs){iniciar();}
    listaDL(listaDL <objeto> &&rhs):head{rhs.head},tail{rhs.tail},tamanio{rhs.tamanio},capacidad{rhs.capacidad}{
        rhs.head=nullptr; rhs.tail=nullptr;
        rhs.tamanio=0;
        rhs.capacidad=10000;
    }
    listaDL(){iniciar();}
    ~listaDL(){
        nodo *aux;
        while(head != NULL){
            aux = head->next;
            delete head;
            head = aux;
        }
    }

    listaDL <objeto> & operator=(const listaDL <objeto> & rhs){
        if (this == &rhs) return *this; // handle self assignment
        return *this; //assignment operator
    }

    void iniciar(){
    	head=nullptr; tail=nullptr;
        tamanio=0;
        capacidad=10000;
    }
    bool vacia(){
        if(head == nullptr) return true;
        else return false;
    }
    int validarPos(int pos){
        if(pos == 0) return -1;
        if(pos == tamanio) return -2;
    	if(pos < tamanio-1 && pos > 0) return 1;
        return 0;
    }
    int getTamanio(){return tamanio;}

    void push_b(const objeto &x){
        nodo *p=new nodo{x, nullptr};
        if(vacia()){
            p->next=head;
            if (head != nullptr) head->prev=p;
            head=p;
            tail=head;
        }
        else{
            tail->next=p;
            p->prev=tail;
            tail=p;
    	}
        tamanio++;
    }
    void push_b(objeto &&x){
        nodo *p=new nodo{move(x),nullptr};
        if(vacia()){
            p->next=head;
            if (head != nullptr) head->prev=p;
            head=p;
            tail=head;
        }
        else{
            tail->next=p;
            p->prev=tail;
            tail=p;
    	}
        tamanio++;
    }
    void push_f(const objeto &x){
        nodo *p=new nodo{x, nullptr};
        p->next=head;
        head=p;
        if(vacia()) tail=head;
        tamanio++;
    }
    void push_f(objeto &&x){
        nodo *p=new nodo{move(x),nullptr};
        p->next=head;
        head=p;
        if(vacia()) tail=head;
        tamanio++;
    }

    void pop_b(){
        if (!vacia()) {
            nodo *tmp=tail;
            tail=tail->prev;
            tail->next=nullptr;
            delete tmp;
            tamanio--;
        }
    }
    void pop_f(){
        if (!vacia()) {
            nodo *tmp=head;
            head=head->next;
            delete tmp;
            if (head != nullptr) head->prev=nullptr;
            tamanio--;
        }
    }

    objeto peek_p(int pos){
        nodo *p=head;
        for(int i=0; i < pos; i++) {
            if (pos < tamanio) p=p->next;
            else break;
        }
        return p->dato;
    }

    void swap_s(int posUno, int posDos) {
        if (vacia()) return;
        if (posUno < 0 || posDos < 0) return; // Verificar si las posiciones son validas

        // Obtener los nodos correspondientes a las posiciones
        nodo* uno = head; nodo* dos = head;
        for (int i = 0; i < posUno && uno != nullptr; i++) uno = uno->next;
        for (int i = 0; i < posDos && dos != nullptr; i++) dos = dos->next;
        if (uno == nullptr || dos == nullptr) return; // Verificar si las posiciones son validas

        objeto datoUno = uno->dato;
        uno->dato = dos->dato;
        dos->dato = datoUno;
    }
};

//==============================================================================

#define VEL 750     /* Velocidad */
#define XP 37       /* X productor */
#define XC 75       /* X consumidor */
#define YPC 4       /* Y prod y cons */

#define XCONT 5     /* X contenedor */
#define YCONT 12    /* Y contenedor */
int ancho=11, alto=5, cols=10, rows=2;

void gotoxy(short int x, short int y){
    COORD position = {x, y};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), position);
}
void Ventanas(int x, int y, int ancho, int largo, int linea, string titulo){
    /* Crea una ventana (contorno) con los parametros dados */
    int centro, sisodd, aisodd, size=titulo.size();
    sisodd=size%2; aisodd=ancho%2; ancho--, largo--;
    if(size >= ancho) ancho=size+4;
    centro=ancho/2;

    if((sisodd && !aisodd) || (!sisodd && aisodd)) ancho--;
    if(!sisodd && !aisodd) centro++;
    //196='─' 179='│' 191='┐' 218='┌' 217='┘' 192='└' 180='┤' 195='├'
    char LH=196, LV=179, ESD=191, ESI=218, EID=217, EII=192, intD=180, intI=195;
    for(int A=0; A < ancho; A++){gotoxy(x+A, y); cout << LH; gotoxy(x+A, y+largo); cout << LH;}
    for(int L=0; L < largo; L++){gotoxy(x, y+L); cout << LV; gotoxy(x+ancho, y+L); cout << LV;}
    gotoxy(x, y); cout << ESI; gotoxy(x+ancho, y); cout << ESD; gotoxy(x, y+largo); cout << EII; gotoxy(x+ancho, y+largo); cout << EID;
    gotoxy(x+centro-(size/2), y+1); cout << titulo;
    if(linea == 1){
        gotoxy(x, y+2); for(int xd=0; xd < ancho; xd++) cout << LH;
        gotoxy(x, y+2); cout << intI; gotoxy(x+ancho, y+2); cout << intD;
    }
}
int getRandomNum(int range, int s){
    srand((unsigned) s+time(NULL));
    return rand()%range;
}

void drawIU(){
    system("cls");
    Ventanas(0, 0, 121, 30, 1, "Productor - Consumidor");
    gotoxy(2, 1); cout << "Programa 5";
    gotoxy(XP, YPC+1);   cout << "Productor";
    gotoxy(XP, YPC+2); cout << "(\\(\\   ";
    gotoxy(XP, YPC+3); cout << "(-.-) ";
    gotoxy(XP, YPC+4); cout << "O_(\")(\") ";
    gotoxy(XC-1, YPC); cout << "Consumidor";
    gotoxy(XC, YPC+1); cout << "  ^~^  ";
    gotoxy(XC, YPC+2); cout << " ('Y') )";
    gotoxy(XC, YPC+3); cout << " /   \\/";
    gotoxy(XC, YPC+4); cout << "(\\|||/)";

    Ventanas(XCONT-1, YCONT, ancho*cols+2, alto*rows+4, 1, "Contenedor");
    for (int y=0; y < rows; y++) for (int x=0; x < cols; x++) {
        Ventanas(XCONT+x*ancho, YCONT+3+y*alto, ancho, alto, 0, " ");
        gotoxy(XCONT+1+x*ancho, YCONT+6+y*alto); cout << (x+1)+(y*10);
    }
}
void actualizaEstado(listaDL <string> *contenedor, int *prod, int *cons){
    string estado; int cant=0;

    for (int i=0; i < 20; i++) if (contenedor->peek_p(i) != "") cant++;
    if (cant == 20 && *prod) {*prod=2;}
    switch (*prod) {
        case 0:{estado="   mimido   "; break;}
        case 1:{estado=" chambeando "; break;}
        case 2:{estado=" Esta lleno "; break;}
        case 3:{estado="Esta ocupado"; break;}
    }
    gotoxy(XP-2, YPC+5); cout << estado;

    if (cant == 0 && *cons) *cons=2;
    if (*cons && *prod) *cons=3;
    if (cant == 20 && *cons == 3) *cons=1;
    switch (*cons) {
        case 0:{estado="   mimido   "; break;}
        case 1:{estado=" chambeando "; break;}
        case 2:{estado=" Esta vacio "; break;}
        case 3:{estado="Esta ocupado"; break;}
    }
    gotoxy(XC+10-estado.length(), YPC+5); cout << estado;
}
int main(){
    listaDL <string> contenedor;
    char c; string c2;
    int prod[2], cons[2], tecla=0, clock=0, cantidad=0;
    for (int i=0; i < 20; i++) contenedor.push_b("");
    for (int i=0; i < 2; i++) {prod[i]=0; cons[i]=0;}
    drawIU();
    while (tecla != 27) {
        actualizaEstado(&contenedor, &prod[0], &cons[0]);
        for (int y=0; y < rows; y++) for (int x=0; x < cols; x++) {
            gotoxy(XCONT+3+x*ancho, YCONT+5+y*alto); cout << "       ";
            gotoxy(XCONT+5+x*ancho, YCONT+5+y*alto); cout << contenedor.peek_p((x+1)+(y*10)-1);
        }

        if (prod[0] == 1 && clock%2) {
            tecla=0;
            cantidad=getRandomNum(3, clock*7)+4;
            gotoxy(XP-3, YPC+6); cout << "Produciendo: " << cantidad;
            while(tecla < cantidad){
                if (contenedor.peek_p(prod[1]) == "") {
                    c=getRandomNum(93, clock*4*tecla)+33; c2=c;
                    contenedor.push_b(c2);
                    contenedor.swap_s(prod[1], 20);
                    contenedor.pop_b();
                    tecla++; prod[1]+=1;
                    if (prod[1] == 20) prod[1]=0;
                }
                else break;
            }
        }
        if (cons[0] == 1 && clock%2) {
            tecla=0;
            cantidad=getRandomNum(3, clock*3)+4;
            gotoxy(XC-3, YPC+6); cout << "Consumiendo: " << cantidad;
            while(tecla < cantidad){
                if (contenedor.peek_p(cons[1]) != "") {
                    contenedor.push_b("");
                    contenedor.swap_s(cons[1], 20);
                    contenedor.pop_b();
                    tecla++; cons[1]+=1;
                    if (cons[1] == 20) cons[1]=0;
                }
                else break;
            }
        }
        gotoxy(116, 27); Sleep(VEL); clock++; gotoxy(3, 27);
        switch (clock%4) {
            case 0:{printf("%c", 192); break;}
            case 1:{printf("%c", 218); break;}
            case 2:{printf("%c", 191); break;}
            case 3:{printf("%c", 217); break;}
        }
        gotoxy(XP-3, YPC+6); cout << "              ";
        gotoxy(XC-3, YPC+6); cout << "              ";
        prod[0] = (getRandomNum(400, clock*12) < 200) ? TRUE : FALSE;
        cons[0] = (getRandomNum(400, clock*7) < 200) ? TRUE : FALSE;
        if (kbhit()) tecla=getch();
    }
    gotoxy(2, 27); cout << "**Ejecucion finalizada**";
    gotoxy(2, 28); system("pause"); return 0;
}
