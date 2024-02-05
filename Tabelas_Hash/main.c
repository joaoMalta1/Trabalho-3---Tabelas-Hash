/* João Ricardo Malta de Oliveira - 2112714 */
/* Vinicius Machado da Rocha Viana - 2111343*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX 1031
#define NUM 7
#define LIMITE 1000
typedef struct placa Placa;
struct placa {
  char placa[NUM];
  int ocupado;
};


FILE* abreArq(char* nome,char* modo)
{
  FILE* arq = fopen(nome,modo);
  if(arq ==NULL)
  {
    printf("erro ao abrir o arquivo\n");
    exit(1);
  }
  return arq;
}

void fechaArq(FILE* arq)
{
  fclose(arq);
  return;
}


unsigned int hash(char *placa, int tam) 
{
  unsigned int hash = 1;
  int c;
  for (int i = 0; i < 7; i++) 
  {
    c = *placa++;
    hash = ((c<<5)+c) + ((hash<<5)+hash);
  }
  return (hash  % tam);
}


// insercao com metodo quadratico
void insere(Placa *vet, char *placa, int tam, int* cont) 
{
  int h = hash(placa, tam);
  int i = 0;
  int c1, c2;
  c1 = 599;
  c2 = 677;
  while (vet[h].ocupado != 0) 
  {
    int index = (((h) + (c1 * i) + (c2 * i * i))) % tam;
    h = index;
    i++;
    (*cont)++;
  }
  strcpy(vet[h].placa, placa);
  vet[h].ocupado = 1;
}

// busca com metodo quadratico
int busca(Placa *vet, char *placa, int tam) 
{
  int h = hash(placa, tam); 
  int i = 0;
  int c1 = 599;
  int c2 = 677;

  while (vet[h].ocupado != 0) 
  {
    if (strcmp(vet[h].placa, placa) == 0) 
    {
      return h;
    }
    int index = ((h) + (c1 * i) + (c2 * i * i)) % tam;
    h = index;
    i++;
    // verifica se ja foi em toda a tabela
    if (i >= tam) 
    {
      break;
    }
  }
  return -1;
}

// exclusao tabela hash
void exclui (Placa* vet , char *placa, int tam) 
{
  int index = busca(vet, placa,tam);
  if (index==-1)
  {
    printf("Elemento nao existe na tabela\n");
  }
  else
  {
    strcpy(vet[index].placa, "");
    vet[index].ocupado =0;
    printf("Elemento excluido com sucesso\n");
  }
}

int main() 
{

  Placa vet[MAX];
  for (int i = 0; i < MAX; i++) 
  {
    vet[i].ocupado = 0;
  }
  FILE* arqEntrada = abreArq("placas.txt", "rt");
  int c ;
  char placa[NUM];
  double timerInclusao;
  int colisoes =0;
  clock_t inicio, fim, iniCod,fimCod;
  int contalinhas = 0;
  inicio = clock();
  iniCod = clock();
  while (contalinhas <LIMITE && fscanf(arqEntrada, "%s", placa) != EOF) 
  {
    insere(vet, placa, MAX, &colisoes);
    contalinhas++;
  }
  
  fim = clock();
  timerInclusao = (double)(fim - inicio) / CLOCKS_PER_SEC;
  printf("Tempo de inclusão de placas: %f segundos\n", timerInclusao);
  printf("Numero de colisoes: %d\n",colisoes);
 
  // Busca das placas 
  inicio = clock();
  int posicao = busca(vet, "HFX5A59", MAX);
  printf("posicao da placa: %d\n",posicao);
  printf("-----%s----\n",vet[posicao].placa);
  fim = clock();
  double timerBusca = (double)(fim - inicio) / CLOCKS_PER_SEC;
  printf("Tempo de busca: %f segundos\n", timerBusca);

  inicio = clock();
  exclui(vet, placa, MAX);
  fim = clock();
  double timerExclusao = (double)(fim - inicio) / CLOCKS_PER_SEC;
  printf("Tempo de exclusao: %f segundos\n", timerExclusao);
  fechaArq(arqEntrada);
  fimCod=clock();
  double timerCod = (double)(fimCod - iniCod) / CLOCKS_PER_SEC;
  printf("Tempo de codigo: %f segundos\n", timerCod);
  return 0;
}


