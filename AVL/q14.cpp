#include <iostream>
#include <stdexcept>
#include <cmath>
using namespace std;

struct pokemon {
    unsigned id;
    string nome;
    string tipo;
    int nivel;
};

ostream& operator<<(ostream& saida, const pokemon& e) {
    saida << "(" << e.id << "," << e.nome << "," << e.tipo << "," << e.nivel << ")";
    return saida;
}

istream& operator>>(istream& entrada, pokemon& e) {
    entrada >> e.id >> e.nome >> e.tipo >> e.nivel;
    return entrada;
}

typedef unsigned tipoChave;

class noh {
    friend class avl;
    private:
        pokemon elemento;
        noh* esq;
        noh* dir;
        unsigned altura;
    public:
        noh(const pokemon& umPokemon):
            elemento(umPokemon),  esq(NULL), dir(NULL), altura(1) { }
        ~noh() { }
        int fatorBalanceamento(noh* umNoh);
        unsigned informaAltura(noh* umNoh);
        void atualizaAltura(noh* umNoh);
};

unsigned noh::informaAltura(noh* umNoh){
    if(umNoh == NULL)
        return 0;
    else
        return umNoh->altura;
}

void noh::atualizaAltura(noh* umNoh){
    int altArvEsq = informaAltura(umNoh->esq);
    int altArvDir = informaAltura(umNoh->dir);
    umNoh->altura = 1 + fmax(altArvEsq, altArvDir);
}

int noh::fatorBalanceamento(noh* umNoh) {
    int altArvEsq = informaAltura(umNoh->esq);
    int altArvDir = informaAltura(umNoh->dir);
    int fatorbal = altArvEsq - altArvDir;
    return fatorbal;
}

class avl {
    friend ostream& operator<<(ostream& output, avl& arvore);
    private:
        noh* raiz;
        // percorrimento em ordem da árvore
        void percorreEmOrdemAux(noh* atual, int nivel);
        // funções auxiliares para remoção
        noh* encontraMenor(noh* raizSub);
        noh* removeMenor(noh* raizSub);
        // funções auxiliares para inserção e remoção usando método recursivo
        // retorna o nó para ajustar o pai ou o raiz
        noh* insereAux(noh* umNoh, const pokemon& umPokemon);
        noh* removeAux(noh* umNoh, tipoChave id);
        // métodos para manutenção do balanceamento
        noh* rotacaoEsquerda(noh* umNoh);
        noh* rotacaoDireita(noh* umNoh);
        noh* arrumaBalanceamento(noh* umNoh);
        // busca, método iterativo
        noh* buscaAux(tipoChave id);
        // função auxiliar do destrutor, usa percorrimento pós-ordem
        void destruirRecursivamente(noh* umNoh);
        void imprimirDir(const std::string& prefixo, const noh* meuNoh);
        void imprimirEsq(const std::string& prefixo, const noh* meuNoh, bool temIrmao);
        void levantamentoAux(noh* umNoh, string tipo, int nivel,int& cont);
    public:
        avl() { raiz = NULL; }
        ~avl();
        void imprimir();
        // inserção e remoção são recursivos
        void insere(const pokemon& umPokemon);
        void remove(tipoChave id);
        // inserção e remoção, métodos recursivos
        // busca retorna uma cópia do objeto armazenado
        pokemon busca(tipoChave id);
        // efetua levantamento de quantos pokemons existem de um dado tipo e nível
        int levantamento(string tipo, int nivel);
};

// destrutor
avl::~avl() {
    destruirRecursivamente(raiz);
}

// destrutor é recursivo, fazendo percorrimento pós-ordem
void avl::destruirRecursivamente(noh* umNoh) {
    if(umNoh != NULL){
        destruirRecursivamente(umNoh->esq);
        destruirRecursivamente(umNoh->dir);
        delete umNoh;
    }
}

void avl::insere(const pokemon& umDado) {
    raiz = insereAux(raiz, umDado);
}

// inserção recursiva, devolve nó para atribuição de pai ou raiz
noh* avl::insereAux(noh* umNoh, const pokemon& umDado) {
    if(umNoh == NULL){
        noh* novo = new noh(umDado);
        return novo;
    }
    if(umDado.id < umNoh->elemento.id)
        umNoh->esq = insereAux(umNoh->esq,umDado);
    else
        umNoh->dir = insereAux(umNoh->dir,umDado);
    return arrumaBalanceamento(umNoh);
}

