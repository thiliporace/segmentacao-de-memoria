#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

typedef struct _Node_MemLivre
{
  int End_i;
  int tam;
	struct _Node_MemLivre* next;
}_Node_MemLivre;

typedef struct _Lista_MemLivre
{
  struct _Node_MemLivre* head;
	struct _Node_MemLivre* tail;
}_Lista_MemLivre;

typedef struct _Node_MemAloc
{
  int NProcesso;
  int End_i;
  int tam;
  struct _Node_MemAloc *next;
}_Node_MemAloc;

typedef struct _Lista_MemAloc
{
  struct _Node_MemAloc *head;
  struct _Node_MemAloc *tail;
}_Lista_MemAloc;

_Lista_MemLivre *ListaLivre;
_Lista_MemAloc *ListaAlocados;
int TAM;

// Inicia o Bloco de Memoria Livre e Bloco de Memoria Alocada. 
void inicia (void)
{
  printf("Insira o total da memoria: ");
  scanf("%d",&TAM);
  _Node_MemLivre *novoBloco = (_Node_MemLivre*) calloc(TAM, sizeof(_Node_MemLivre));
  novoBloco->End_i = TAM;
  novoBloco->tam = TAM;
  novoBloco->next = NULL;

  ListaLivre = (_Lista_MemLivre*) calloc(TAM, sizeof(_Lista_MemLivre));
  ListaLivre->head = novoBloco;
  ListaLivre->tail = novoBloco;

  ListaAlocados = (_Lista_MemAloc*) calloc(TAM, sizeof(_Lista_MemAloc));
  ListaAlocados->head = NULL;
  ListaAlocados->tail = NULL;
  printf("\nMemoria iniciada com %d bytes\n",TAM);
}

void printFreeList() 
{
  int livre;
  printf("\nLista Free\n");
  livre = ListaLivre->tail->tam;
  printf("espaço livre: %d\n",livre);
}

void printAlocList() 
{
  int num = 1,inicio;
  _Node_MemAloc* current = ListaAlocados->head;
  if (current==NULL)
    printf("Nao ha nada alocado");
  else
  {
    printf("\nLista Aloc\n");
    while (current != NULL) 
    {
      inicio = current->End_i - current->tam;
      printf("%dºbloco: Id = %d Tamanho = %d,inicio = %d fim = %d\n",num,current->NProcesso,current->tam,inicio,current->End_i);
      current = current->next;
      num++;
    }
  }
}

void UpdateMemLivre(int end_i, int tam)
{//como o livre é sempre o tail, ele se insete entre o tail e o antes do tail
  _Node_MemLivre *novoBloco = (_Node_MemLivre*) calloc(tam, sizeof(_Node_MemLivre));
  
  if(ListaLivre->head==ListaLivre->tail)//é o primeiro a ser inserido
  {
    novoBloco->next=ListaLivre->head;
    ListaLivre->head = novoBloco;
    novoBloco->End_i = tam;
    novoBloco->tam = tam;
    novoBloco->next->End_i = TAM;
    novoBloco->next->tam = TAM-novoBloco->End_i;
  }
  else
  {
    _Node_MemLivre *prev = ListaLivre->head;
    while(prev->next != ListaLivre->tail)//pra achar o anterior ao tail
      prev = prev->next;
    prev->next = novoBloco;
    novoBloco->next = ListaLivre->tail;//agora esta entre o previamente preenchido e o tail, que é o livre.
    novoBloco->End_i = prev->End_i+tam;
    novoBloco->tam = tam;
    novoBloco->next->End_i = TAM;
    novoBloco->next->tam = TAM-novoBloco->End_i;
  }
}

void InsereBlocoAloc(int nProcesso, int end_i, int tam)
{
 _Node_MemAloc *novoBloco = (_Node_MemAloc*) calloc(tam, sizeof(_Node_MemAloc));
  novoBloco->next = NULL;
  novoBloco->tam = tam;
  novoBloco->NProcesso = nProcesso;
  
  if(ListaAlocados->head==NULL)//lista vazia
  {
    novoBloco->End_i = tam;
    ListaAlocados->head = novoBloco;
  }
  else
  {
     ListaAlocados->tail->next = novoBloco;
     novoBloco->End_i = ListaAlocados->tail->End_i+tam;
  }
  ListaAlocados->tail = novoBloco;
  UpdateMemLivre(end_i, tam);
  printf("\nBloco Inserido!\n");
}

int BuscaEspacoDisp (int Tam)
{
  int livre;
   _Node_MemLivre* blocoLivre = ListaLivre->tail;
  livre = blocoLivre->tam;
  if(livre<Tam)
    return -1;
  else 
    return blocoLivre->End_i-blocoLivre->tam;//fim do ultimo antes do livre
}

// Alocar Bloco de Memoria para Processo
// Antes de alocar, deve buscar o espaço livre que atenda o tamanho solicitado.
void AlocaMemoria (int nProcesso, int Tam)
{
  if(ListaLivre->head->tam == TAM)//primeiro a ser alocado
    InsereBlocoAloc(nProcesso,Tam,Tam);
  else
  {
    int endUltimo = BuscaEspacoDisp(Tam);
    if(endUltimo == -1)
      printf("Espaço insuficiente para alocar %d bytes de memoria",Tam);
    else
      InsereBlocoAloc(nProcesso,endUltimo,Tam);
  }
}

