#include <iostream>
#include <conio.h>      /* getch() y kbhit() */
#include <windows.h>    /* Sleep() y gotoxy() */
#include <limits>       /* cin.good */
#include <ctime>        /* time() para srand() */
#include <cmath>
#include "colorama.h"
using namespace std;

class Proceso{
public:
    struct operando{
        float op1, op2;
        string op;
    };
    Proceso(){didF=FALSE; isAt=FALSE; TCr=0; TLl=-1; TEj=0; TBl=0; TFi=0; TTr=0; Qt=0;}
    void setProceso(int a, int b, int s, operando d){
        pID=a; TEM=b; size=s;
        operacion.op=d.op;
        operacion.op1=d.op1;
        operacion.op2=d.op2;
    }
    void setEstado(int a, int contG){
        state=a;
        switch (state) {
            /*Nuevo*/      case 0:{TCr=contG; break;}
            /*Listo*/      case 1:{if (TLl == -1) TLl=contG; break;}
            /*Ejecucion*/  case 2:{if (!isAt) {isAt=TRUE; TEj=contG;} break;}
            /*Bloqueado*/  case 3:{TBl=contG; break;}
            /*Finalizado*/ case 4:{TFi=contG; break;}
        }
    }
    void setTTr(int a){TTr=a;}
    void setFail(){didF=TRUE;}
    void setPID(int a){pID=a;}
    void setQt(int a){Qt=a;}

    int getPID(){return pID;}
    int getSize(){return size;}
    int getQt(){return Qt;}
    int getEstado(){return state;}
    int didFail(){return didF;}
    int getAt(){return isAt;}
    int getTEM(){return TEM;}
    int getTTr(){return TTr;}
    int getTLl(){return TLl;}
    int getTF(){return TFi;}
    int getTBl(){return TBl;}
    int getTRt(){return TFi-TLl;}
    int getTRs(){return TEj-TLl;}
    int getTEs(){return getTRt()-getTTr();}

    void getOpS(){printf("%.1f", operacion.op1);cout<<operacion.op;printf("%.1f", operacion.op2);}
    operando getOp(){return operacion;}
private:
    bool isAt, didF;
    int pID, state, size;
    int TEM, TCr, TLl, TEj, TBl, TFi, TTr, Qt;
    operando operacion;
};

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
        // Verificar si las posiciones son validas
        if (posUno < 0 || posDos < 0) return;

        // Obtener los nodos correspondientes a las posiciones
        nodo* uno = head;
        nodo* dos = head;
        for (int i = 0; i < posUno && uno != nullptr; i++) uno = uno->next;
        for (int i = 0; i < posDos && dos != nullptr; i++) dos = dos->next;

        // Verificar si las posiciones son validas
        if (uno == nullptr || dos == nullptr) return;

        // Intercambiar los datos de los nodos
        objeto datoUno = uno->dato;
        uno->dato = dos->dato;
        dos->dato = datoUno;
    }
};

struct marco{
    int pID, taken;
};

//==============================================================================

#define VEL 1000 /* Velocidad de ejecucion */
#define XLE 3    /* x Lote en Ejecucion */
#define XPB 30   /* x Procesos Bloqueados */
#define XPE 55   /* x Proceso en Ejecucion */
#define XPF 86   /* x Procesos Finalizados */
#define YIN 5    /* y interfaz */