// checa e arruma, se necessário, o balanceamento em umNoh,
// fazendo as rotações e ajustes necessários
noh* avl::arrumaBalanceamento(noh* umNoh) {
    if(umNoh == NULL)
        return umNoh;
    umNoh->atualizaAltura(umNoh);
    int fatorBal = umNoh->fatorBalanceamento(umNoh);
    if(((fatorBal >= -1) and (fatorBal <= 1)))
        return umNoh;
    if(fatorBal > 1 and (umNoh->fatorBalanceamento(umNoh->esq) >= 0))
        return rotacaoDireita(umNoh);
    else if(fatorBal > 1 and (umNoh->fatorBalanceamento(umNoh->esq) < 0)){
        umNoh->esq = rotacaoEsquerda(umNoh->esq);
        return rotacaoDireita(umNoh);
    }
    else if(fatorBal < -1 and (umNoh->fatorBalanceamento(umNoh->dir) <= 0))
        return rotacaoEsquerda(umNoh);
    else if(fatorBal < -1 and (umNoh->fatorBalanceamento(umNoh->dir) > 0)){
        umNoh->dir = rotacaoDireita(umNoh->dir);
        return rotacaoEsquerda(umNoh);
    }
    return umNoh;
}


// rotação à esquerda na subárvore com raiz em umNoh
// retorna o novo pai da subárvore
noh* avl::rotacaoEsquerda(noh* umNoh) {
    if(umNoh != NULL){ 
        noh* aux = umNoh->dir;
        umNoh->dir = aux->esq;
        aux->esq = umNoh;
        umNoh->atualizaAltura(umNoh);
        umNoh->atualizaAltura(aux);
        return aux;
    }
    else
        return umNoh;
}


// rotação à direita na subárvore com raiz em umNoh
// retorna o novo pai da subárvore
noh* avl::rotacaoDireita(noh* umNoh) {
    if(umNoh != NULL){
        noh* aux = umNoh->esq;
        umNoh->esq = aux->dir;
        aux->dir = umNoh;
        umNoh->atualizaAltura(umNoh);
        umNoh->atualizaAltura(aux);
        return aux;
    }else
        return umNoh;
}


// método de busca auxiliar (retorna o nó), iterativo
noh* avl::buscaAux(tipoChave chave) {
    noh* atual = raiz;
    while(atual != NULL){
        if(atual->elemento.id == chave)
            return atual;
        else if(atual->elemento.id > chave)
            atual = atual->esq;
        else
            atual = atual->dir;
    } 
    return atual;
}

// busca elemento com uma dada chave na árvore e retorna o registro completo
pokemon avl::busca(tipoChave chave) {
    noh* resultado = buscaAux(chave);
    if (resultado != NULL)
        return resultado->elemento;
    else
        throw runtime_error("Erro na busca: elemento não encontrado!");
}

// nó mínimo (sucessor) de subárvore com raiz em raizSub (folha mais à esquerda)
noh* avl::encontraMenor(noh* raizSub) {
    while(raizSub->esq != NULL){
        raizSub = raizSub->esq;
    }
    return raizSub;
}

// procedimento auxiliar para remover o sucessor substituíndo-o pelo
// seu filho à direita
noh* avl::removeMenor(noh* raizSub) {
    if(raizSub->esq == NULL)
        return raizSub->dir;
    else
        raizSub->esq = removeMenor(raizSub->esq);
    return arrumaBalanceamento(raizSub);
    
}

// remoção recursiva
void avl::remove(tipoChave chave) {
    raiz = removeAux(raiz, chave);
}

noh* avl::removeAux(noh* umNoh, tipoChave chave) {
    if(umNoh == NULL){
        throw runtime_error("Erro na remoção: chave não encontrada!");
    }
    noh* raizSubArvore = umNoh;
    if(chave < umNoh->elemento.id)
        umNoh->esq = removeAux(umNoh->esq, chave); //valor menor que o no atual vai pra subarvore esquerda
    else if(chave > umNoh->elemento.id)
        umNoh->dir = removeAux(umNoh->dir,chave); //valor maior que o no atual vai pra sub direita
    else{
        if(umNoh->esq == NULL)
            raizSubArvore = umNoh->dir;
        else if(umNoh->dir == NULL)
            raizSubArvore = umNoh->esq;
        else{
            raizSubArvore = encontraMenor(umNoh->dir);
            raizSubArvore->dir = removeMenor(umNoh->dir);
            raizSubArvore->esq = umNoh->esq;
        }
        delete umNoh;
    }
    return arrumaBalanceamento(raizSubArvore);
    
}

