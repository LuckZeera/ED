#include <iostream>
using namespace std;

class Noh{
    friend class Lista;
    private:
        int dado;
        Noh* proximo;
    public:
        Noh(int dado);
};

Noh::Noh(int d){
    dado = d;
    proximo = NULL;
}

class Lista{
    private:
        Noh *primeiro;
        Noh *ultimo;
        int tamanho;
        void removerTudo();
        void imprimirReversoAux(Noh* umNoh);
    public:
        Lista();
        Lista(const Lista& umaLista);
        ~Lista();
        Lista& operator=(const Lista& umaLista);
        inline void inserir(int dado);
        void inserirFim(int dado);
        void inserirInicio(int dado);
        void removerInicio();
        void removerFim();
        void imprimir();
        void ordenar();
        inline bool vaziar();
};

Lista::Lista(){
    primeiro = NULL;
    ultimo = NULL;
    tamanho = 0;
}

Lista::Lista(const Lista& umaLista){
    primeiro = NULL;
    ultimo = NULL;
    tamanho = 0;
    Noh* aux = umaLista.primeiro;
    while(aux != NULL){
        inserirFim(aux->dado);
        aux = aux->proximo;
    }
}

Lista::~Lista(){
    removerTudo();
}

void Lista::removerTudo(){
    Noh* aux = primeiro;
    Noh* temp;
    while(aux != NULL){
        temp = aux;
        aux = aux->proximo;
        delete temp;
    }
    tamanho = 0;
    primeiro = NULL;
    ultimo = NULL;
}

Lista& Lista::operator=(const Lista& umaLista){
    removerTudo();
    Noh* aux = umaLista.primeiro;
    while(aux != NULL){
        inserirFim(aux->dado);
        aux = aux->proximo;
    }
    return *this;
}

void Lista::inserir(int dado){
    inserirFim(dado);
}

void Lista::inserirFim(int dado){
    Noh* novo = new Noh(dado);
    if(vaziar()){
        primeiro = novo;
        ultimo = novo;
    }else{
        ultimo->proximo = novo;
        ultimo = novo;
    }
    tamanho++;
}

void Lista::inserirInicio(int dado){
    Noh* novo = new Noh(dado);
    if(vaziar()){
        primeiro = novo;
        ultimo = novo;
    }else{
        novo->proximo = primeiro;
        primeiro = novo;
    }
    tamanho++;
}


void Lista::imprimir(){
    Noh* aux = primeiro;
    while(aux != NULL){
        cout << aux->dado << " ";
        aux = aux->proximo;
    }
    cout << endl;
}

inline bool Lista::vaziar(){
    return (primeiro == NULL);
}

void Lista::removerFim(){
    if(vaziar()){
        cerr << "Remoção em lista vazia!" << endl;
        exit(EXIT_FAILURE);
    }
    Noh* aux = primeiro;
    Noh* anterior;
    while(aux->proximo != NULL){
        anterior = aux;
        aux = aux->proximo;
    }
    delete ultimo;
    anterior->proximo = NULL;
    ultimo = anterior;
    tamanho--;
    if(tamanho == 0) primeiro = NULL;
    
}

void Lista::removerInicio(){
    if(vaziar()){
        cerr << "Remoção em lista vazia!" << endl;
        exit(EXIT_FAILURE);
    }
    Noh* removido = primeiro;
    primeiro = primeiro->proximo;
    delete removido;
    tamanho--;
    if(vaziar()) ultimo = NULL;
}

void Lista::ordenar(){
    int tam = tamanho / 2,n1,n2;
    for(int i=0;i<tam;i++){
        n1 = primeiro -> dado;
        n2 = primeiro -> proximo-> dado;
        if(n1 >)
    }
}

int main(){
    Lista l1,l2;
    int n;
    cin >> n;
    while(n != -1){
        l1.inserir(n);
        cin >> n;
    }
    cin >> n;
    while(n != -1){
        l2.inserir(n);
        cin >> n;
    }
    l1.ordenar();
    l2.ordenar();
}