#define MEM 220  /* Capacidad de memoria */
#define MSP 5    /* Capacidad de cada "marco" */
#define XTP 15   /* x Tabla paginas */
#define YTP 32   /* y Tabla paginas */

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
void grid(int x, int y, int cols, int lines, int cWidth, int cHeight){
    int xc, yc, cont=0;
    //   196='─' 179='│' 191='┐'  218='┌'  217='┘'  192='└'  180='┤'  195='├'  194='┬'  193='┴'  197='┼'
    char LH=196, LV=179, ESD=191, ESI=218, EID=217, EII=192, inD=180, inA=195, inW=194, inS=193, in=197;
    // Dibujar lineas horizontales y verticales
    for (int z=0; z < lines+1; z++) {
        for (int i=0; i < cols+1; i++) {
            xc=x+i*(cWidth+1); yc=y+z*(cHeight+1);
            for (int j=0; j < cWidth+1; j++) {
                if (i < cols) {gotoxy(xc+j, yc); cout << LH;}
            }
            for (int j=0; j < cHeight+1; j++) {
                if (z < lines){gotoxy(xc, yc+j); cout << LV;}
                if (j == 0) {
                    // Intersecciones del borde
                    if (z == 0) {gotoxy(xc+j, yc); cout << inW;}
                    if (z == lines) {gotoxy(xc+j, yc); cout << inS;}
                    if (i == 0) {gotoxy(xc+j, yc); cout << inA;}
                    if (i == cols) {gotoxy(xc+j, yc); cout << inD;}
                }
                if (j == 1 && i < cols && z < lines) {
                    gotoxy(xc+1, yc+1); cout << cont++;
                    gotoxy(xc+3, yc); cout << inW;
                    gotoxy(xc+3, yc+1); cout << LV;
                }
            }
            if ((i > 0 && i < cols) && (z > 0 && z < lines)) {gotoxy(xc, yc); cout << in;}
        }
    }
    // Poner esquinas
    gotoxy(x, y); cout << ESI;
    gotoxy(x+cols*(cWidth+1), y); cout << ESD;
    gotoxy(x, y+lines*(cHeight+1)); cout << EII;
    gotoxy(x+cols*(cWidth+1), y+lines*(cHeight+1)); cout << EID;
}
int intCheck(int x, int y, int pos, int limitLow, int limitHigh, string warn){
    int aux;
    while(1){
        gotoxy(x+pos, y); cout << "                         ";
        gotoxy(x, y+2); cout << "                         ";
        gotoxy(x+pos, y); cin >> aux;
        if(!cin.good()){cin.clear(); cin.ignore(numeric_limits<streamsize>::max(),'\n'); aux=-1;}
        if(aux >= limitLow && aux <= limitHigh) break;
        gotoxy(x, y+2); cout << warn; Sleep(750);
    }
    return aux;
}
int getRandomNum(int range, int s){
    srand((unsigned) s+time(NULL));
    return rand()%range;
}
void Conejito(int x, int y){
    gotoxy(x, y);   cout << "(\\(\\   ";
    gotoxy(x, y+1); cout << "(-.-) ";
    gotoxy(x, y+2); cout << "O_(\")(\") ";
}
void Gatito(int x, int y) {
    gotoxy(x, y);   cout << "  ^~^  ";
    gotoxy(x, y+1); cout << " ('Y') )";
    gotoxy(x, y+2); cout << " /   \\/";
    gotoxy(x, y+3); cout << "(\\|||/)";
}

void generaRes(Proceso *tempProceso){
    if (!tempProceso->didFail()) {
        Proceso::operando ope=tempProceso->getOp();
        float res;
        switch (ope.op[0]) {
            case '%':{int x=ope.op1, y=ope.op2; res=x%y; break;}
            case '+':{res=ope.op1+ope.op2; break;}
            case '-':{res=ope.op1-ope.op2; break;}
            case '*':{res=ope.op1*ope.op2; break;}
            case '/':{res=ope.op1/ope.op2; break;}
            case '^':{res=pow(ope.op1, ope.op2); break;}
        }
        printf("%.2f", res);
    }
    else cout << F_ORANGE << "ERROR" << F_RESET;
}
Proceso generaProceso(listaDL <Proceso> *procesos, int id, int contG){
    int ok=0; char op=' ';
    Proceso::operando operacion;
    Proceso tempProceso;

    switch (getRandomNum(6, id*5)) {
        case 1:{op='%'; break;}
        case 2:{op='/'; break;}
        case 3:{op='*'; break;}
        case 4:{op='-'; break;}
        case 5:{op='^'; break;}
        default:{op='+'; break;}
    }
    operacion.op1=getRandomNum(100, id*2);
    operacion.op2=getRandomNum(100, id*7);
    operacion.op=op;
    while (!ok) {
        if (operacion.op2 == 0 && (operacion.op == "/" || operacion.op == "%")) {
            operacion.op2=getRandomNum(100, id*3);
        } else {
            if (operacion.op2 > 6 && operacion.op == "^") {
                operacion.op1=getRandomNum(12, id*2)+1;
                operacion.op2=getRandomNum(4, id*3)+2;
            }
            else ok=1;
        }
    }
    tempProceso.setProceso(id, getRandomNum(12, id)+6, getRandomNum(20, id*5)+6, operacion);
    tempProceso.setEstado(0, contG);
    return tempProceso;
}
void control(int tecla, int *MOD){
    if (tecla == '-') if (*MOD/2 > 0) *MOD=*MOD/2;
    if (tecla == '+') if (*MOD*2 < VEL) *MOD=*MOD*2;
    if (tecla == '*') {*MOD = (VEL/ *MOD < VEL) ? *MOD=1 : *MOD=VEL*2; gotoxy(107, 1); cout << "            ";}
    if (tecla == 'p' || tecla == 'P') {
        gotoxy(2, 26); cout << "En Pausa";
        while(1){
            tecla=getch();
            if(tecla == 'c' || tecla == 'C') {gotoxy(2, 26); cout << "        "; break;}
        }
    }
    gotoxy(107, 1); cout << "Vel: " << VEL/ *MOD << "ms ";
}

