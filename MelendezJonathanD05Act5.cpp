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
    Proceso(){pID=0; TEM=0; nombre="";}
    void setProceso(int a, int b, operando d){
        pID=a; TEM=b; TTr=0;
        operacion.op=d.op;
        operacion.op1=d.op1;
        operacion.op2=d.op2;
    }
    void setTTr(int a){TTr=a;}
    string getNombre(){return nombre;}
    int getPID(){return pID;}
    int getTEM(){return TEM;}
    int getTTr(){return TTr;}
    int getOps(int a){
        switch (a) {
            case 1:{return operacion.op1;}
            case 2:{return operacion.op2;}
        }
        return 0;
    }
    string getOp(){return operacion.op;}
    void getOpS(){printf("%.1f", operacion.op1);cout<<operacion.op;printf("%.1f", operacion.op2);}
private:
    int pID, TEM, TTr;
    string nombre;
    operando operacion;
};

//==============================================================================

#define VEL 1000 /* Velocidad de ejecucion */
#define XLE 6   /* x Lote en Ejecucion */
#define XPE 30  /* x Proceso en Ejecucion */
#define XPF 68  /* x Proceso Finalizado */

void drawIU(){
    system("cls");
    Ventanas(0, 0, 121, 30, 1, "Ejecutando");
    gotoxy(XLE-1, 5); cout << "Lote en ejecucion:";
    gotoxy(XLE, 6); cout << "pID    TEM    TTr";
    gotoxy(XPE, 5); cout << "Proceso en ejecucion:";
    gotoxy(XPE, 6); cout << "        pID | ";
    gotoxy(XPE, 7); cout << "  Operacion | ";
    gotoxy(XPE, 8); cout << "        TEM | ";
    gotoxy(XPE, 9); cout << "Trancurrido | ";
    gotoxy(XPE, 10); cout << "   Restante | ";

    Ventanas(XPF-4, 2, 121-XPF+3, 28, 0, "");
    gotoxy(119, 2); printf("%c", 180);
    gotoxy(XPF-4, 2); printf("%c", 194);
    gotoxy(XPF-4, 29); printf("%c", 193);
    gotoxy(XPF, 4); cout << "Procesos finalizados:";
    gotoxy(XPF, 5); cout << "#L   pID    Operacion       Resultado";

    gotoxy(2, 1); cout << "Lotes en espera ->";
    gotoxy(2, 25); cout << "Tiempo en ejecucion: ";
}
void drawCE(int pos){
    string titulo;
    int x=(pos%2)? 8 : 64, y=30+((pos-1)/2)*30;
    titulo="Lotes "+to_string((pos*4)+1)+" a "+to_string((pos*4)+4);
    if (titulo.length()%2) titulo=titulo+" ";
    Ventanas(x, y, 56, 30, 1, titulo);
    gotoxy(x+4, y+4); cout << "Procesos finalizados:";
    gotoxy(x+4, y+5); cout << "#L   pID    Operacion       Resultado";
}
Proceso generaProceso(listaDL <Proceso> *procesos, int id){
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
    return tempProceso;
}
void generaRes(float op1, float op2, string ops){
    float res;
    switch (ops[0]) {
        case '%':{int x=op1, y=op2; res=x%y; break;}
        case '+':{res=op1+op2; break;}
        case '-':{res=op1-op2; break;}
        case '*':{res=op1*op2; break;}
        case '/':{res=op1/op2; break;}
        case '^':{res=pow(op1, op2); break;}
    }
    printf("%.2f", res);
}
void ejecutaProceso(Proceso *tempProceso, int loteNum, int proNum, int *contG, int *didIn, int *MOD){
    int x2=14, XP=XPF, YPF, tecla, ok=1, pos=(loteNum-1)/4;

    gotoxy(XPE+x2, 6); cout << tempProceso->getPID();
    gotoxy(XPE+x2, 7); tempProceso->getOpS();
    gotoxy(XPE+x2, 8); cout << tempProceso->getTEM();
    for (int i=0; i <= tempProceso->getTEM()-tempProceso->getTTr(); i++) {
        gotoxy(23, 25); cout << *contG;
        gotoxy(2, 28); cout << "Vel: " << VEL/ *MOD << "ms   ";
        gotoxy(XPE+x2, 9); cout << tempProceso->getTTr()+i;
        gotoxy(XPE+x2, 10); cout << tempProceso->getTEM()-tempProceso->getTTr()-i << "  ";
        Sleep(VEL/ *MOD); *contG=*contG+1;
        if (kbhit()) {
            tecla=getch();
            if (tecla == '-') if (*MOD/2 > 0) *MOD=*MOD/2;
            if (tecla == '+') if (*MOD*2 < VEL) *MOD=*MOD*2;
            if (tecla == '*') *MOD = (VEL/ *MOD < VEL) ? *MOD=1 : *MOD=VEL*2;

            if((tecla == 'i' || tecla == 'I') && proNum != 4  && (tempProceso->getTEM() != tempProceso->getTTr()+i)) {
                tempProceso->setTTr(i+tempProceso->getTTr());
                *didIn=1; return;
            }
            if(tecla == 'e' || tecla == 'E') {ok=0; break;}
            if(tecla == 'p' || tecla == 'P') {
                gotoxy(2, 27); cout << "En Pausa";
                while(1){
                    tecla=getch();
                    if(tecla == 'c' || tecla == 'C') {gotoxy(2, 27); cout << "         "; break;}
                }
            }
        }
    }
    for (int i=6; i < 12; i++) {gotoxy(XPE+x2, i); cout << "                    ";}

    YPF=6+proNum+(loteNum-1)*6+6*pos-((pos)/2)*30;
    if (loteNum > 4) {
        XP = (pos%2) ? 12 : 68;
        if (loteNum%4 == 1) drawCE(pos);
    }
    /* Generar resultado y mostrar*/
    gotoxy(XP, YPF); cout << loteNum;
    gotoxy(XP+5, YPF); cout << tempProceso->getPID();
    gotoxy(XP+12, YPF); tempProceso->getOpS();
    if (ok) {gotoxy(XP+28, YPF); generaRes(tempProceso->getOps(1), tempProceso->getOps(2), tempProceso->getOp());}
    else {gotoxy(XP+28, YPF); cout << "ERROR";}
}
int main(){
    listaDL <Proceso> procesos, lote;
    Proceso tempProceso;
    int salir=0, pt, loteNum=1, proNum=0, contGlobal=0, didIn, MOD=1;

    system("cls");
    Ventanas(0, 0, 121, 30, 1, "Procesamiento por lotes con multiprogramacion ");
    gotoxy(2, 1); cout << "Programa 2";
    gotoxy(3, 4); cout << "Ingresa el numero de procesos a ejecutar:";
    gotoxy(3, 5); cout << "->"; pt=intCheck(3, 5, 3, 0, "**Debe ser mayor a 0");
    drawIU();
    for (int i=0; i < pt; i++) {procesos.push_b(generaProceso(&procesos, i+1));}
    while(!procesos.vacia()){
        gotoxy(0,0); gotoxy(21, 1);
        if (procesos.getTamanio()%5) cout << (procesos.getTamanio()/5) << " ";
        else cout << (procesos.getTamanio()/5)-1 << " ";
        for (int i=0; i < 5; i++) {
            if (!procesos.vacia()) {
                tempProceso=procesos.peek_p(0);
                procesos.pop_f();
                gotoxy(XLE, 7+i); cout << tempProceso.getPID();
                gotoxy(XLE+7, 7+i); cout << tempProceso.getTEM();
                gotoxy(XLE+14, 7+i); cout << tempProceso.getTTr();
                lote.push_b(tempProceso);
            }
        }
        while (!lote.vacia()) {
            didIn=0;
            tempProceso=lote.peek_p(0);
            gotoxy(XLE, 7+proNum); cout << "                    ";
            ejecutaProceso(&tempProceso, loteNum, proNum++, &contGlobal, &didIn, &MOD);
            lote.pop_f();
            if (didIn){
                proNum--;
                lote.push_b(tempProceso);
                for (int i=0; i < 5; i++) {gotoxy(XLE, 7+i); cout << "                      ";}
                for (int i=6; i < 12; i++) {gotoxy(XPE+14, i); cout << "                    ";}
                for (int i=0; i < lote.getTamanio(); i++) {
                    tempProceso=lote.peek_p(i);
                    gotoxy(XLE, 7+i+proNum); cout << tempProceso.getPID();
                    gotoxy(XLE+7, 7+i+proNum); cout << tempProceso.getTEM();
                    gotoxy(XLE+14, 7+i+proNum); cout << tempProceso.getTTr();
                }
            }
            else if (proNum == 5) {proNum=0; loteNum++;}
            gotoxy(0,0);
        }
    }
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
