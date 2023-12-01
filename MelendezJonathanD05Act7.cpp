#include <iostream>
#include <conio.h>      /* getch() y kbhit() */
#include <windows.h>    /* Sleep() y gotoxy() */
#include <limits>       /* cin.good */
#include <ctime>        /* time() para srand() */
#include <cmath>
#include "classListaDL.h"
using namespace std;

void gotoxy(short int x, short int y);
void Ventanas(int x, int y, int ancho, int largo, int linea, string titulo);
int intCheck(int, int, int, int, string);
int getRandomNum(int, int);

class Proceso{
public:
    struct operando{
        float op1, op2;
        string op;
    };
    Proceso(){didF=0; isAt=0; TCr=0; TLl=-1; TEj=0; TBl=0; TFi=0; TTr=0;}
    void setProceso(int a, int b, operando d){
        pID=a; TEM=b;
        operacion.op=d.op;
        operacion.op1=d.op1;
        operacion.op2=d.op2;
    }
    void setEstado(int a, int contG){
        state=a;
        switch (state) {
            case 0:{TCr=contG; break;} /* Nuevo */
            case 1:{if (TLl == -1) TLl=contG; break;} /* Listo */
            case 2:{if (!isAt) {isAt=TRUE; TEj=contG;} break;} /* Ejecucion */
            case 3:{TBl=contG; break;} /* Bloqueado */
            case 4:{TFi=contG; break;} /* Finalizado */
        }
    }
    void setTTr(int a){TTr=a;}
    void setFail(int a){didF=a;}

    int getPID(){return pID;}
    int getEstado(){return state;}
    int didFail(){return didF;}
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
    int pID, state;
    int TEM, TCr, TLl, TFi, TTr, TEj, TBl;
    operando operacion;
    bool isAt, didF;
};

//==============================================================================

#define VEL 1000    /* Velocidad de ejecucion */
#define XLE 3       /* x Lote en Ejecucion */
#define XPE 25      /* x Proceso en Ejecucion */
#define XPF 54      /* x Proceso Finalizado */

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
void imprimeBCP(Proceso *tempProceso, int YPF){
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
void control(int tecla, int *MOD){
    if (tecla == '-') if (*MOD/2 > 0) *MOD=*MOD/2;
    if (tecla == '+') if (*MOD*2 < VEL) *MOD=*MOD*2;
    if (tecla == '*') *MOD = (VEL/ *MOD < VEL) ? *MOD=1 : *MOD=VEL*2;
    if (tecla == 'p' || tecla == 'P') {
        gotoxy(2, 27); cout << "En Pausa";
        while(1){
            tecla=getch();
            if(tecla == 'c' || tecla == 'C') {gotoxy(2, 27); cout << "         "; break;}
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
            if (operacion.op2 > 5 && operacion.op == "^") {
                operacion.op1=getRandomNum(12, id*2)+1;
                operacion.op2=getRandomNum(5, id*3);
            }
            else ok=1;
        }
    }
    tempProceso.setProceso(id, getRandomNum(12, id)+6, operacion);
    tempProceso.setEstado(0, contG);
    return tempProceso;
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
void ejecutaProceso(listaDL <Proceso> *listos, listaDL <Proceso> *bloq, Proceso *tempProceso, int proNum, int *contG, int *didIn, int *MOD){
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
                listos->push_b(bloq->peek_p(i));
                bloq->pop_f();
                imprimeListos(listos);
            }
        }
        imprimeBloq(bloq, *contG);
        if (kbhit()) {
            int tecla=getch();
            control(tecla, MOD);
            if (tecla == 'e' || tecla == 'E') {tempProceso->setFail(1); break;}
            if ((tecla == 'i' || tecla == 'I') && (tempProceso->getTEM() != tempProceso->getTTr())) {
                *didIn=1; return;
            }
        }
    }

    YPF=6+proNum+7*pos;
    if (proNum > 20 && proNum%22 == 1) drawCE(pos);
    /* Generar resultado y mostrar*/
    tempProceso->setEstado(4, *contG);
    imprimeBCP(tempProceso, YPF);
    gotoxy(XPF+20, YPF); generaRes(tempProceso);
}
void gestionProcesos(listaDL <Proceso> *nuevos){
    listaDL <Proceso> listos, bloq, final;
    Proceso tempProceso;
    int proNum=0, contGlobal=0, didIn, MOD=1;
    while(!nuevos->vacia() || !bloq.vacia()){
        /* Proceso nulo o admitir mas procesos */
        if (listos.vacia() && !bloq.vacia()) {
            for (int i=0; i < bloq.getTamanio(); i++) {
                if (contGlobal-(bloq.peek_p(0).getTBl()) >= 8){
                    tempProceso=bloq.peek_p(i);
                    tempProceso.setEstado(1, contGlobal);
                    listos.push_b(tempProceso);
                    bloq.pop_f();
                    imprimeListos(&listos);
                }
            }
            imprimeBloq(&bloq, contGlobal);
            gotoxy(XPE+14, 6); cout << "NULL";
            for (int i=7; i < 11; i++) {gotoxy(XPE+14, i); cout << "x";}
            gotoxy(23, 25); cout << contGlobal;
            Sleep(VEL/MOD); contGlobal++;
            if (kbhit()) {
                char tecla=getch();
                control(tecla, &MOD);
            }
        }
        else {
            for (int i=0; i < 5; i++) {
                if (!nuevos->vacia()) {
                    tempProceso=nuevos->peek_p(0);
                    nuevos->pop_f();
                    gotoxy(XLE, 7+i); cout << tempProceso.getPID();
                    gotoxy(XLE+7, 7+i); cout << tempProceso.getTEM();
                    gotoxy(XLE+14, 7+i); cout << tempProceso.getTTr();
                    tempProceso.setEstado(1, contGlobal);
                    listos.push_b(tempProceso);
                }
            }
        }

        while (!listos.vacia()) {
            didIn=0;
            gotoxy(21, 1); cout << nuevos->getTamanio() << " ";
            tempProceso=listos.peek_p(0);
            tempProceso.setEstado(2, contGlobal);
            ejecutaProceso(&listos, &bloq, &tempProceso, proNum++, &contGlobal, &didIn, &MOD);
            listos.pop_f();

            if (didIn) {
                tempProceso.setEstado(3, contGlobal);
                proNum--;
                bloq.push_b(tempProceso);
                imprimeBloq(&bloq, contGlobal);
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
    Ventanas(0, 0, 121, 30, 1, "Algoritmo de planificacion FCFS ");
    gotoxy(2, 1); cout << "Programa 3";
    gotoxy(3, 4); cout << "Ingresa el numero de procesos a ejecutar:";
    gotoxy(3, 5); cout << "->"; pt=intCheck(3, 5, 3, 0, "**Debe ser mayor a 0");
    drawIU();
    for (int i=0; i < pt; i++) {nuevos.push_b(generaProceso(&nuevos, i+1, 0));}
    gestionProcesos(&nuevos);
    gotoxy(2, 27); cout << "**Ejecucion finalizada**";
    gotoxy(2, 28); system("pause"); return 0;
}

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