void drawIU(int Q){
    system("cls");
    Ventanas(0, 0, 121, 30, 1, "Ejecutando");
    gotoxy(2, 1); cout << "Procesos nuevos ->";

    gotoxy(XLE-1, YIN); cout << "Procesos listos:";
    gotoxy(XLE, YIN+1); cout << "pID    TEM    TTr";

    gotoxy(XPB-1, YIN); cout << "Procesos Bloqueados:";
    gotoxy(XPB, YIN+1); cout << "pID    TBloq";

    int y=YIN;
    gotoxy(XPE, y++); cout << "Proceso en ejecucion:";
    gotoxy(XPE, y++); cout << "        pID | ";
    gotoxy(XPE, y++); cout << "  Operacion | ";
    gotoxy(XPE, y++); cout << "        TEM | ";
    gotoxy(XPE, y++); cout << "Trancurrido | ";
    gotoxy(XPE, y++); cout << "   Q Actual | ";
    gotoxy(XPE, y); cout << "   Restante | ";

    Ventanas(XPF-3, 2, 121-XPF+3, 28, 0, " ");
    gotoxy(119, 2); printf("%c", 180);
    gotoxy(XPF-3, 2); printf("%c", 194);
    gotoxy(XPF-3, 29); printf("%c", 193);
    gotoxy(XPF, 4); cout << "Procesos finalizados:";
    gotoxy(XPF, 5); cout << "pID    Operacion    Resultado";

    gotoxy(2, 28); cout << "Tiempo en ejecucion: ";
    gotoxy(71, 28); cout << "Quantum: " << Q;

    gotoxy(XTP+15, YTP-1); cout << "Memoria";
    grid(XTP, YTP, 4, 11, 8, 1);
    gotoxy(XTP+38, YTP+1); cout << "Marcos libres:";
    gotoxy(XTP+38, YTP+4); cout << "Uso:";
    gotoxy(XTP+38, YTP+6); cout << B_YELLOW << " Listo ";
    gotoxy(XTP+38, YTP+8); cout << B_RED << " Bloqueado ";
    gotoxy(XTP+38, YTP+10); cout << B_LBLUE << " Ejecucion ";
    gotoxy(XTP+38, YTP+12); cout << B_BLUE << " SO " << F_RESET;
}
void drawCE(int pos){
    string titulo;
    int y=30+(pos-1)*30;
    titulo="Procesos "+to_string((pos*23)+1)+" a "+to_string(23+pos*23);
    if (!(titulo.length()%2)) titulo=titulo+" ";
    Ventanas(XPF-3, y, 121-XPF+3, 30, 1, titulo);
    gotoxy(XPF, y+4); cout << "Procesos finalizados:";
    gotoxy(XPF, y+5); cout << "pID    Operacion    Resultado";
}
void imprimeListos(listaDL <Proceso> *listos){
    for (int i=1; i < listos->getTamanio(); i++) {
        gotoxy(XLE, YIN+2+i); cout << "                ";
        gotoxy(XLE, YIN+2+i); cout << listos->peek_p(i).getPID();
        gotoxy(XLE+7, YIN+2+i); cout << listos->peek_p(i).getTEM();
        gotoxy(XLE+14, YIN+2+i); cout << listos->peek_p(i).getTTr();
    }
    for (int i=listos->getTamanio(); i < 20; i++) {gotoxy(XLE, YIN+2+i); cout << "                ";}
}
void imprimeBloq(listaDL <Proceso> *bloq, int contG){
    for (int i=0; i < bloq->getTamanio(); i++) {
        gotoxy(XPB, YIN+2+i); cout << "                ";
        gotoxy(XPB, YIN+2+i); cout << bloq->peek_p(i).getPID();
        gotoxy(XPB+7, YIN+2+i); cout << contG-bloq->peek_p(i).getTBl();
    }
    for (int i=bloq->getTamanio(); i < 10; i++) {gotoxy(XPB, YIN+2+i); cout << "                ";}
}
void imprimeProcFin(Proceso *tempProceso, int YPF){
    gotoxy(XPF, YPF); cout << tempProceso->getPID();
    gotoxy(XPF+7, YPF); tempProceso->getOpS();
}
void imprimeProcEjec(Proceso *tempProceso){
    int y=YIN+1;
    for (int i=y; i < y+6; i++) {gotoxy(XPE+14, i); cout << "            ";}
    gotoxy(XPE+14, y++); cout << tempProceso->getPID();
    gotoxy(XPE+14, y++); tempProceso->getOpS();
    gotoxy(XPE+14, y++); cout << tempProceso->getTEM();
    gotoxy(XPE+14, y++); cout << tempProceso->getTTr();
    gotoxy(XPE+14, y++); cout << tempProceso->getQt();
    gotoxy(XPE+14, y++); cout << tempProceso->getTEM();
}