// utiliza o nó atual e seu nível na árvore (para facilitar visualização)
void avl::percorreEmOrdemAux(noh* atual, int nivel) {
    if(atual != NULL){
        percorreEmOrdemAux(atual->esq,nivel);
        imprimir();
        percorreEmOrdemAux(atual->dir,nivel);
    }
}

ostream& operator<<(ostream& output, avl& arvore) {
    arvore.imprimir();
    return output;
}

// imprime formatado seguindo o padrao tree as subarvores direitas de uma avl
void avl::imprimirDir(const std::string& prefixo, const noh* meuNoh)
{
    if( meuNoh != nullptr )
    {
        std::cout << prefixo
                  << "└d─"
                  << "(" << meuNoh->elemento.id << "," << meuNoh->elemento.nome << ")"
                  << std::endl;

        // Repassa o prefixo para manter o historico de como deve ser a formatacao e chama no filho direito e esquerdo
        imprimirEsq( prefixo + "    " , meuNoh->esq , meuNoh->dir==nullptr );
        imprimirDir( prefixo + "    " , meuNoh->dir );
    }
}

// imprime formatado seguindo o padrao tree as subarvores direitas de uma avl
void avl::imprimirEsq(const std::string& prefixo, const noh* meuNoh, bool temIrmao)
{
    if( meuNoh != nullptr )
    {
        std::cout << prefixo ;

        // A impressao da arvore esquerda depende da indicacao se existe o irmao a direita
        if (temIrmao)
            std::cout << "└e─" ;
        else
            std::cout << "├e─";

        std::cout << "(" << meuNoh->elemento.id << "," << meuNoh->elemento.nome << ")"
                  << std::endl;

        // Repassa o prefixo para manter o historico de como deve ser a formatacao e chama no filho direito e esquerdo
        imprimirEsq( prefixo + "│   " , meuNoh->esq, meuNoh->dir==nullptr );
        imprimirDir( prefixo + "│   " , meuNoh->dir );
    }
}

// imprime formatado seguindo o padrao tree uma avl
void avl::imprimir()
{
    if( this->raiz != nullptr )
    {
        std::cout << "(" << this->raiz->elemento.id << "," << this->raiz->elemento.nome << ")" << std::endl;
        // apos imprimir a raiz, chama os respectivos metodos de impressao nas subarvore esquerda e direita
        // a chamada para a impressao da subarvore esquerda depende da existencia da subarvore direita
        imprimirEsq( " " , this->raiz->esq, this->raiz->dir==nullptr );
        imprimirDir( " " , this->raiz->dir );
    } else
        std::cout << "*arvore vazia*" << std::endl;
}

int avl::levantamento(string tipo,int nivel){
    int cont=0;
    levantamentoAux(raiz,tipo,nivel,cont);
    return cont;
}

void avl::levantamentoAux(noh* umNoh, string tipo, int nivel,int& cont){
    if(umNoh != NULL){
        levantamentoAux(umNoh->esq,tipo,nivel,cont);
        if(umNoh->elemento.tipo == tipo and umNoh->elemento.nivel == nivel)
            cont++;
        levantamentoAux(umNoh->dir,tipo,nivel,cont);
    }
}

int main() {
    avl arvore;
    tipoChave id;
    pokemon umPokemon;
    string tipo;
    int nivel;
    int quantidade;

    char operacao;

    do {
        try {
            cin >> operacao;
            switch (operacao) {
                case 'i': // Inserir recursivamente
                    // objeto recebe id, nome, tipo e nível
                    cin >> umPokemon;
                    arvore.insere(umPokemon);
                    break;
                case 'r': // Remover recursivamente
                    cin >> id;
                    arvore.remove(id);
                    break;
                case 'b': // Buscar
                    cin >> id; // ler o id
                    umPokemon = arvore.busca(id); // escrever dados do pokemon
                    cout << "Elemento buscado: " << umPokemon << endl;
                    break;
                case 'l': // Levantamento por tipo e nível
                    cin >> tipo >> nivel;
                    quantidade = arvore.levantamento(tipo, nivel);
                    cout << "Levantamento de pokemons tipo " << tipo
                         << " e nível " << nivel << ": " << quantidade << endl;
                    break;
                case 'e': // Escrever tudo (em ordem)
                    cout << arvore;
                    break;
                case 'f': // Finalizar execução
                    break;
                default:
                    cout << "Comando invalido!\n";
            }
        } catch (runtime_error& e) {
            cout << e.what() << endl;
        }
    } while (operacao != 'f');

    return 0;
}
