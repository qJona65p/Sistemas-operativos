#ifndef LISTA_DL_H
#define LISTA_DL_H

using namespace std;

template <typename objeto>
class listaDL{
private:
    struct nodo{
        objeto dato;
        nodo *next;
        nodo *prev;
        nodo(const objeto &d=objeto{}, nodo *n=nullptr):dato{d}, next{n}, prev{n}{}
        nodo(objeto &&d=objeto{}, nodo *n=nullptr):dato{move(d)}, next{move(n)}, prev{move(n)}{}
    };
    nodo *head;
    nodo *tail;
    int tamanio;
    int capacidad;

public:
    listaDL(const listaDL <objeto> &rhs){
        iniciar();
        /*for(auto &x:rhs)
        push_b(x);*/
        //copy ctor
    }
    listaDL(listaDL <objeto> &&rhs):head{rhs.head},tail{rhs.tail},tamanio{rhs.tamanio},capacidad{rhs.capacidad}{
        rhs.head=nullptr;
        rhs.tail=nullptr;
        rhs.tamanio=0;
        rhs.capacidad=10000;
    }
    listaDL(){
        iniciar();
    }
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
        //assignment operator
        return *this;
    }

    void iniciar(){
    	head=nullptr;
        tail=nullptr;
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
            tamanio++;
        }
        else{
            tail->next=p;
            p->prev=tail;
            tail=p;
            tamanio++;
    	}
    }
    void push_b(objeto &&x){
        nodo *p=new nodo{move(x),nullptr};
        if(vacia()){
            p->next=head;
            if (head != nullptr) head->prev=p;
            head=p;
            tail=head;
            tamanio++;
        }
        else{
            tail->next=p;
            p->prev=tail;
            tail=p;
            tamanio++;
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
            if (pos < tamanio) {
                p=p->next;
            }
            else break;
        }
        return p->dato;
    }
};

#endif // LISTA_DL_H
