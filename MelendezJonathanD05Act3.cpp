#include <iostream>
#include <sstream>
#include <windows.h>
#include <string>
#include <limits>
#include "classListaDL.h"

using namespace std;

class Proceso{
public:
    struct operando{
        float op1, op2;
        string op;
    };

    Proceso(){pID=0; TEM=0; nombre="";}
    void setProceso(int a, int b, string c, operando d){
        pID=a;
        TEM=b;
        nombre=c;
        operacion.op=d.op;
        operacion.op1=d.op1;
        operacion.op2=d.op2;
    }

    string getNombre(){return nombre;}
    int getPID(){return pID;}
    int getTEM(){return TEM;}
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
    int pID, TEM;
    string nombre;
    operando operacion;
};

void gotoxy(short int x, short int y);
void Ventanas(int x, int y, int ancho, int largo, int linea, string titulo);
int intCheck(int, int, int, int, string);
string stringCheck(int, int, int, string);

//==================================================================================

#define VEL 750
#define XLE 6
#define XPE 30
#define XPF 68

void drawIU(){
    system("cls");
    Ventanas(0, 0, 121, 30, 1, "Ejecutando");
    Ventanas(XPF-4, 2, 121-XPF+3, 28, 0, "");
    gotoxy(119, 2); printf("%c", 180);
    gotoxy(XPF-4, 2); printf("%c", 194);
    gotoxy(XPF-4, 29); printf("%c", 193);

    gotoxy(2, 1); cout << "Lotes en espera ->";
    gotoxy(2, 25); cout << "Tiempo en ejecucion: ";
    gotoxy(XLE-1, 5); cout << "Lote en ejecucion:";
    gotoxy(XLE, 6); cout << "pID          TEM";

    gotoxy(XPE, 5); cout << "Proceso en ejecucion:";
    gotoxy(XPE, 6); cout << "     Nombre | ";
    gotoxy(XPE, 7); cout << "  Operacion | ";
    gotoxy(XPE, 8); cout << "        pID | ";
    gotoxy(XPE, 9); cout << "        TEM | ";
    gotoxy(XPE, 10); cout << "Trancurrido | ";
    gotoxy(XPE, 11); cout << "   Restante | ";

    gotoxy(XPF, 5); cout << "Procesos finalizados:";
    gotoxy(XPF, 6); cout << "#L  pID   Operacion         Resultado";
}
Proceso capturaProceso(listaDL <Proceso> *procesos, int *salir, int *pt){
    int x=3, y=3, ok, id, tem;
    string nombre, op1, op2, tempOp;
    char op=' ';
    Proceso::operando operacion;
    Proceso tempProceso;

    gotoxy(x, y+1); cout << "Ingresa el nombre: [0] Terminar captura";
    gotoxy(x, y+2); cout << "->";
    while (1) {
        nombre=stringCheck(x, y+2, 3, "**ERROR. No puede estar vacio");
        if (nombre == "0") {*salir=1; break;}
        else break;
    }
    if (!*salir) {
        gotoxy(x, y+4); cout << "Ingresa el ID:";
        gotoxy(x, y+5); cout << "->";
        ok=0;
        while (!ok) {
            id=intCheck(x, y+5, 3, 0, "El ID debe ser mayor a 0");
            ok=1;
            for (int i=0; i < procesos->getTamanio(); i++) {
                if (procesos->peek_p(i).getPID() == id) {
                    ok=0;
                    gotoxy(x, y+7); cout << "El ID ya esta registrado";
                    Sleep(500);
                    gotoxy(x+3, y+5); cout << "             ";
                    gotoxy(x, y+7); cout << "                        ";
                    break;
                }
            }
        }

        gotoxy(x, y+7); cout << "Ingresa el tiempo estimado maximo:";
        gotoxy(x, y+8); cout << "->";
        tem=intCheck(x, y+8, 3, 0, "Debe ser mayor a 0");

        gotoxy(x, y+10); cout << "Ingresa la operacion: ";
        ok=0;
        while (!ok) {
            gotoxy(x+22, y+10); fflush(stdin); getline(cin, tempOp);
            for (int i=0; i < tempOp.length(); i++) {
                if (tempOp[i] == 37 || tempOp[i] == 42 || tempOp[i] == 43 || tempOp[i] == 45 || tempOp[i] == 47) {
                    op=tempOp[i]; break;
                }
            }
            if (op == ' ' || tempOp.length() < 3) {
                gotoxy(x, y+12); cout << "**Ingresa una operacion valida";
                ok=0; Sleep(VEL);
            }
            else{
                stringstream input_stringstream(tempOp);
                getline(input_stringstream, op1, op);
                getline(input_stringstream, op2, op);
                try{
                    operacion.op1=stof(op1);
                    operacion.op2=stof(op2);
                    operacion.op=op;
                    if (operacion.op2 == 0 && operacion.op == "/") {
                        gotoxy(x, y+12); cout << "**No esta permitido dividir por 0";
                        ok=0; Sleep(VEL);
                    }
                    else ok=1;
                }
                catch(...){
                    gotoxy(x, y+12); cout << "**Ingresa una operacion valida";
                    ok=0; Sleep(VEL);
                }
            }
            gotoxy(x+22, y+10); cout << "                                 ";
            gotoxy(x, y+12); cout << "                                 ";
        }
        tempProceso.setProceso(id, tem, nombre, operacion);
        *pt=*pt+1;
        for (int i=5; i < 13; i++) {gotoxy(x, y+i); cout << "                                   ";}
    }
    else{tempProceso.setProceso(0, 0, "", operacion);}
    return tempProceso;
}
void generaRes(float op1, float op2, string ops){
    float res;
    switch (ops[0]) {
        case 43:{res=op1+op2; break;}
        case 45:{res=op1-op2; break;}
        case 42:{res=op1*op2; break;}
        case 47:{res=op1/op2; break;}
        case 37:{
            int x=op1, y=op2;
            res=x%y; break;
        }
    }
    printf("%.2f", res);
}
void ejecutaProceso(Proceso *tempProceso, int loteNum, int proNum, int *cont){
    int x2=14, YPE=7+proNum+(loteNum-1)*6;
    gotoxy(XPE+x2, 6); cout << tempProceso->getNombre();
    gotoxy(XPE+x2, 7); tempProceso->getOpS();
    gotoxy(XPE+x2, 8); cout << tempProceso->getPID();
    gotoxy(XPE+x2, 9); cout << tempProceso->getTEM();
    for (int i=0; i <= tempProceso->getTEM(); i++) {
        gotoxy(23, 25); cout << *cont;
        gotoxy(XPE+x2, 10); cout << i;
        gotoxy(XPE+x2, 11); cout << tempProceso->getTEM()-i << "  ";
        Sleep(VEL);
        *cont=*cont+1;
    }
    for (int i=6; i < 12; i++) {gotoxy(XPE+x2, i); cout << "                    ";}
    /* Generar resultado y mostrar*/
    gotoxy(XPF, YPE); cout << loteNum;
    gotoxy(XPF+4, YPE); cout << tempProceso->getPID();
    gotoxy(XPF+10, YPE); tempProceso->getOpS();
    gotoxy(XPF+28, YPE); generaRes(tempProceso->getOps(1), tempProceso->getOps(2), tempProceso->getOp());
}
int main(){
    system("cls");
    Ventanas(0, 0, 121, 30, 1, "Procesamiento por lotes ");
    gotoxy(2, 1); cout << "Programa 1";

    listaDL <Proceso> procesos;
    listaDL <Proceso> lote;
    Proceso tempProceso;
    int salir=0, pt=0, loteNum=1, proNum=0, contGlobal=0;

    while(!salir){
        gotoxy(XPF, 4); cout << "Procesos capturados -> " << pt;
        tempProceso=capturaProceso(&procesos, &salir, &pt);
        gotoxy(XPF+13, 4); cout << pt;
        if (!salir) procesos.push_b(tempProceso);
    }

    drawIU();
    while(!procesos.vacia()){
        gotoxy(21, 1); cout << procesos.getTamanio()/5;
        for (int i=0; i < 5; i++) {
            if (!procesos.vacia()) {
                tempProceso=procesos.peek_p(0);
                procesos.pop_f();
                gotoxy(XLE, 7+i); cout << tempProceso.getPID();
                gotoxy(XLE+13, 7+i); cout << tempProceso.getTEM();
                lote.push_b(tempProceso);
            }
        }
        Sleep(VEL);
        while (!lote.vacia()) {
            tempProceso=lote.peek_p(0);
            gotoxy(XLE, 7+proNum); cout << "                    ";
            ejecutaProceso(&tempProceso, loteNum, proNum++, &contGlobal);
            lote.pop_f();
            if (proNum == 5) {proNum=0; loteNum++;}
        }
    }
    gotoxy(2, 27); cout << "**Ejecucion finalizada con exito**";
    gotoxy(2, 28); system("pause"); return 0;
}

