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

    class iterador_const{
        protected:
            nodo *actual;
            objeto & recuperar() const{
                return actual->dato;
            }
            iterador_const (nodo *p):actual{p}{}
            friend class listaDL <objeto>;
        public:
            iterador_const():actual{nullptr}{}
            const objeto & operator*()const{
                return recuperar();
            }
            iterador_const & operator++(){
                actual=actual->next;
                return *this;
            }
            iterador_const & operator++(int){
                iterador_const viejo=*this;
                ++(*this);
                return viejo;
            }
            iterador_const & operator--(){
                actual=actual->prev;
                return *this;
            }
            iterador_const & operator--(int){
                iterador_const viejo=*this;
                --(*this);
                return viejo;
            }
            bool operator== (const iterador_const &rhs)const{
                return actual == rhs.actual;
            }
            bool operator!=(const iterador_const &rhs)const{
                return !(*this == rhs);
            }
    };
    class iterador:public iterador_const{
        protected:
            friend class listaDL <objeto>;
        public:
            iterador(){}
            objeto & operator*(){
                return iterador_const::recuperar();
            }
            const objeto & operator*()const{
                return iterador_const::operator*();
            }
            iterador & operator++(){
                this->actual=this->actual->next;
                return *this;
            }
            iterador operator++(int){
                iterador viejo=*this;
                ++(*this);
                return viejo;
            }
            iterador & operator--(){
                this->actual=this->actual->prev;
                return *this;
            }
            iterador operator--(int){
                iterador viejo=*this;
                --(*this);
                return viejo;
            }
    };

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
    bool llena(){
        if(tamanio == capacidad) return true;
        else return false;
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
    void push_f(const objeto &x){
        nodo *p=new nodo{x, nullptr};
        if(vacia()){
          p->next=head;
          if (head != nullptr) head->prev=p;
          head=p;
          tail=head;
          tamanio++;
        }
        else{
            p->next=head;
            if (head != nullptr) head->prev=p;
            head=p;
            tamanio++;
        }
    }
    void push_f(objeto &&x){
        nodo *p=new nodo{move(x),nullptr};
        if(vacia()){
            p->next=head;
            if (head != nullptr) head->prev=p;
            head=p;
            tail=head;
            tamanio++;
        }
        else{
            p->next=head;
            if (head != nullptr) head->prev=p;
            head=p;
            tamanio++;
    	}
    }
    void push_p(const objeto &x, int pos){
        if(vacia()){
            nodo *p=new nodo{x, nullptr};
            p->next=head;
            if (head != nullptr) head->prev=p;
            head=p;
            tail=head;
            tamanio++;
        }
        else if(validarPos(pos) == -1){push_f(x);}
        else if(validarPos(pos) == -2){push_b(x);}
        else if(validarPos(pos) == 1){
            nodo *pre, *aft;
    		pre=head;
    		for(int k=0; k<pos-1; k++) pre=pre->next;
    		aft=pre->next;
    		nodo *p=new nodo{x, nullptr};
    		p->next=aft;
            aft->prev=p;
    		pre->next=p;
            p->prev=pre;
            tamanio++;
    	}
    }
    void push_p(objeto &&x, int pos){
        if(vacia()){
            nodo *p=new nodo{move(x), nullptr};
            p->next=head;
            if (head != nullptr) head->prev=p;
            head=p;
            tail=head;
            tamanio++;
        }
        else if(validarPos(pos) == -1){push_f(x);}
        else if(validarPos(pos) == -2){push_b(x);}
        else if(validarPos(pos) == 1){
            nodo *pre, *aft;
    		pre=head;
    		for(int k=0; k<pos-1; k++) pre=pre->next;
    		aft=pre->next;
    		nodo *p=new nodo{move(x), nullptr};
            p->next=aft;
            aft->prev=p;
    		pre->next=p;
            p->prev=pre;
    	}
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
    void pop_p(int pos){
        if(validarPos(pos) == 1){
            nodo *pre=head;
            for (int i=0; i < pos-1; i++) pre=pre->next;
            nodo *del=pre->next, *aft=del->next;
            pre->next=aft;
            aft->prev=pre;
            tamanio--;
        }
        else if(validarPos(pos) == -1){pop_f();}
        else if(validarPos(pos) == -2){pop_b();}
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

    int getTamanio(){return tamanio;}

    int busquedaBin(objeto buscar){
        int mitad=(tamanio-1)/2;
        if (mitad < 0) return -1;
        if (buscar == peek_p(mitad)) return mitad;
        else if (buscar < peek_p(mitad)) busquedaBin(0, mitad-1, buscar);
        else if (buscar > peek_p(mitad)) busquedaBin(mitad+1, tamanio, buscar);
    }
    int busquedaBin(int inicio, int fin, objeto buscar){
        int mitad=(inicio+fin)/2;

        if (inicio > fin) return -1;
        if (buscar == peek_p(mitad)) return mitad;
        else if (buscar < peek_p(mitad)) busquedaBin(inicio, mitad-1, buscar);
        else if (buscar > peek_p(mitad)) busquedaBin(mitad+1, fin, buscar);
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

#endif // LISTA_DL_H