void imprimeProcBCP(int x, int y, int contG, Proceso tempProceso){
    gotoxy(x+33, y); cout << "x"; gotoxy(x+66, y); cout << "-"; gotoxy(x+78, y); cout << "-";
    switch(tempProceso.getEstado()){
        case 0:{gotoxy(x+6, y); cout << "Nuevo"; break;}
        case 1:{gotoxy(x+6, y); cout << F_YELLOW << "Listo" << F_RESET; break;}
        case 2:{gotoxy(x+6, 6); cout << F_LBLUE << "Ejecucion" << F_RESET; break;}
        case 3:{gotoxy(x+6, y); cout << F_RED << "Bloqueado" << F_RESET; break;}
        case 4:{gotoxy(x+6, y); cout << F_LGREEN << "Finalizado" << F_RESET;
            gotoxy(x+33, y); generaRes(&tempProceso);
            gotoxy(x+66, y); cout << tempProceso.getTF();
            gotoxy(x+78, y); cout << tempProceso.getTRt();
            break;
        }
    }
    gotoxy(x, y); cout << tempProceso.getPID();
    gotoxy(x+19, y); tempProceso.getOpS();
    gotoxy(x+48, y); cout << tempProceso.getTEM();                                                        /* TEM */
    gotoxy(x+54, y); cout << tempProceso.getTTr();                                                        /* Servicio */
    if(tempProceso.getTLl() == -1) {
        gotoxy(x+60, y); cout << "-";                                                   /* Llegada */
        gotoxy(x+72, y); cout << contG - tempProceso.getTTr();                          /* Espera */
    }
    else {
        gotoxy(x+60, y); cout << tempProceso.getTLl();                                  /* Llegada */
        gotoxy(x+72, y); cout << contG - tempProceso.getTLl() - tempProceso.getTTr();   /* Espera */
    }
    gotoxy(x+84, y); if (tempProceso.getAt()) cout << tempProceso.getTRs(); else cout << "-";             /* Respuesta */
    gotoxy(x+90, y); if (tempProceso.getEstado() != 4) cout << tempProceso.getTEM()-tempProceso.getTTr(); /* Restante */
    else cout << 0;
    // gotoxy(x+100, y); cout << tempProceso.getSize();
}
void muestraBCP(listaDL <Proceso> *nuevos, listaDL <Proceso> *listos, listaDL <Proceso> *bloq, listaDL <Proceso> *fin, int contG, Proceso enEjec, int salir){
    system("cls"); system("mode con: cols=120");
    int y=7, x=2;
    gotoxy(2, 4); cout << "pID   Estado       Operacion     Resultado      TEM   TS    TL    TF    TE    TRt   TRs   Restante";
    if (salir) y--;
    if (enEjec.getPID() != -1) imprimeProcBCP(x, 6, contG, enEjec);
    else y--;
    for(int i=0; i < listos->getTamanio(); i++) imprimeProcBCP(x, y++, contG, listos->peek_p(i));
    for(int i=0; i < bloq->getTamanio(); i++)   imprimeProcBCP(x, y++, contG, bloq->peek_p(i));
    if (nuevos->getTamanio() > 0) y++;
    for(int i=0; i < nuevos->getTamanio(); i++) imprimeProcBCP(x, y++, contG, nuevos->peek_p(i));
    if (fin->getTamanio() > 0) y++;
    for(int i=0; i < fin->getTamanio(); i++)    imprimeProcBCP(x, y++, contG, fin->peek_p(i));

    gotoxy(x, y+2); cout << "Tiempo en ejecucion: " << contG;
    if (!salir) {
        Ventanas(0, 0, 121, y+6, 1, "Tabla de procesos ");
        gotoxy(x, y+3); cout << "En Pausa"; gotoxy(0, 0);
        while(1){
            int tecla=getch();
            if(tecla == 'c' || tecla == 'C') break;
        }
    }
    else{
        Ventanas(0, 0, 121, y+7, 1, "Tabla de procesos ");
        gotoxy(92, 3); cout << "        ";
        gotoxy(x, y+4); cout << "**Ejecucion finalizada**";
        gotoxy(x, y+5);
    }

}