void gotoxy(short int x, short int y){
    /* Mueve el cursor en consola a la posicion x, y indicada */
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

    char LH=196, LV=179, ESD=191, ESI=218, EID=217, EII=192, intD=180, intI=195;
    for(int A=0; A < ancho; A++){gotoxy(x+A, y); cout << LH; gotoxy(x+A, y+largo); cout << LH;}
    for(int L=0; L < largo; L++){gotoxy(x, y+L); cout << LV; gotoxy(x+ancho, y+L); cout << LV;}
    gotoxy(x, y); cout << ESI; gotoxy(x+ancho, y); cout << ESD;
    gotoxy(x, y+largo); cout << EII; gotoxy(x+ancho, y+largo); cout << EID;
    gotoxy(x+centro-(size/2), y+1); cout << titulo;
    if(linea==1){
        gotoxy(x,y+2); for(int xd=0; xd < ancho; xd++) cout << LH;
        gotoxy(x,y+2); cout << intI; gotoxy(x+ancho, y+2); cout << intD;
    }
}
int intCheck(int x, int y, int pos, int limit, string warn){
    int aux;
    while(1){
        gotoxy(x+pos, y); cout << "                         ";
        gotoxy(x, y+2); cout << "                         ";
        gotoxy(x+pos, y); cin >> aux;
        if(!cin.good()){cin.clear(); cin.ignore(numeric_limits<streamsize>::max(),'\n'); aux=-1;}
        if(aux > limit) break;
        gotoxy(x, y+2); cout << warn; Sleep(750);
    }
    return aux;
}
string stringCheck(int x, int y, int pos, string warn){
    string aux;
    while(1){
        aux="";
        gotoxy(x+pos, y); cout << "                                   ";
        gotoxy(x, y+2); cout << "                                   ";
        gotoxy(x+pos, y); fflush(stdin); getline(cin, aux);
        if(!(aux == "" || aux == " ")) break;
        gotoxy(x, y+2); cout << warn; Sleep(750);
    }
    return aux;
}
