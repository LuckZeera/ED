#include <fstream>
#include <iostream>
#include <cstring>
using namespace std;

struct Registro { // registro com os campos a serem lidos da base de dados
    char anzsic06 [5] = {char(0)};
    char Area [7] = {char(0)};
    char ano [7] = {char(0)};
    char geo_count [7] = {char(0)};
    char ec_count [7] = {char(0)};
    // os vetores de char sempre serão inicializados completos por caracteres nulos
};

void inverterPosicoes(long long int pos1,long long  int pos2) {
  if(pos1 == 300000)
    cout << 0 << endl;
  cout << pos1 << endl;
  Registro registro1, registro2;
  fstream receber1, receber2;
  receber1.open ("CSV.bin", ios::in | ios::out | ios::ate); // declaração das funções de manipulação do arquivo binário
  receber2.open ("CSV.bin", ios::in | ios::out | ios::ate);
  // posicionamento das cabeças de leitura nas posições desejadas
  receber1.seekg (pos1 * sizeof (Registro));
  receber2.seekg (pos2 * sizeof (Registro));
  // leitura dos registros nas posições solicitadas
  receber1.read ((char*)(&registro1), sizeof (Registro));
  receber2.read ((char*)(&registro2), sizeof (Registro));
  // posicionamento das cabeças de escrita invertendo as posições
  receber1.seekp (pos2 * sizeof (Registro));
  receber2.seekp (pos1 * sizeof (Registro));
  // gravação dos registros com as posições trocadas
  receber1.write ((const char*)(&registro1), sizeof (Registro));
  receber2.write ((const char*)(&registro2), sizeof (Registro));
  // encerramento das funções
  receber1.close ();
  receber2.close ();
}

// function to rearrange array (find the partition point)
int partition(fstream &arquivo,long long int low, long long int high) {

  // select the rightmost element as pivot
  arquivo.seekg(high * sizeof(Registro));
  Registro pivot;
  arquivo.read((char*) (&pivot), sizeof(Registro));

  // pointer for greater element
  long long int i = (low - 1);
  Registro aux;
  // traverse each element of the array
  // compare them with the pivot
  for (long long int j = low; j < high; j++) {
    arquivo.seekg(j * sizeof(Registro));
    arquivo.read((char*)(&aux), sizeof(Registro));
    if (string(aux.Area) < string(pivot.Area) or (string (aux.Area) == string (pivot.Area) and string (aux.geo_count) <= string (pivot.geo_count))) {
      // if element smaller than pivot is found
      // swap it with the greater element pointed by i
      i++;
      // swap element at i with element at j
      inverterPosicoes(i,j);
    }
  }
  // swap pivot with the greater element at i
  inverterPosicoes(i+1,high);

  // return the partition point
  return (i + 1);
}

void quickSort(fstream &arquivo,long long  int low, long long int high) {
  if (low < high) {

    // find the pivot element such that
    // elements smaller than pivot are on left of pivot
    // elements greater than pivot are on righ of pivot
    long long int pi = partition(arquivo, low, high);

    // recursive call on the left of pivot
    quickSort(arquivo, low, pi - 1);

    // recursive call on the right of pivot
    quickSort(arquivo, pi + 1, high);
  }
}
void depurarTodosRegistros () {
    ifstream ler ("CSV.bin"); // abre o arquivo binario para leitura
    ler.seekg (0, ler.end); // posiciona a cabeça de leitura no fim
    long long int tam_bytes = ler.tellg (); // recebe o número de bytes do arquivo
    long long int qntdCadastrados = (tam_bytes / sizeof (Registro)); 
    // divide o tamanho do arquivo pelo tamanho da estrutura Registro para saber o número de registros no arquivo
    ler.seekg (0, ler.beg); // retorna a cabeça de leitura para o início
    if (ler) {
        for (long long int j = 0; j < 300000; j++) { // enquanto for possível ler, a variável "registro" recebe um registro lido do arquivo
            Registro registro;
            ler.read ((char*)(&registro), sizeof (Registro));
            // a cada registro lido são depurados seus atributos
            for (int i = 0; i < 5; i++)
                cout << registro.anzsic06 [i];
            cout << ",";
            for (int i = 0; i < 7; i++)
                cout << registro.Area [i];
            cout << ",";
            for (int i = 0; i < 7; i++)
                cout << registro.ano [i];
            cout << ",";
            for (int i = 0; i < 7; i++)
                cout << registro.geo_count [i];
            cout << ",";
            for (int i = 0; i < 7; i++)
                cout << registro.ec_count [i];
            cout << endl;
        }
    }
    else {
        ler.close (); // fechamento da função de leitura
    }
}


int main(){
  fstream arquivo("CSV.bin", ios::in | ios::out);
  ifstream ler("CSV.bin"); // abre o arquivo binario para leitura
  ler.seekg (0, ler.end); // posiciona a cabeça de leitura no fim
  long long int tam_bytes = ler.tellg (); // recebe o número de bytes do arquivo
  long long int qntdCadastrados = (tam_bytes / sizeof (Registro));
  quickSort(arquivo, 0, 300000);
  depurarTodosRegistros();
  return 0;
}