int getEstado(int pID, listaDL <Proceso> *listos, listaDL <Proceso> *bloq, int ejecucion){
    if (pID == ejecucion) return 2;
    for (int i=0; i < bloq->getTamanio(); i++) {
        if (bloq->peek_p(i).getPID() == pID) return 3;
    }
    for (int i=0; i < listos->getTamanio(); i++) {
        if (listos->peek_p(i).getPID() == pID) return 1;
    }
    return 0;
}
int getMarcosDisp(listaDL <marco> *memoria){
    int cont=0;
    for (int i=0; i < MEM/MSP; i++)
        if (memoria->peek_p(i).pID == 0) cont++;
    return cont;
}
void muestraMarcos(int xt, int yt, listaDL <marco> *memoria, listaDL <Proceso> *listos, listaDL <Proceso> *bloq, int ejecucion, int selec){
    marco tempMarco; int state;
    for (int y=0; y < 11; y++) {
        for (int x=0; x < 4; x++) {
            tempMarco=memoria->peek_p(y*4+x);
            gotoxy(xt+4+x*9, yt+1+y*2); for (int i=0; i < 5; i++) cout << " ";
            if (tempMarco.pID != 0) {
                state=getEstado(tempMarco.pID, listos, bloq, ejecucion);
                switch (state) {
                    case 1:{cout << F_YELLOW; break;}
                    case 2:{cout << F_LBLUE; break;}
                    case 3:{cout << F_RED; break;}
                }
                if (y == 10) cout << F_BLUE;
                if (tempMarco.pID == selec) {cout << F_MAGENTA;}
                gotoxy(xt+4+x*9, yt+1+y*2); for (int i=0; i < memoria->peek_p(y*4+x).taken; i++) printf("%c", 219);
            }
        }
    }
    cout << F_RESET;
}
int agregaMarco(listaDL <marco> *memoria, int pID, int size){
    int marNes=size/MSP;
    if (size%MSP != 0) marNes++;
    if (getMarcosDisp(memoria) < marNes) return 0;
    marco tempMarco; int tempSize=size;
    for (int i=0; i < MEM/MSP; i++) {
        if (memoria->peek_p(i).pID == 0){
            marNes--;
            if (tempSize > 5) {
                tempMarco.taken=5;
                tempSize-=5;
            }
            else tempMarco.taken=tempSize;
            tempMarco.pID=pID;
            memoria->push_b(tempMarco);
            memoria->swap_s(i, memoria->getTamanio()-1);
            memoria->pop_b();
        }
        if (marNes == 0) break;
    }
    return 1;
}
void borrarMarco(listaDL <marco> *memoria, int pID){
    marco tempMarco; tempMarco.pID=0; tempMarco.taken=0;
    for (int i=0; i < MEM/MSP; i++) {
        if (memoria->peek_p(i).pID == pID) {
            memoria->push_b(tempMarco);
            memoria->swap_s(i, memoria->getTamanio()-1);
            memoria->pop_b();
        }
    }
}
void imprimeMarcoUso(listaDL <marco> *memoria, int pID){
    for (int j=0; j < memoria->getTamanio(); j++) {
        if (memoria->peek_p(j).pID == pID) {
            cout << j << "  ";
            if (j < 10) cout << " ";
        }
    }
}
void muestraTP(listaDL <marco> *memoria, listaDL <Proceso> *listos, listaDL <Proceso> *bloq, Proceso ejecucion){
    int xm=78, ym=5, x=6, y=4;
    system("cls"); system("mode con: cols=120");

    Ventanas(0, 0, 121, 40, 1, "Tabla de paginas");
    gotoxy(3, 37); cout << "En Pausa";

    gotoxy(xm+15, ym-1); cout << "Memoria";
    grid(xm, ym, 4, 11, 8, 1);
    gotoxy(xm+5, ym+24); cout << "Marcos libres: " << getMarcosDisp(memoria);
    gotoxy(xm+5, ym+26); cout << "Uso:";
    gotoxy(xm+5, ym+28); cout << B_YELLOW << " Listo ";
    gotoxy(xm+20, ym+28); cout << B_LBLUE << " Ejecucion ";
    gotoxy(xm+5, ym+30); cout << B_RED << " Bloqueado ";
    gotoxy(xm+20, ym+30); cout << B_BLUE << " SO ";
    gotoxy(xm+5, ym+32); cout << B_MAGENTA << " Seleccionado " << F_RESET;

    gotoxy(x, y++); cout << "pID  Estado      Tamanio    Marcos en uso";
    if (ejecucion.getPID() > 0) {
        gotoxy(x, ++y); cout << ejecucion.getPID();
        gotoxy(x+5, y); cout << F_LBLUE << "Ejecucion" << F_RESET;
        gotoxy(x+17, y); cout << ejecucion.getSize();
        gotoxy(x+28, y); imprimeMarcoUso(memoria, ejecucion.getPID());

        for (int i=0; i < listos->getTamanio(); i++) {
            gotoxy(x, ++y); cout << listos->peek_p(i).getPID();
            gotoxy(x+5, y); cout << F_YELLOW << "Listo" << F_RESET;
            gotoxy(x+17, y); cout << listos->peek_p(i).getSize();
            gotoxy(x+28, y); imprimeMarcoUso(memoria, listos->peek_p(i).getPID());
        }
    }
    for (int i=0; i < bloq->getTamanio(); i++) {
        gotoxy(x, ++y); cout << bloq->peek_p(i).getPID();
        gotoxy(x+5, y); cout << F_RED << "Bloqueado" << F_RESET;
        gotoxy(x+17, y); cout << bloq->peek_p(i).getSize();
        gotoxy(x+28, y); imprimeMarcoUso(memoria, bloq->peek_p(i).getPID());
    }
    y+=2;
    gotoxy(x, y); cout << "SO";
    gotoxy(x+5, y); cout << F_BLUE << "Sistema" << F_RESET;
    gotoxy(x+17, y); cout << 20;
    gotoxy(x+28, y++); imprimeMarcoUso(memoria, -1);

    int tecla, sel, totalProc=listos->getTamanio()+bloq->getTamanio();
    if (ejecucion.getPID() > 0) totalProc++;
    gotoxy(x, ++y); cout << "Total de procesos en memoria: " << totalProc;
    int a=6; y=0;
    while(1){
        gotoxy(x-4, y+a); cout << "->";
        if (ejecucion.getPID() > 0){
            if (y == 0) sel=ejecucion.getPID();
            else if (y < totalProc-bloq->getTamanio()) sel=listos->peek_p(y-1).getPID();
            else if (y > totalProc-bloq->getTamanio()-1) sel=bloq->peek_p(y-1-listos->getTamanio()).getPID();
        }
        else{
            if (y < totalProc-bloq->getTamanio()) sel=listos->peek_p(y).getPID();
            else if (y > totalProc-bloq->getTamanio()-1) sel=bloq->peek_p(y-listos->getTamanio()).getPID();
        }
        muestraMarcos(xm, ym, memoria, listos, bloq, ejecucion.getPID(), sel);
        gotoxy(11, 37); tecla=getch();
        gotoxy(x-4, y+a); cout << "  ";
        if (tecla == 'c' || tecla == 'C') return;
        if (tecla == 80) {y++; if (y > totalProc-1) y=0;}
        if (tecla == 72) {y--; if (y < 0) y=totalProc-1;}
    }
}

