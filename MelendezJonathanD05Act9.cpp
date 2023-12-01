#include <iostream>
#include <conio.h>      /* getch() y kbhit() */
#include <windows.h>    /* Sleep() y gotoxy() */
#include <limits>       /* cin.good */
#include <ctime>        /* time() para srand() */
#include <cmath>
using namespace std;

class Proceso{
public:
    struct operando{
        float op1, op2;
        string op;
    };
    Proceso(){didF=FALSE; isAt=FALSE; TCr=0; TLl=-1; TEj=0; TBl=0; TFi=0; TTr=0;}
    void setProceso(int a, int b, operando d){
        pID=a; TEM=b;
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

    int getPID(){return pID;}
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
    int pID, state;
    int TEM, TCr, TLl, TEj, TBl, TFi, TTr;
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
};

//==============================================================================

#define VEL 1000 /* Velocidad de ejecucion */
#define XLE 3    /* x Lote en Ejecucion */
#define XPE 25   /* x Proceso en Ejecucion */
#define XPF 54   /* x Proceso Finalizado */

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
int intCheck(int x, int y, int pos, int limit, string warn){
    int aux;
    while(1){
        gotoxy(x+pos, y); cout << "                                                  ";
        gotoxy(x, y+2); cout << "                                                  ";
        gotoxy(x+pos, y); cin >> aux;
        if(!cin.good()){cin.clear(); cin.ignore(numeric_limits<streamsize>::max(),'\n'); aux=-1;}
        if(aux > limit) break;
        gotoxy(x, y+2); cout << warn; Sleep(1000);
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
void drawIU(){
    system("cls");
    Ventanas(0, 0, 121, 30, 1, "Ejecutando");
    gotoxy(XLE-1, 5); cout << "Procesos listos:";
    gotoxy(XLE, 6); cout << "pID    TEM    TTr";
    gotoxy(XLE-1, 14); cout << "Procesos Bloqueados:";
    gotoxy(XLE, 15); cout << "pID    TBloq";
    gotoxy(XPE, 5); cout << "Proceso en ejecucion:";
    gotoxy(XPE, 6); cout << "        pID | ";
    gotoxy(XPE, 7); cout << "  Operacion | ";
    gotoxy(XPE, 8); cout << "        TEM | ";
    gotoxy(XPE, 9); cout << "Trancurrido | ";
    gotoxy(XPE, 10); cout << "   Restante | ";

    Ventanas(XPF-3, 2, 121-XPF+3, 28, 0, " ");
    gotoxy(119, 2); printf("%c", 180);
    gotoxy(XPF-3, 2); printf("%c", 194);
    gotoxy(XPF-3, 29); printf("%c", 193);
    gotoxy(XPF, 4); cout << "Procesos finalizados:";
    gotoxy(XPF, 5); cout << "pID    Operacion    Resultado      TL   TF   TE   TS   TRt  TRs";

    gotoxy(2, 1); cout << "Procesos nuevos ->";
    gotoxy(2, 25); cout << "Tiempo en ejecucion: ";
}
void drawCE(int pos){
    string titulo;
    int y=30+(pos-1)*30;
    titulo="Procesos "+to_string((pos*23)+1)+" a "+to_string(23+pos*23);
    if (!(titulo.length()%2)) titulo=titulo+" ";
    Ventanas(XPF-3, y, 121-XPF+3, 30, 1, titulo);
    gotoxy(XPF, y+4); cout << "Procesos finalizados:";
    gotoxy(XPF, y+5); cout << "pID    Operacion    Resultado      TL   TF   TE   TS   TRt  TRs";
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
    else cout << "ERROR";
}
void imprimeListos(listaDL <Proceso> *listos){
    for (int i=1; i < listos->getTamanio(); i++) {
        gotoxy(XLE, 7+i); cout << "                ";
        gotoxy(XLE, 7+i); cout << listos->peek_p(i).getPID();
        gotoxy(XLE+7, 7+i); cout << listos->peek_p(i).getTEM();
        gotoxy(XLE+14, 7+i); cout << listos->peek_p(i).getTTr();
    }
    for (int i=listos->getTamanio(); i < 5; i++) {gotoxy(XLE, 7+i); cout << "                ";}
}
void imprimeBloq(listaDL <Proceso> *bloq, int contG){
    for (int i=0; i < bloq->getTamanio(); i++) {
        gotoxy(XLE, 16+i); cout << "                ";
        gotoxy(XLE, 16+i); cout << bloq->peek_p(i).getPID();
        gotoxy(XLE+7, 16+i); cout << contG-bloq->peek_p(i).getTBl();
    }
    for (int i=bloq->getTamanio(); i < 5; i++) {gotoxy(XLE, 16+i); cout << "                ";}
}
void imprimeProcFin(Proceso *tempProceso, int YPF){
    gotoxy(XPF, YPF); cout << tempProceso->getPID();
    gotoxy(XPF+7, YPF); tempProceso->getOpS();
    gotoxy(XPF+35, YPF); cout << tempProceso->getTLl();
    gotoxy(XPF+40, YPF); cout << tempProceso->getTF();
    gotoxy(XPF+45, YPF); cout << tempProceso->getTEs();
    gotoxy(XPF+50, YPF); cout << tempProceso->getTTr();
    gotoxy(XPF+55, YPF); cout << tempProceso->getTRt();
    gotoxy(XPF+60, YPF); cout << tempProceso->getTRs();
}
void imprimeProcEjec(Proceso *tempProceso){
    for (int i=6; i < 12; i++) {gotoxy(XPE+14, i); cout << "            ";}
    gotoxy(XPE+14, 6); cout << tempProceso->getPID();
    gotoxy(XPE+14, 7); tempProceso->getOpS();
    gotoxy(XPE+14, 8); cout << tempProceso->getTEM();
    gotoxy(XPE+14, 9); cout << tempProceso->getTTr();
    gotoxy(XPE+14, 10); cout << tempProceso->getTEM();
}
void imprimeProcBCP(int x, int y, int contG, Proceso tempProceso){
    gotoxy(x+33, y); cout << "x"; gotoxy(x+66, y); cout << "-"; gotoxy(x+78, y); cout << "-";
    switch(tempProceso.getEstado()){
        case 0:{gotoxy(x+6, y); cout << "Nuevo"; break;}
        case 1:{gotoxy(x+6, y); cout << "Listo"; break;}
        case 2:{gotoxy(x+6, 6); cout << "Ejecucion"; break;}
        case 3:{gotoxy(x+6, y); cout << "Bloqueado"; break;}
        case 4:{gotoxy(x+6, y); cout << "Finalizado";
            gotoxy(x+33, y); generaRes(&tempProceso);
            gotoxy(x+66, y); cout << tempProceso.getTF();
            gotoxy(x+78, y); cout << tempProceso.getTRt();
            break;
        }
    }
    gotoxy(x, y); cout << tempProceso.getPID();
    gotoxy(x+19, y); tempProceso.getOpS();
    gotoxy(x+48, y); cout << tempProceso.getTEM();                                                          /* TEM */
    gotoxy(x+54, y); cout << tempProceso.getTTr();                                                          /* Servicio */
    if(tempProceso.getTLl() == -1) {
        gotoxy(x+60, y); cout << "-";                                                   /* Llegada */
        gotoxy(x+72, y); cout << contG - tempProceso.getTTr();                          /* Espera */
    }
    else {
        gotoxy(x+60, y); cout << tempProceso.getTLl();                                  /* Llegada */
        gotoxy(x+72, y); cout << contG - tempProceso.getTLl() - tempProceso.getTTr();   /* Espera */
    }
    gotoxy(x+84, y); if (tempProceso.getAt()) cout << tempProceso.getTRs(); else cout << "-";               /* Respuesta */
    if (tempProceso.getEstado() != 4) {gotoxy(x+90, y); cout << tempProceso.getTEM()-tempProceso.getTTr();} /* Restante */
}
void muestraBCP(listaDL <Proceso> *nuevos, listaDL <Proceso> *listos, listaDL <Proceso> *bloq, listaDL <Proceso> *fin, int contG, Proceso enEjec){
    system("cls"); int y=7, x=2;
    gotoxy(2, 4); cout << "pID   Estado       Operacion     Resultado      TEM   TS    TL    TF    TE    TRt   TRs   Restante";

    if (enEjec.getPID() != -1) imprimeProcBCP(x, 6, contG, enEjec);
    else y--;
    for(int i=0; i < listos->getTamanio(); i++) imprimeProcBCP(x, y++, contG, listos->peek_p(i));
    for(int i=0; i < bloq->getTamanio(); i++)   imprimeProcBCP(x, y++, contG, bloq->peek_p(i));
    if (nuevos->getTamanio() > 0) y++;
    for(int i=0; i < nuevos->getTamanio(); i++) imprimeProcBCP(x, y++, contG, nuevos->peek_p(i));
    if (fin->getTamanio() > 0) y++;
    for(int i=0; i < fin->getTamanio(); i++)    imprimeProcBCP(x, y++, contG, fin->peek_p(i));

    Ventanas(0, 0, 121, y+7, 1, "Tabla de procesos ");
    gotoxy(x, y+3); cout << "Tiempo en ejecucion: " << contG;
    gotoxy(x, y+4); cout << "En Pausa"; gotoxy(0, 0);
    while(1){
        int tecla=getch();
        if(tecla == 'c' || tecla == 'C') break;
    }
}
void control(int tecla, int *MOD){
    if (tecla == '-') if (*MOD/2 > 0) *MOD=*MOD/2;
    if (tecla == '+') if (*MOD*2 < VEL) *MOD=*MOD*2;
    if (tecla == '*') *MOD = (VEL/ *MOD < VEL) ? *MOD=1 : *MOD=VEL*2;
    if (tecla == 'p' || tecla == 'P') {
        gotoxy(2, 27); cout << "En Pausa";
        while(1){
            tecla=getch();
            if(tecla == 'c' || tecla == 'C') {gotoxy(2, 27); cout << "        "; break;}
        }
    }
    gotoxy(2, 28); cout << "Vel: " << VEL/ *MOD << "ms   ";
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
    tempProceso.setProceso(id, getRandomNum(12, id)+6, operacion);
    tempProceso.setEstado(0, contG);
    return tempProceso;
}
void admision(listaDL <Proceso> *nuevos, listaDL <Proceso> *listos, listaDL <Proceso> *bloq, listaDL <Proceso> *fin, int *contG, int *MOD, int proNum){
    Proceso tempProceso;
    if (listos->vacia() && !bloq->vacia()) {
        gotoxy(XPE+14, 6); cout << "NULL";
        for (int i=7; i < 11; i++) {gotoxy(XPE+14, i); cout << "x";}

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

        gotoxy(23, 25); cout << *contG;
        Sleep(VEL/ *MOD); *contG+=1;
        if (kbhit()) {
            char tecla=getch();
            control(tecla, MOD);
            if (tecla == 'n' || tecla == 'N') {
                nuevos->push_b(generaProceso(nuevos, 1+proNum+nuevos->getTamanio()+listos->getTamanio()+bloq->getTamanio(), *contG));
                if (listos->vacia() && bloq->getTamanio() < 5){
                    tempProceso=nuevos->peek_p(0);
                    nuevos->pop_f();
                    tempProceso.setEstado(1, *contG);
                    listos->push_b(tempProceso);
                }
                imprimeListos(listos);
            }
            if (tecla == 'b' || tecla == 'B') {
                Proceso temp2; temp2.setPID(-1);
                muestraBCP(nuevos, listos, bloq, fin, *contG, temp2);
                drawIU();
                for (int i=0; i < fin->getTamanio(); i++) {
                    temp2=fin->peek_p(i);
                    imprimeProcFin(&temp2, 6+i);
                    gotoxy(XPF+20, 6+i); generaRes(&temp2);
                }
            }
        }
    }
    else {
        for (int i=0; i < 5; i++) {
            if (!nuevos->vacia() && (bloq->getTamanio()+listos->getTamanio() < 5)) {
                tempProceso=nuevos->peek_p(0);
                nuevos->pop_f();
                gotoxy(XLE, 7+i); cout << tempProceso.getPID();
                gotoxy(XLE+7, 7+i); cout << tempProceso.getTEM();
                gotoxy(XLE+14, 7+i); cout << tempProceso.getTTr();
                tempProceso.setEstado(1, *contG);
                listos->push_b(tempProceso);
            }
        }
    }
    gotoxy(21, 1); cout << nuevos->getTamanio() << " ";
    gotoxy(XLE, 7); cout << "                    ";
}
int ejecutaProceso(listaDL <Proceso> *nuevos, listaDL <Proceso> *listos, listaDL <Proceso> *bloq, listaDL <Proceso> *fin, Proceso *tempProceso, int proNum, int *contG, int *MOD){
    int YPF, pos=(proNum)/23, restante=tempProceso->getTEM()-tempProceso->getTTr();
    gotoxy(XLE, 7); cout << "                    ";
    imprimeProcEjec(tempProceso);
    gotoxy(2, 28); cout << "Vel: " << VEL/ *MOD << "ms   ";
    for (int i=1; i <= restante; i++) {
        gotoxy(23, 25); cout << *contG;
        Sleep(VEL/ *MOD); *contG+=1;
        tempProceso->setTTr(tempProceso->getTTr()+1);
        gotoxy(XPE+14, 9); cout << tempProceso->getTTr();
        gotoxy(XPE+14, 10); cout << tempProceso->getTEM()-tempProceso->getTTr() << "  ";
        for (int i=0; i < bloq->getTamanio(); i++) {
            if (*contG-(bloq->peek_p(i).getTBl()) >= 8) {
                Proceso temp2=bloq->peek_p(i);
                temp2.setEstado(1, *contG);
                listos->push_b(temp2);
                bloq->pop_f();
                imprimeListos(listos);
            }
        }
        imprimeBloq(bloq, *contG);
        if (kbhit()) {
            int tecla=getch();
            control(tecla, MOD);
            if (tecla == 'e' || tecla == 'E') {tempProceso->setFail(); break;}
            if ((tecla == 'i' || tecla == 'I') && (tempProceso->getTEM() != tempProceso->getTTr())) return 1;
            if ((tecla == 'b' || tecla == 'B') && (tempProceso->getTEM() != tempProceso->getTTr())) return 2;
            if (tecla == 'n' || tecla == 'N') {
                nuevos->push_b(generaProceso(nuevos, 1+proNum+nuevos->getTamanio()+listos->getTamanio()+bloq->getTamanio(), *contG));
                admision(nuevos, listos, bloq, fin, contG, MOD, proNum);
                imprimeListos(listos);
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
void ejecucion(listaDL <Proceso> *nuevos){
    listaDL <Proceso> listos, bloq, final;
    Proceso tempProceso;
    int proNum=0, contGlobal=0, didIn, MOD=1;
    drawIU();
    while(!nuevos->vacia() || !bloq.vacia()){
        admision(nuevos, &listos, &bloq, &final, &contGlobal, &MOD, proNum);
        while (!listos.vacia()) {
            didIn=0;
            gotoxy(21, 1); cout << nuevos->getTamanio() << " ";
            tempProceso=listos.peek_p(0);
            tempProceso.setEstado(2, contGlobal);
            didIn=ejecutaProceso(nuevos, &listos, &bloq, &final, &tempProceso, proNum++, &contGlobal, &MOD);
            listos.pop_f();

            if (didIn != 0) {
                switch (didIn){
                    case 1:{ /* Interrupcion de E/S */
                        tempProceso.setEstado(3, contGlobal);
                        proNum--;
                        bloq.push_b(tempProceso);
                        imprimeBloq(&bloq, contGlobal);
                        break;
                    }
                    case 2:{ /* Interrupcion por BCP */
                        Proceso temp2; int y, pos;
                        proNum--;
                        muestraBCP(nuevos, &listos, &bloq, &final, contGlobal, tempProceso);
                        listos.push_f(tempProceso);
                        drawIU();
                        for (int i=0; i < final.getTamanio(); i++) {
                            temp2=final.peek_p(i);
                            pos=(i)/23; y=6+i+7*pos;
                            if (i > 20 && (i+1)%23 == 1) drawCE(pos);
                            imprimeProcFin(&temp2, y);
                            gotoxy(XPF+20, y); generaRes(&temp2);
                        }
                        break;
                    }
                }
            }
            else final.push_b(tempProceso);

            if (!nuevos->vacia() && (listos.getTamanio() + bloq.getTamanio()) < 5) {
                tempProceso=nuevos->peek_p(0);
                tempProceso.setEstado(1, contGlobal);
                listos.push_b(tempProceso);
                nuevos->pop_f();
            }
            for (int i=6; i < 12; i++) {gotoxy(XPE+14, i); cout << "            ";}
            imprimeListos(&listos);
            gotoxy(0,0);
        }
    }
}
int main(){
    listaDL <Proceso> nuevos; int pt;
    system("cls"); Conejito(1, 26); Gatito(111, 25);
    Ventanas(0, 0, 121, 30, 1, "Algoritmo de planificacion FCFS parte 2 ");
    gotoxy(2, 1); cout << "Programa 4";
    gotoxy(3, 4); cout << "Ingresa el numero de procesos a ejecutar:";
    gotoxy(3, 5); cout << "->"; pt=intCheck(3, 5, 3, 0, "**Debe ser mayor a 0");
    for (int i=0; i < pt; i++) {nuevos.push_b(generaProceso(&nuevos, i+1, 0));}
    ejecucion(&nuevos);
    gotoxy(2, 27); cout << "**Ejecucion finalizada**";
    gotoxy(2, 28); system("pause"); return 0;
}