void LiberaLivre()
{
  _Node_MemLivre* nodeLivre = ListaLivre->head;//cria nó node que começa em head
	_Node_MemLivre* nextLivre = NULL;//cria nó next que começa como nulo
	while (nodeLivre != NULL)//enquanto node não for nulo
	{
		nextLivre = nodeLivre->next;//next aponta para node->next
		free(nodeLivre);//libera memória alocada para node
		nodeLivre = nextLivre;//node aponta para next
  }
	ListaLivre->head = NULL;
	ListaLivre->tail = NULL;//destruindo as referencias já que a lista esvaziou
  
  _Node_MemLivre *novoBloco = (_Node_MemLivre*) calloc(TAM, sizeof(_Node_MemLivre));
  novoBloco->End_i = TAM;
  novoBloco->tam = TAM;
  novoBloco->next = NULL;

  ListaLivre->head = novoBloco;
  ListaLivre->tail = novoBloco;
}
void LiberaAloc()
{
  _Node_MemAloc* node = ListaAlocados->head;//cria nó node que começa em head
	_Node_MemAloc* next = NULL;//cria nó next que começa como nulo
	while (node != NULL)//enquanto node não for nulo
	{
		next = node->next;//next aponta para node->next
		free(node);//libera memória alocada para node
		node = next;//node aponta para next
  }
	ListaAlocados->head = NULL;
	ListaAlocados->tail = NULL;//destruindo as referencias já que a lista esvaziou
}

void Liberalistas ()
{
  LiberaAloc();
  LiberaLivre();  
}



void DefragAlocMemory ()
{
  bool frag = false;
  _Node_MemAloc* current = ListaAlocados->head;//cria nó toRemove que começa em head
	_Node_MemAloc* previous = NULL;// cria nó previous que começa como nulo
  while(current!=NULL)
  {
    if(previous == NULL)//esta no head
    {
      if(current->End_i-current->tam != 0)//deletaram o inicio
       {
        current->End_i = current->tam;
        frag = true; 
       }
    }
    else
    {
      if (previous->End_i != (current->End_i-current->tam))//ocorreu fragmentação
      {
        frag = true;
        current->End_i = previous->End_i + current->tam;//"remendando" os valores de memoria
      }
    }
    previous = current;//previous aponta para toRemove
    current = current->next;//toRemove aponta para o próximo nó da lista
  }
  if (frag == true)
    printf("Ocorreu Fragmentação de memoria, desfragmentando...");
  previous= NULL;free(previous);
  current= NULL;free(current);
}

void DefragFreeMemory()// sempre roda APOS o defragAloc
{//mais facil recriar a lista de livres...
  LiberaLivre();
  _Node_MemAloc* current = ListaAlocados->head;
  while(current!=NULL)
  {
    UpdateMemLivre(current->End_i,current->tam);
    current= current->next;
  }
  printf("\nProcesso liberado e memória desfragmentada!\n");
}

// Finaliza o processo
void FinalizaProcesso(int Id)
{
  if (ListaAlocados->head == NULL)
    printf("\nNão Há Nada Alocado\n");
	else
	{
		_Node_MemAloc* toRemove = ListaAlocados->head;//cria nó toRemove que começa em head
		_Node_MemAloc* previous = NULL;// cria nó previous que começa como nulo
		while (toRemove != NULL && toRemove->NProcesso != Id)
		{
			previous = toRemove;//previous aponta para toRemove
			toRemove = toRemove->next;//toRemove aponta para o próximo nó da lista
		}
		if (toRemove == NULL)
		{
			printf("\nID nao encontrado\n");
		}//se toRemove for nulo, retorna nulo
      
		else if (toRemove == ListaAlocados->head)
    {
      if (ListaAlocados->head == ListaAlocados->tail)//significa que só há um nó
        Liberalistas ();
      else
        ListaAlocados->head = ListaAlocados->head->next;// se não é o único, o head vira o próximo
      toRemove->next = NULL;
    }

		else if (toRemove == ListaAlocados->tail)
    {
  		if (previous != NULL)
  		  previous->next = NULL;//pois vai virar o último
  		ListaAlocados->tail = previous;//tail aponta para penúltimo nó(previous)
  		toRemove->next = NULL;
    }		
    else
		{
			previous->next = toRemove->next;//religando a lista
			toRemove->next = NULL;//next de toRemove aponta para nulo(quebra vínculo com a lista).
		}
    previous=NULL;
    toRemove=NULL;
    free(previous);
    free(toRemove);
    DefragAlocMemory();
    DefragFreeMemory();
	} 
}


int main(void) 
{
  int option=1;
  // A idéia é só alocar os blocos ao fim da memoria livre, e toda vez que algo for deletado, desfragmentar a   memoria, juntando os blocos, para o fim da memoria ficar livre 
  inicia ();
  while(option!=0)
  {
    printf("\nDigite a opção:\n\n1 - Alocar Memoria\n2 - Liberar Memoria\n3 - Limpar todos os alocados\n4- Imprimir estado da memoria\n0 - finalizar programa\n");
    scanf("%d",&option);

    int nProcesso,Tam;
    switch(option)
    {
      case 1:
        printf("\nId do processo: ");
        scanf("%d",&nProcesso);
        printf("\nTamanho do processo: ");
        scanf("%d",&Tam);
        AlocaMemoria (nProcesso,Tam);
      break;
      
      case 2:
        printf("\nId do processo a ser deletado: ");
        scanf("%d",&nProcesso);
        FinalizaProcesso(nProcesso);
      break;
      
      case 3:
        Liberalistas();
        printf("Memoria inteira Liberada!");
      break;
      
      case 4:
        printAlocList();
        printFreeList();
      break;
      
      case 0:
        printf("***FIM***");
      break;
      
      default:
        printf("\nOpção invalida");
      break;
    }
  }
}