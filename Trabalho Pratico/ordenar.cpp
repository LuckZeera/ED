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

void intercala(Registro v[], long long int p,long long  int q, long long int r){
    long long int i = p, j = q;
    long long int tamanho = r - p + 1;
    Registro w[tamanho]; // vetor auxiliar
    long long int k = 0;
    while ((i < q) and (j <= r)) {                
       if (strcpy(v[i].Area,v[j].Area))  {
           w[k++] = v[i++]; /* w[k] = v[i]; k++; i++; */
		} else  {
			w[k++] = v[j++]; /* w[k] = v[j]; k++; j++; */
		}
	} 
    // terminou um dos vetores, agora copia o outro                           
    while (i < q) {
        w[k++] = v[i++];
    }      
    while (j <= r) {
       w[k++] = v[j++]; 
    }
    // agora copiamos do vetor auxiliar aux[] em v[p:r]
	for (long long int m = 0; m < tamanho; m++){ 
		v[p + m] = w[m];
    }
}

void mergesort(Registro a[], long long int inicio, long long int fim){
	long long int meio;
	if (inicio < fim) {
		meio = (inicio + fim)/2; 
		mergesort(a, inicio, meio);
		mergesort(a, meio+1, fim);
		intercala(a,inicio,meio,fim);
	}
}

void FazerVetor(){
	Registro converter;
	ifstream ler("CSV.bin"); // abre o arquivo binario para leitura
    ler.seekg (0, ler.end); // posiciona a cabeça de leitura no fim
    long long int tam_bytes = ler.tellg (); // recebe o número de bytes do arquivo
    long long int qntdCadastrados = (tam_bytes / sizeof (Registro));
    Registro novoRegistro[150];
    long long int inicio = 0;
    // divide o tamanho do arquivo pelo tamanho da estrutura Registro para saber o número de registros no arquivo
    long long int linhaInicio = 2000000;
    ler.seekg ((linhaInicio) * sizeof (Registro));
    if (ler) {
        for (long long int j = 0; j < 150; j++) { // enquanto for possível ler, a variável "registro" recebe um registro lido do arquivo
            ler.read ((char*)(&converter), sizeof (Registro));
            novoRegistro[j] = converter;
        }
    }
    ler.close (); // fechamento da função de leitura
    mergesort(novoRegistro, inicio, 150);
    for(long long int j=0; j < 150; j++){
        cout << j << " ";
        int length = sizeof(novoRegistro[j].anzsic06)/sizeof(char);
        for (int i = 0; i < length; i++)
            cout <<  novoRegistro[j].anzsic06 [i];
        cout << ",";
        length = sizeof(novoRegistro[j].Area)/sizeof(char);
        for (int i = 0; i < length; i++)
            cout << novoRegistro[j].Area [i];
        cout << ",";
        length = sizeof(novoRegistro[j].ano)/sizeof(char);
        for (int i = 0; i < length; i++)
            cout << novoRegistro[j].ano [i];
        cout << ",";
        length = sizeof(novoRegistro[j].geo_count)/sizeof(char);
        for (int i = 0; i < length; i++)
            cout << novoRegistro[j].geo_count [i];
        cout << ",";
        length = sizeof(novoRegistro[j].ec_count)/sizeof(char);
        for (int i = 0; i < length; i++)
            cout << novoRegistro[j].ec_count [i];
        cout << endl;
    }                  
}

int main(){
    FazerVetor();
    return 0;
}