void admision(int Q, listaDL <marco> *memoria, listaDL <Proceso> *nuevos, listaDL <Proceso> *listos, listaDL <Proceso> *bloq, listaDL <Proceso> *fin, int *contG, int *MOD, int proNum){
    Proceso tempProceso;
    if (listos->vacia() && !bloq->vacia()) {
        gotoxy(XPE+14, YIN+1); cout << "NULL";
        for (int i=YIN+2; i < 12; i++) {gotoxy(XPE+14, i); cout << "x";}
        muestraMarcos(XTP, YTP, memoria, listos, bloq, -2, -2);
        for (int i=0; i < bloq->getTamanio(); i++) {
            if (*contG-(bloq->peek_p(0).getTBl()) >= 8){
                tempProceso=bloq->peek_p(i);
                tempProceso.setEstado(1, *contG);
                listos->push_b(tempProceso);
                bloq->pop_f();
                imprimeListos(listos);
            }
        }
        imprimeBloq(bloq, *contG);

        gotoxy(23, 28); cout << *contG;
        Sleep(VEL/ *MOD); *contG+=1;
        if (kbhit()) {
            char tecla=getch();
            control(tecla, MOD);
            if (tecla == 'n' || tecla == 'N') {
                nuevos->push_b(generaProceso(nuevos, 1+proNum+nuevos->getTamanio()+listos->getTamanio()+bloq->getTamanio(), *contG));
                tempProceso=nuevos->peek_p(0);
                if (agregaMarco(memoria, tempProceso.getPID(), tempProceso.getSize())) {
                    nuevos->pop_f();
                    tempProceso.setEstado(1, *contG);
                    listos->push_b(tempProceso);
                }
                imprimeListos(listos);
            }
            if (tecla == 'b' || tecla == 'B') {
                int y, pos;
                Proceso temp2; temp2.setPID(-1);
                muestraBCP(nuevos, listos, bloq, fin, *contG, temp2, 0);
                drawIU(Q);
                for (int i=0; i < fin->getTamanio(); i++) {
                    temp2=fin->peek_p(i);
                    pos=(i)/23; y=6+i+7*pos;
                    if (i > 20 && (i+1)%23 == 1) drawCE(pos);
                    imprimeProcFin(&temp2, y);
                    gotoxy(XPF+20, y); generaRes(&temp2);
                }
            }
            if (tecla == 't' || tecla == 'T') {
                int y, pos;
                Proceso temp2; temp2.setPID(-2);
                muestraTP(memoria, listos, bloq, temp2);
                drawIU(Q);
                for (int i=0; i < fin->getTamanio(); i++) {
                    temp2=fin->peek_p(i);
                    pos=(i)/23; y=6+i+7*pos;
                    if (i > 20 && (i+1)%23 == 1) drawCE(pos);
                    imprimeProcFin(&temp2, y);
                    gotoxy(XPF+20, y); generaRes(&temp2);
                }
                imprimeBloq(bloq, *contG);
            }
        }
    }
    else {
        for (int i=0; i < nuevos->getTamanio(); i++) { // cosas pasan ------------------------------------------
            tempProceso=nuevos->peek_p(0);
            if (agregaMarco(memoria, tempProceso.getPID(), tempProceso.getSize())) {
                nuevos->pop_f(); i--;
                tempProceso.setEstado(1, *contG);
                listos->push_b(tempProceso);
            }
            else break;
        }
        imprimeListos(listos);
    }
    gotoxy(21, 1); cout << nuevos->getTamanio() << " ";

    gotoxy(XTP+53, YTP+1); cout << getMarcosDisp(memoria) << " ";
    if (nuevos->getTamanio() > 0) {
        gotoxy(2, 3); cout << "Siguiente >  pID: " <<nuevos->peek_p(0).getPID() << "  Tamanio: " << nuevos->peek_p(0).getSize() << " ";
    }
    else {gotoxy(2, 3); cout << "                                        ";}

}
int ejecutaProceso(int Q, listaDL <marco> *memoria, listaDL <Proceso> *nuevos, listaDL <Proceso> *listos, listaDL <Proceso> *bloq, listaDL <Proceso> *fin, Proceso *tempProceso, int proNum, int *contG, int *MOD){
    int YPF, pos=(proNum)/23, restante=tempProceso->getTEM()-tempProceso->getTTr();
    gotoxy(XLE, YIN+2); cout << "                    ";
    imprimeProcEjec(tempProceso);
    gotoxy(107, 1); cout << "Vel: " << VEL/ *MOD << "ms ";
    for (int i=1; i <= restante; i++) {
        gotoxy(23, 28); cout << *contG;
        Sleep(VEL/ *MOD); *contG+=1;
        tempProceso->setTTr(tempProceso->getTTr()+1);
        gotoxy(XPE+14, YIN+4); cout << tempProceso->getTTr();
        gotoxy(XPE+14, YIN+5); cout << i + tempProceso->getQt();
        gotoxy(XPE+14, YIN+6); cout << tempProceso->getTEM()-tempProceso->getTTr() << "  ";
        for (int i=0; i < bloq->getTamanio(); i++) {
            if (*contG-(bloq->peek_p(i).getTBl()) >= 8) {
                Proceso temp2=bloq->peek_p(i);
                temp2.setEstado(1, *contG);
                listos->push_b(temp2);
                bloq->pop_f();
                imprimeListos(listos);
                muestraMarcos(XTP, YTP, memoria, listos, bloq, tempProceso->getPID(), -2);
            }
        }
        imprimeBloq(bloq, *contG);
        if ((i == Q-tempProceso->getQt()) && (tempProceso->getTEM() != tempProceso->getTTr())) return 1;
        if (kbhit()) {
            int tecla=getch();
            control(tecla, MOD);
            if (tecla == 'e' || tecla == 'E') {tempProceso->setFail(); break;}
            if ((tecla == 'i' || tecla == 'I') && (tempProceso->getTEM() != tempProceso->getTTr())) return 2;
            if ((tecla == 'b' || tecla == 'B') && (tempProceso->getTEM() != tempProceso->getTTr())) {tempProceso->setQt(i); return 3;}
            if ((tecla == 't' || tecla == 'T') && (tempProceso->getTEM() != tempProceso->getTTr())) {tempProceso->setQt(i); return 4;}
            if (tecla == 'n' || tecla == 'N') {
                nuevos->push_b(generaProceso(nuevos, 1+proNum+nuevos->getTamanio()+listos->getTamanio()+bloq->getTamanio(), *contG));
                admision(Q, memoria, nuevos, listos, bloq, fin, contG, MOD, proNum);
                muestraMarcos(XTP, YTP, memoria, listos, bloq, tempProceso->getPID(), -2);
            }
        }
    }
    YPF=6+proNum+7*pos;
    if (proNum > 20 && (proNum+1)%23 == 1) drawCE(pos);
    /* Generar resultado y mostrar */
    tempProceso->setEstado(4, *contG);
    imprimeProcFin(tempProceso, YPF);
    gotoxy(XPF+20, YPF); generaRes(tempProceso);
    return 0;
}
void ejecucion(listaDL <Proceso> *nuevos, int Q){
    listaDL <Proceso> listos, bloq, final;
    listaDL <marco> memoria;
    Proceso tempProceso;
    int proNum=0, contGlobal=0, didIn, MOD=2;
    if (1){
        marco temp; temp.pID=0; temp.taken=0;
        for (int i=0; i < MEM/MSP; i++) {
            if (i >=MEM/MSP-4) {temp.pID=-1; temp.taken=5;}
            memoria.push_b(temp);
        }
    }
    drawIU(Q);
    while(!nuevos->vacia() || !bloq.vacia()){
        admision(Q, &memoria, nuevos, &listos, &bloq, &final, &contGlobal, &MOD, proNum);
        while (!listos.vacia()) {
            didIn=0;
            gotoxy(21, 1); cout << nuevos->getTamanio() << " ";
            gotoxy(XTP+53, YTP+1); cout << getMarcosDisp(&memoria) << " ";
            tempProceso=listos.peek_p(0);
            tempProceso.setEstado(2, contGlobal);
            muestraMarcos(XTP, YTP, &memoria, &listos, &bloq, tempProceso.getPID(), -2);
            didIn=ejecutaProceso(Q, &memoria, nuevos, &listos, &bloq, &final, &tempProceso, proNum++, &contGlobal, &MOD);
            listos.pop_f();

            if (didIn != 0) {
                switch (didIn){
                    case 1:{ /* Tiempo en CPU agotado */
                        proNum--;
                        tempProceso.setEstado(1, contGlobal);
                        listos.push_b(tempProceso);
                        break;
                    }
                    case 2:{ /* Interrupcion de E/S */
                        tempProceso.setEstado(3, contGlobal);
                        proNum--;
                        bloq.push_b(tempProceso);
                        imprimeBloq(&bloq, contGlobal);
                        break;
                    }
                    case 3:{ /* Interrupcion por BCP */
                        Proceso temp2; int y, pos;
                        proNum--;
                        muestraBCP(nuevos, &listos, &bloq, &final, contGlobal, tempProceso, 0);
                        listos.push_f(tempProceso);
                        drawIU(Q);
                        for (int i=0; i < final.getTamanio(); i++) {
                            temp2=final.peek_p(i);
                            pos=(i)/23; y=6+i+7*pos;
                            if (i > 20 && (i+1)%23 == 1) drawCE(pos);
                            imprimeProcFin(&temp2, y);
                            gotoxy(XPF+20, y); generaRes(&temp2);
                        }
                        imprimeBloq(&bloq, contGlobal);
                        break;
                    }
                    case 4:{ /* Interrupcion por Paginacion */
                        Proceso temp2; int y, pos;
                        proNum--;
                        muestraTP(&memoria, &listos, &bloq, tempProceso);
                        listos.push_f(tempProceso);
                        drawIU(Q);
                        for (int i=0; i < final.getTamanio(); i++) {
                            temp2=final.peek_p(i);
                            pos=(i)/23; y=6+i+7*pos;
                            if (i > 20 && (i+1)%23 == 1) drawCE(pos);
                            imprimeProcFin(&temp2, y);
                            gotoxy(XPF+20, y); generaRes(&temp2);
                        }
                        imprimeBloq(&bloq, contGlobal);
                        break;
                    }
                }
            }
            else {
              final.push_b(tempProceso);
              borrarMarco(&memoria, tempProceso.getPID());
            }

            admision(Q, &memoria, nuevos, &listos, &bloq, &final, &contGlobal, &MOD, proNum);
            for (int i=YIN+1; i < YIN+7; i++) {gotoxy(XPE+14, i); cout << "            ";}
            imprimeListos(&listos);
            gotoxy(0,0);
        }
    }
    Proceso temp2; temp2.setPID(-1);
    muestraBCP(nuevos, &listos, &bloq, &final, contGlobal, temp2, 1);
    system("pause");
}
int main(){
    listaDL <Proceso> nuevos; int pt;
    system("cls"); Conejito(1, 26); Gatito(111, 25);
    Ventanas(0, 0, 121, 30, 1, "Paginacion simple ");
    gotoxy(2, 1); cout << "Programa 7";

    gotoxy(3, 4); cout << "Ingresa el numero de procesos a ejecutar:";
    gotoxy(3, 5); cout << "->"; pt=intCheck(3, 5, 3, 1, 9999, "**Rango 1 - 9999");
    for (int i=0; i < pt; i++) {nuevos.push_b(generaProceso(&nuevos, i+1, 0));}
    gotoxy(3, 7); cout << "Ingresa el Quantum:";
    gotoxy(3, 8); cout << "->"; pt=intCheck(3, 8, 3, 1, 99, "**Rango 1 - 99");

    system("mode con: cols=120");
    ejecucion(&nuevos, pt);
    return 0;
}
