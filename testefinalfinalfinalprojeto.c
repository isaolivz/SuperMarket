/*=================================================================================================================================================================

                                                               PROJETO SEMESTRAL -- ESTRUTURA DE DADOS
                                                                      PROFA: EURIDES BALBINO
                                                                      
                                                                  ISABELLA BATISTA __ SP315632X
                                                                  ERICA MARIANO __ SP3163205

================================================================================================================================================================*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <unistd.h>


#define maxitens 5 // Capacidade máxima de cada prateleira
#define NUM_PRATELEIRAS 10 // Número total de prateleiras




typedef struct {
    char nome[100];
    char prontuario[10];
    int tipo;
} registro; // estrutura para registro dos usuarios



// Estrutura do Produto
typedef struct {
    char nome[50];
    char descricao[100];
    float peso;
    float preco;
} Produto;



// Estrutura da Pilha
typedef struct {
    Produto itens[maxitens];
    int topo; // Índice do topo da pilha (-1 se vazia)
} pilha;

pilha prateleiras[NUM_PRATELEIRAS];

//Estrutura da fila
typedef struct {
    Produto itens[maxitens];
    int frente, tras;
} fila;

//Estruturas para o carrinho
typedef struct NoCarrinho {
    Produto produto;
    struct NoCarrinho *proximo;
} NoCarrinho;

typedef struct {
    NoCarrinho *topo;
} Carrinho;


//Prototipos das funções
void ordenarusuarios (registro *usuarios, int tamanho);
void criarusuariosdat ();
void centralizar(const char *str, int largura);
void teladelogin();
void lerusuarios();
void limpartela();
void lerUsuariosArquivo(registro **usuarios, int *tamanho);
void gravarUsuariosArquivo(registro *usuarios, int tamanho); 
void listarusuarios();
void adicionarusuario ();
void removerusuarios();


void iniciopilha(pilha *p);
int vazia(pilha *p);
int cheia(pilha *p);
int quantidadeItens(pilha *p);
void empilhar(pilha *p, Produto produto);
Produto desempilhar(pilha *p);

void exibirPrateleira(pilha *p, int prateleiraID);
void exibirTodasPrateleiras();
void inicializarTodasPrateleiras();
void salvarPrateleirasEmArquivo();
void carregarPrateleirasDeArquivo();

void retirarProduto(Carrinho *carrinho);
void abastecerPrateleira();

void exibirCarrinho(Carrinho *carrinho);
void inicializarCarrinho(Carrinho *carrinho);
void adicionarAoCarrinho(Carrinho *carrinho, Produto p) ;

void exibirmenumaster();
void exibirMenuUsuarioComum();
void menugerenciarusuarios();
int buscaBinaria(registro *usuarios, int tamanho, const char *nome, const char *prontuario);

void inicializarFila(fila *f) ;
int filaVazia(fila *f) ;
void enfileirar(fila *f, Produto p);
Produto desenfileirar(fila *f) ;
void transferirParaFila(Carrinho *carrinho, fila *caixa);
void finalizarCompra(Carrinho *carrinho);
void processarCaixa(fila *caixa);
 

int main() {
	setlocale(LC_ALL, "Portuguese");

    criarusuariosdat();
    teladelogin();
    inicializarTodasPrateleiras();  // Carrega as prateleiras do arquivo
    abastecerPrateleira();
    exibirTodasPrateleiras();
    salvarPrateleirasEmArquivo();  

    Carrinho carrinho;
    inicializarCarrinho(&carrinho); // Isso evita que topo tenha um valor inválido!
    exibirMenuUsuarioComum(&carrinho);
    return 0;
}

// 1 ETAPA ------- LOGIN E GERENCIAMENTO

void criarusuariosdat (){
	FILE *arquivo = fopen ("USUARIOS.DAT","rb");
	if (arquivo != NULL){
		fclose (arquivo);
		return;
		}// abre o arquivo no modo leitura e confere se esta vazio


    registro usuariosdefault [] = {
            {"Domingos Lucas Latorre de Oliveira", "CJ146456",1},
            {"Leandro Pinto Santana", "CP220383",1},
            {"Rodrigo Ribeiro de Oliveira", "RG134168",1},
            {"Andre Luiz da Silva", "SP030028",1},
            {"Claudia Miyuki Werhmuller", "SP030041",1},
            {"Claudete de Oliveira Alves", "SP03020X",1},
            {"Francisco Verissimo Luciano", "SP030247",1},
            {"Luk Cho Man", "SP060380",1},
            {"Ivan Francolin Martinez", "SP060835",1},
            {"Joao Vianei Tamanini", "SP060914",1},
            {"Jose Oscar Machado Alexandre", "SP070038",1},
            {"Jose Braz de Araujo", "SP070385",1},
            {"Paulo Roberto de Abreu", "SP070816",1},
            {"Eurides Balbino da Silva", "SP07102X",1},
            {"Domingos Bernardo Gomes Santos", "SP090888",1},
            {"Andre Evandro Lourenco", "SP100092",1},
            {"Miguel Angelo Tancredi Molina", "SP102763",1},
            {"Antonio Airton Palladino", "SP112197",1},
            {"Luis Fernando Aires Branco Menegueti", "SP145385",1},
            {"Antonio Ferreira Viana", "SP200827",1},
            {"Leonardo Bertholdo", "SP204973",1},
            {"Marcelo Tavares de Santana", "SP20500X",1},
            {"Wagner de Paula Gomes", "SP215016",1},
            {"Daniel Marques Gomes de Morais", "SP220097",1},
            {"Alexandre Beletti Ferreira", "SP226117",1},
            {"Vladimir Camelo Pinto", "SP240291",1},
            {"Leonardo Andrade Motta de Lima", "SP24031X",1},
            {"Aldo Marcelo Paim", "SP240497",1},
            {"Cesar Lopes Fernandes", "SP890534",1},
            {"Josceli Maria Tenorio", "SZ124382",1},
            {"isabella", "sp", 0}
        };

       int tamanho = sizeof(usuariosdefault)/ sizeof(usuariosdefault[0]);// confere o numero de elementos de usuarios default para usarmos na ordenacao
       ordenarusuarios (usuariosdefault, tamanho);// chama a funçao
       
     arquivo = fopen ("USUARIOS.DAT","wb");
        if (arquivo == NULL) {
        perror("Erro ao criar o arquivo");
        exit(1);
    }
    
       fwrite (usuariosdefault,sizeof(registro),tamanho,arquivo);
       fclose (arquivo);
	   }// abre o arquivo, escreve os usuarios e fecha o arquivo
       
       
	   
	   void ordenarusuarios (registro *usuarios, int tamanho){
       	
       	int i,j;
       	registro aux;
       	
       	for (i=1;i<tamanho;i++) {
       		aux = usuarios[i];
       		j = i-1;
       		
       		while (j >= 0 && strcmp(usuarios[j].nome, aux.nome) > 0) {
            usuarios[j + 1] = usuarios[j];
            j--;
        }
        usuarios[j + 1] = aux;
    }
		   }//insertion sort para ordenação dos usuarios
		   
/*	___
	
	BUSCA BINARIA
	___*/
	
		   
	int buscaBinaria(registro *usuarios, int tamanho, const char *nome, const char *prontuario) {
		
    int inicio = 0, fim = tamanho - 1;

    while (inicio <= fim) {
        int meio = (inicio + fim) / 2;
        int cmp = strcmp(usuarios[meio].nome, nome);

        if (cmp == 0 && strcmp(usuarios[meio].prontuario, prontuario) == 0) {
            return meio;
        }
        if (cmp < 0) {
            inicio = meio + 1;
        } else {
            fim = meio - 1;
        }
    }
    return -1;
}// realiza a busca binaria nos nomes ja ordenados



void centralizar(const char *str, int largura) {
    int comprimento = strlen(str);
    if (comprimento >= largura) {
        printf("%s\n", str); // Se for maior que a largura, imprime normalmente
        return;
    }
    int espacos = (largura - comprimento) / 2;
    printf("%*s%s\n", espacos, "", str); // Usa especificador de formato para alinhar
}//função para layout


void teladelogin(){
	
	  char nome[100], prontuario[10];
	 int largura = 200; // Definido a largura da linha
    limpartela();
     printf("__________________________________________________________________________________________________________________________________________________________________________________________________\n");
    printf ("\n");
    
    centralizar("SUPERMARKET IFSP", largura);
    centralizar("Seu conforto é o nosso prazer, supermarket de A a Z :)", largura);
    printf ("\n");
    
  printf("__________________________________________________________________________________________________________________________________________________________________________________________________\n");
    printf ("\n");
   
    printf ("   |Informe seu nome:|   ");
    fgets (nome, sizeof(nome),stdin);
nome[strcspn(nome, "\n")] = 0;
printf ("\n");

printf("   |Informe seu prontuário:|   ");
    fgets(prontuario, sizeof(prontuario), stdin);
    prontuario[strcspn(prontuario, "\n")] = 0;
    printf("__________________________________________________________________________________________________________________________________________________________________________________________________\n");

/*_

ALOCAÇÃO DE MEMORIA 
MALLOC
__*/

FILE *arquivo = fopen("USUARIOS.DAT", "rb");
    if (arquivo == NULL) {
        printf("Erro: Arquivo de usuários não encontrado.\n");
        exit(1);
    }

    fseek(arquivo, 0, SEEK_END);
    long tamanho = ftell(arquivo) / sizeof(registro);
    rewind(arquivo);

    registro *usuarios = (registro *)malloc(tamanho * sizeof(registro));
    if (usuarios == NULL) {
        printf("malloc devolveu NULL.\n");
        fclose(arquivo);
        exit(1);
    }

    fread(usuarios, sizeof(registro), tamanho, arquivo);
    fclose(arquivo);
	
	
 /*_
 
	VALIDAR O LOGIN
 _*/
	 
    int indice = buscaBinaria(usuarios, tamanho, nome, prontuario);


    if (indice == -1) {
    	    free(usuarios);
    	limpartela();
    	 centralizar ("\n",largura);
  centralizar ("\n",largura);
  centralizar ("_ !!! _",largura);
   centralizar ("\n",largura);
        centralizar ("Erro: USUARIO E/OU PRONTUARIO INVALIDO\n", largura);
        centralizar ("_ !!! _",largura);
          centralizar ("\n",largura);
        centralizar ("Tente novamente (selecione a tecla enter)", largura);
          getchar ();
        limpartela();
        teladelogin ();
        exit(1);
    }
    limpartela();
 centralizar ("\n",largura);
  centralizar ("\n",largura);
   centralizar ("=====================\n",largura);
   centralizar (" Login bem-sucedido!",largura);
   centralizar ("=====================\n",largura);
   if (usuarios[indice].tipo == 1) {
    exibirmenumaster();
} else {
   exibirMenuUsuarioComum();
}
    free(usuarios);
}    

/*	___
	
	GERENCIAR USUARIOS
	___*/
	
	void lerUsuariosArquivo(registro **usuarios, int *tamanho) {
    FILE *arquivo = fopen("USUARIOS.DAT", "rb");
    if (arquivo == NULL) {
        printf("Erro ao abrir arquivo de usuários.\n");
        exit(1);
    }

    fseek(arquivo, 0, SEEK_END);
    *tamanho = ftell(arquivo) / sizeof(registro);
    rewind(arquivo);

    *usuarios = (registro *)malloc((*tamanho) * sizeof(registro));
    if (*usuarios == NULL) {
        printf("Erro ao alocar memória.\n");
        fclose(arquivo);
        exit(1);
    }

    fread(*usuarios, sizeof(registro), *tamanho, arquivo);
    fclose(arquivo);
}

void gravarUsuariosArquivo(registro *usuarios, int tamanho) {
    FILE *arquivo = fopen("USUARIOS.DAT", "wb");
    if (arquivo == NULL) {
        printf("Erro ao abrir arquivo para gravar.\n");
        exit(1);
    }

    fwrite(usuarios, sizeof(registro), tamanho, arquivo);
    fclose(arquivo);
}
	
	void listarusuarios() {
    FILE *arquivo = fopen("USUARIOS.DAT", "rb");
    if (arquivo == NULL) {
        printf("Erro ao abrir arquivo de usuários.\n");
        return;
    }

    printf("+----------------------------------------------------------+--------------+--------+\n");
    printf("| Nome                                                     | Prontuário   | Tipo   |\n");
    printf("+----------------------------------------------------------+--------------+--------+\n");

    registro temp;
    while (fread(&temp, sizeof(registro), 1, arquivo)) {
        printf("| %-56s | %-12s | %-6s |\n", temp.nome, temp.prontuario, temp.tipo == 1 ? "Master" : "Comum");
    }

    printf("+----------------------------------------------------------+--------------+--------+\n");

    fclose(arquivo);
}

void adicionarusuario() {
    registro *usuarios = NULL;
    int tamanho = 0;
    int opcao;

    lerUsuariosArquivo(&usuarios, &tamanho);

    registro novo;
    printf("\n");
    printf("\n\tInforme o nome do novo usuário:| ");
    fgets(novo.nome, sizeof(novo.nome), stdin);
    novo.nome[strcspn(novo.nome, "\n")] = 0;

    printf("\tInforme o prontuário do novo usuário:| ");
    fgets(novo.prontuario, sizeof(novo.prontuario), stdin);
    novo.prontuario[strcspn(novo.prontuario, "\n")] = 0;

    printf("\tTipo do usuário (1 - Master / 0 - Comum):| ");
    scanf("%d", &novo.tipo);
    getchar();

    // Redimensiona o vetor para adicionar o novo usuário
    usuarios = (registro *)realloc(usuarios, (tamanho + 1) * sizeof(registro));
    if (usuarios == NULL) {
        printf("Erro ao alocar memória.\n");
        exit(1);
    }

    usuarios[tamanho] = novo;
    tamanho++;

    ordenarusuarios(usuarios, tamanho);
    gravarUsuariosArquivo(usuarios, tamanho);

    free(usuarios);

    printf("\n\t[Usuário adicionado com sucesso!]\n");
    printf ("\n");
    
   printf("__________________________________________________________________________________________________________________________________________________________________________________________________\n");
    printf ("\n");
    printf ("\n");
}


void removerusuario() {
    char prontuario[10];

printf ("\n");
    printf("\n\tInforme o prontuário do usuário a ser removido:| ");
    fgets(prontuario, sizeof(prontuario), stdin);
    prontuario[strcspn(prontuario, "\n")] = 0;

    registro *usuarios = NULL;
    int tamanho = 0;

    lerUsuariosArquivo(&usuarios, &tamanho);

    int i,j, encontrado = 0;

    for (i = 0; i < tamanho; i++) {
        if (strcmp(usuarios[i].prontuario, prontuario) == 0) {
            encontrado = 1;
            printf("\n");
            printf("\t[Usuário %s removido!]\n", usuarios[i].nome);
            break;
        }
    }

    if (!encontrado) {
        printf("[!!!] Prontuário não encontrado.\n");
    } else {
        // Remove o usuário deslocando os elementos
        for ( j = i; j < tamanho - 1; j++) {
            usuarios[j] = usuarios[j + 1];
        }
        tamanho--;

        usuarios = (registro *)realloc(usuarios, tamanho * sizeof(registro));
        if (tamanho > 0 && usuarios == NULL) {
            printf("[!!!]Erro ao realocar memória.\n");
            exit(1);
        }

        ordenarusuarios(usuarios, tamanho); // Ordena só por segurança
        gravarUsuariosArquivo(usuarios, tamanho);
    }

    free(usuarios);
    printf ("\n");
    printf ("\n");
    printf("__________________________________________________________________________________________________________________________________________________________________________________________________\n");
}



//2º ETAPA ---- ABASTECIMENTO GONDOLAS


/*__

PILHA (VETOR)
__*/

// funções para munusear as pilhas(prateleiras)

void limpartela() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}//função de layout

void iniciopilha(pilha *p) { p->topo = -1; }
int vazia(pilha *p) { return p->topo == -1; }
int cheia(pilha *p) { return p->topo == maxitens - 1; }
int quantidadeItens(pilha *p) { return p->topo + 1; }
 
void empilhar(pilha *p, Produto produto) {
    if (cheia(p)) {
        printf("\n\t|A prateleira está cheia!|\n");
        return;
    }
   p->topo++;
    p->itens[p->topo] = produto;
    printf("\n\t__ Produto '%s' adicionado com sucesso! _\n", produto.nome);
}

Produto desempilhar(pilha *p) {
    Produto vazio = {"", "", 0.0, 0.0};
    
    if (vazia(p)) {
        printf("\n\t|A prateleira está vazia!|\n");
        return vazio;
    }
    Produto removido = p->itens[p->topo]; 
    p->topo--; 

    return removido;
}

/*__________

FUNÇÕES PARA AS PRATELEIRAS
__________*/

//Funções para manusear as prateleiras e seus produtos

void exibirPrateleira(pilha *p, int prateleiraID) {
	int i;
    printf("\nPRATELEIRA [%d]=========================================================\n", prateleiraID);
    if (vazia(p)) {
        printf("[Vazia]\n");
        return;
    }
    printf("%-20s %-30s %-10s %-10s\n", "Nome", "Descrição", "Peso", "Preço");
    printf("-----------------------------------------------------------------------\n");
    for ( i = p->topo; i >= 0; i--) {
        Produto prod = p->itens[i];
        printf("%-20s %-30s %-10.2f %-10.2f\n", prod.nome, prod.descricao, prod.peso, prod.preco);
    }
}

void exibirTodasPrateleiras() {
	int i;
	int largura=200;
    centralizar("\n----------------------------------------- TODAS AS PRATELEIRAS ------------------------------------------\n",largura);
    printf ("\n");
    printf ("\n");
    for ( i = 0; i < NUM_PRATELEIRAS; i++) {
        exibirPrateleira(&prateleiras[i], i);
    }
}

void inicializarTodasPrateleiras() {
	int i;
    carregarPrateleirasDeArquivo();  // Carrega os dados do arquivo
    for ( i = 0; i < NUM_PRATELEIRAS; i++) {
        if (vazia(&prateleiras[i])) {
            iniciopilha(&prateleiras[i]);  // Inicializa as prateleiras que estão vazias
        }
    }
}


/*________

FUNÇÕES PARA FUNCIONALIDADES
__________*/

//Funçoes para adicionar as funcionalidades de cada tipo de usuario

void abastecerPrateleira() {
	
    int prateleiraID;
    int largura=200;
    Produto novoProduto;
    
    limpartela ();
    printf ("\n");
    centralizar ("|Abastecer prateleiras|",largura);
    printf ("\n");
    
    do {
     printf("__________________________________________________________________________________________________________________________________________________________________________________________________\n");
    	printf ("\n");
    printf("\n... Digite o ID da prateleira (0 a %d) [10 para voltar ao menu]: ", NUM_PRATELEIRAS - 1);
    scanf("%d", &prateleiraID);
    getchar();
     
  if (prateleiraID == 10) //condição de saida
  {limpartela();
  return;}
  
        if (prateleiraID < 0 || prateleiraID >= NUM_PRATELEIRAS) {
            printf("\n|ID inválido!|\n");
            continue;
        }
    
   printf("\n|Nome do produto:| ");
   fgets(novoProduto.nome, sizeof(novoProduto.nome), stdin);
   novoProduto.nome[strcspn(novoProduto.nome, "\n")] = '\0'; // Remove o '\n'

   printf("|Descrição:| ");
   fgets(novoProduto.descricao, sizeof(novoProduto.descricao), stdin);
   novoProduto.descricao[strcspn(novoProduto.descricao, "\n")] = '\0';

    printf("|Peso (Kg):| ");
    scanf("%f", &novoProduto.peso);
    printf("|Preço (R$):| ");
    scanf("%f", &novoProduto.preco);
    getchar(); // Limpa buffer
    
    empilhar(&prateleiras[prateleiraID], novoProduto);
    exibirPrateleira(&prateleiras[prateleiraID], prateleiraID);
    
    salvarPrateleirasEmArquivo();
    
}while (1);}



void retirarProduto(Carrinho *carrinho) {
    int prateleiraID;
    limpartela();
    printf("\n|Produtos disponíveis:|\n\n");
    exibirTodasPrateleiras();

    char confirmacao;
    char input[10];
    
    while (1) {
        printf("\n");
        printf("\n\tDigite o ID da prateleira de onde deseja retirar um produto (ou -1 para sair):| ");
        fgets(input, sizeof(input), stdin);
        if (sscanf(input, "%d", &prateleiraID) != 1) {
            printf("\n\t[Entrada inválida! Digite um número válido.]\n");
            continue;
        }

        if (prateleiraID == -1) {
            printf("\n\t\t\t|Saindo da retirada de produtos.|\n");
            limpartela();
            return;
        }

        if (prateleiraID < 0 || prateleiraID >= NUM_PRATELEIRAS) {
            printf("\n\t[ID da prateleira inválido! Tente novamente.]\n");
            continue;
        }

        if (vazia(&prateleiras[prateleiraID])) {
            printf("\n\t[Não há produtos para retirar dessa prateleira. Escolha outra.]\n");
            continue;
        }

        Produto removido = desempilhar(&prateleiras[prateleiraID]);

        if (removido.nome[0] == '\0') {
            printf("\n\t[Erro ao retirar produto!]\n");
            continue;
        }

        printf("\t-- Produto removido: %s | Peso: %.2f | Preço: %.2f\n", removido.nome, removido.peso, removido.preco);

        // Adiciona ao carrinho usando a nova função
        adicionarAoCarrinho(carrinho, removido);

        printf ("\n");
        printf("\n\t[Produto adicionado ao carrinho!]\n");
    }
}

/*______

CARRINHO
______*/

void exibirCarrinho(Carrinho *carrinho) {
    int largura = 200;
    
    // Verifica se carrinho é nulo
    if (carrinho == NULL) {
        printf("[ERRO] Carrinho está NULL! A memória não foi alocada corretamente.\n");
        return;
    }

    // Verifica se o carrinho está vazio
    if (carrinho->topo == NULL) {
        printf("[!!!] O carrinho está vazio!\n");
        return;
    }

    printf("\n===== CARRINHO DE COMPRAS ===================================================================\n\n");
    printf("%-20s %-30s %-10s %-10s\n", "Nome", "Descrição", "Peso", "Preço");
    printf("-------------------------------------------------------------------------------------------\n");

    NoCarrinho *atual = carrinho->topo; // Ponteiro para percorrer a lista

    while (atual != NULL) {
        Produto prod = atual->produto;

        // Verifica se os dados do produto são válidos
        if (prod.nome[0] == '\0') {
            printf("[ERRO] Produto inválido encontrado!\n");
            exit(1); // Para o programa para evitar erro pior
        }

        printf("%-20s %-30s %-10.2f %-10.2f\n", prod.nome, prod.descricao, prod.peso, prod.preco);
        printf("__________________________________________________________________\n");

        atual = atual->proximo; // Move para o próximo item do carrinho
    }
}


void inicializarCarrinho(Carrinho *carrinho) {
    carrinho->topo = NULL;
}

void adicionarAoCarrinho(Carrinho *carrinho, Produto p) {
    NoCarrinho *novo = (NoCarrinho *)malloc(sizeof(NoCarrinho));
    if (novo == NULL) {
        printf("Erro de memória ao adicionar produto ao carrinho!\n");
        return;
    }
    novo->produto = p;
    novo->proximo = carrinho->topo;
    carrinho->topo = novo;
}

Produto removerDoCarrinho(Carrinho *carrinho) {
    Produto p = {"", "", 0, 0};
    if (carrinho->topo == NULL) {
        printf("O carrinho está vazio!\n");
        return p;
    }
    NoCarrinho *temp = carrinho->topo;
    p = temp->produto;
    carrinho->topo = temp->proximo;
    free(temp);
    return p;
}

void liberarCarrinho(Carrinho *carrinho) {
    NoCarrinho *atual = carrinho->topo;
    while (atual != NULL) {
        NoCarrinho *temp = atual;
        atual = atual->proximo;
        free(temp);
    }
    carrinho->topo = NULL;
}


/*_____

PRODUTOS.DAT
_____*/

// Função para salvar as prateleiras no arquivo
void salvarPrateleirasEmArquivo() {
	int i;
    FILE *file = fopen("produtos.dat", "wb");
    if (file == NULL) {
        printf("\nErro ao abrir o arquivo para salvar as prateleiras.\n");
        return;
    }

    for ( i = 0; i < NUM_PRATELEIRAS; i++) {
        fwrite(&prateleiras[i].topo, sizeof(int), 1, file); // Salva o topo da pilha
        fwrite(prateleiras[i].itens, sizeof(Produto), prateleiras[i].topo + 1, file); // Salva os produtos
    }
fflush(file);
    fclose(file);
    printf("\nPrateleiras salvas com sucesso!\n");
}

void carregarPrateleirasDeArquivo() {
	int i;
    FILE *file = fopen("produtos.dat", "rb");
    if (file == NULL) {
        printf("\nArquivo não encontrado. Criando prateleiras vazias...\n");
        inicializarTodasPrateleiras();
        return;
    }

    for ( i = 0; i < NUM_PRATELEIRAS; i++) {
        fread(&prateleiras[i].topo, sizeof(int), 1, file); // Lê o topo da pilha
        if (prateleiras[i].topo >= 0) { // Se houver produtos na pilha
            fread(prateleiras[i].itens, sizeof(Produto), prateleiras[i].topo + 1, file);
        }
    }
fflush(file);
    fclose(file);
    printf("\nPrateleiras carregadas com sucesso!\n");
}





/*_______

      MENUS
_______*/

void exibirmenumaster() {
    int largura = 200;
    int opcao;
    char buffer[10];
    do {
        centralizar("---------------------------------\n", largura);
        centralizar(" Sistema Gerenciador de Supermercado", largura);
        centralizar("---------------------------\n", largura);
        printf("\n");

        centralizar("+____________________________________+", largura);
        centralizar("¦           MENU PRINCIPAL           ¦", largura);
        centralizar("¦------------------------------------¦", largura);
        centralizar("¦ 1. Abastecimento de Gôndolas       ¦", largura);
        centralizar("¦ 2. Ver os produtos                 ¦", largura);
        centralizar("¦ 3. Gerenciar Usuários              ¦", largura);
        centralizar("¦ 0. Sair                            ¦", largura);
        centralizar("+____________________________________+", largura);
        printf("\n");

        centralizar("Escolha uma opção: ", largura);
        fgets(buffer, sizeof(buffer), stdin);
        opcao = atoi(buffer);
        
        switch (opcao) {
            case 1:
                abastecerPrateleira();
                break;
            case 2:
            	limpartela();
                exibirTodasPrateleiras();
                printf ("\n");
                printf ("Selecione enter (tecla) para voltar ao Menu");
                getchar();
                limpartela();
                break;
                case 3:
               menugerenciarusuarios();
                break;
            case 0:
                printf("Saindo do sistema. Até logo!\n");
                teladelogin();
                break;
            default:
                printf("Opção inválida. Tente novamente.\n");
        }
    } while (opcao != 0);
}


	void exibirMenuUsuarioComum() {
    Carrinho carrinho;
    inicializarCarrinho(&carrinho); // GARANTE QUE O CARRINHO ESTÁ INICIALIZADO
    
    int largura = 200;
    int opcao;
    char buffer[10];
    fila caixa;
    inicializarFila(&caixa);
    
    do {
        centralizar("---------------------------------\n", largura);
        centralizar("        Bem-vindo ao Mercado       ", largura);
        centralizar("---------------------------------\n", largura);
        printf("\n");

        centralizar("+____________________________________+", largura);
        centralizar("¦           MENU DO CLIENTE          ¦", largura);
        centralizar("¦------------------------------------¦", largura);
        centralizar("¦ 1. Ver e retirar produtos          ¦", largura);
        centralizar("¦ 2. Ver carrinho                    ¦", largura);
        centralizar("¦ 3. Ir para o caixa                 ¦", largura);
        centralizar("¦ 0. Sair                            ¦", largura);
        centralizar("+____________________________________+", largura);
        printf("\n");

        centralizar("Escolha uma opção: ", largura);
        fgets(buffer, sizeof(buffer), stdin);
        opcao = atoi(buffer);
        
        switch (opcao) {
            case 1:
                retirarProduto(&carrinho);
                break;
                
            case 2:
                limpartela();
                exibirCarrinho(&carrinho);  // Exibe o carrinho sem recriar a função
                printf("\n");
                printf("Selecione enter (tecla) para voltar ao menu");
                getchar();
                limpartela();
                break;
                
            case 3:
                limpartela();
                printf("\n[Antes de ir para o caixa, veja seu carrinho:]\n");
                exibirCarrinho(&carrinho);
                printf("\n");
                printf("\nDeseja finalizar a compra? (S/N): ");
                
                char confirmacao;
                scanf(" %c", &confirmacao);
                while (getchar() != '\n');  
                
                if (confirmacao == 'S' || confirmacao == 's') {
                    limpartela();
                    printf("\n");
                    
                    centralizar("=====================\n", largura);
                    centralizar("Caixa (PDV)", largura);
                    centralizar("=====================\n", largura);
                    
                    finalizarCompra(&carrinho); // Verifique se essa função está ajustada para lista encadeada
                    printf("\n");
                    
                    printf("\nTransação Aprovada? (S/N): ");
                    scanf(" %c", &confirmacao);
                    getchar();
                    
                    if (confirmacao == 'S' || confirmacao == 's') {
                        printf("\n");
                    printf("__________________________________________________________________________________________________________________________________________________________________________________________________\n");                        printf("\n");
                        printf("\n");
                        centralizar("Agradecemos a preferência", largura);
                        centralizar("VOLTE SEMPRE!", largura);
                        printf("\n");
                        centralizar("\t\t\t\t\t\tass. SuperMarket IFSP, seu conforto de A a Z :)", largura);
                        printf("\n");
                         printf("__________________________________________________________________________________________________________________________________________________________________________________________________\n");
                    }
                    getchar();
                    teladelogin();
                    
                } else {
                    printf("\nCompra cancelada. Voltando ao menu.\n");
                    getchar();
                    limpartela();
                }
                break;
                
            case 0:
                printf("Saindo do sistema. Até logo!\n");
                liberarCarrinho(&carrinho);  // Libera a memória alocada para o carrinho antes de sair
                break;
                
            default:
                printf("Opção inválida. Tente novamente.\n");
        }
    } while (opcao != 0);
}


void menugerenciarusuarios() {
   int largura = 200;
    int opcao;
    char buffer[10];
limpartela();
    do {
        centralizar("+____________________________________+", largura);
        centralizar("¦          GERENCIAR USUARIOS        ¦", largura);
        centralizar("¦------------------------------------¦", largura);
        centralizar("¦ 1. Ver usuarios                    ¦", largura);
        centralizar("¦ 2. Adicionar usuario               ¦", largura);
        centralizar("¦ 3. Remover usuario                 ¦", largura);
        centralizar("¦ 0. Sair                            ¦", largura);
        centralizar("+____________________________________+", largura);
        printf("\n");

        centralizar("Escolha uma opção: ", largura);
        fgets(buffer, sizeof(buffer), stdin);
        opcao = atoi(buffer);
printf ("\n");
 printf("__________________________________________________________________________________________________________________________________________________________________________________________________\n");
        switch (opcao) {
            case 1:
                listarusuarios();
                printf ("Selececione enter para visualizar o menu novamente. ");
                getchar ();
                break;
            case 2:
                adicionarusuario();
                break;
            case 3:
                removerusuario();
                break;
            case 0:
                printf("Voltando...\n");
                limpartela();
                exibirmenumaster();
                break;
            default:
                printf("Opção inválida!\n");
        }
    } while (opcao != 0);
}


//3º ETAPA ------------- CAIXA PDV

/*___
      FILA
____*/

//Funções para manusear a fila

void inicializarFila(fila *f) {
    f->frente = 0;
    f->tras = -1;
}

int filaVazia(fila *f) {
    return f->tras < f->frente;
}

void enfileirar(fila *f, Produto p) {
    if (f->tras < maxitens - 1) {
        f->itens[++f->tras] = p;
    }
}

Produto desenfileirar(fila *f) {
    Produto p = {"", "", 0, 0};
    if (!filaVazia(f)) {
        p = f->itens[f->frente++];
    }
    return p;
}


   void transferirParaFila(Carrinho *carrinho, fila *caixa) {
    Carrinho auxiliar;
    inicializarCarrinho(&auxiliar); // Inicializa a pilha auxiliar

    // Inverte a pilha original usando uma pilha auxiliar (listando os produtos na ordem correta)
    while (carrinho->topo != NULL) {
        Produto removido = removerDoCarrinho(carrinho);  // Usando a função de remoção de pilha encadeada
        adicionarAoCarrinho(&auxiliar, removido);  // Adiciona à pilha auxiliar 
    }

    // Transferir os produtos da pilha auxiliar para a fila
    while (auxiliar.topo != NULL) {
        Produto removido = removerDoCarrinho(&auxiliar);  // Remove da pilha auxiliar 
        enfileirar(caixa, removido);  // Coloca na fila
    }
}


void finalizarCompra(Carrinho *carrinho) {
    fila caixa;
    inicializarFila(&caixa);

    printf("\n Transferindo produtos do carrinho para a esteira...\n");
    printf("\n");
    sleep(1);
    
    // Passa os produtos da pilha (carrinho) para a fila (caixa)
    transferirParaFila(carrinho, &caixa);

    printf("\n....Produtos foram colocados na esteira! Iniciando o processamento...\n");
    sleep(1);

    // processa os produtos e gera o cupom fiscal
    processarCaixa(&caixa);
}


void processarCaixa(fila *caixa) {
    int largura = 200;
    FILE *file = fopen("cupom_fiscal.txt", "w");  // Abrindo o arquivo para gravação

    if (file == NULL) {
        printf("\nErro ao abrir o arquivo de cupom fiscal.\n");
        return;
    }
    
    float total = 0;

    // Imprime o título no arquivo e no terminal
    fprintf(file, "\n=========================== CUPOM FISCAL =============================\n");
    printf("\n=========================== CUPOM FISCAL =============================\n");

    // Processa os produtos da fila
    while (!filaVazia(caixa)) {
        Produto p = desenfileirar(caixa);  // Retira o produto da fila

        // Simula a esteira
        printf("____\nProduto sendo processado...\n____\n");
        sleep(1);  // Pequeno delay para simular movimento da esteira
        
        // Imprime no arquivo
        fprintf(file, "\nProduto: %s\n", p.nome);
        fprintf(file, "Preço:\t\t\t\t\tR$ %.2f\n", p.preco);
        fprintf(file, "Peso:\t\t\t\t\t%.2f kg\n", p.peso);
        fprintf(file, "Descrição:\t\t\t\t\t%s\n", p.descricao);
        fprintf(file, "-------------------------------------------------------------------------\n");

        // Imprime no terminal
        printf("\n   %s\n", p.nome);
        printf("   Preço:\t\t\t\t\tR$ %.2f\n", p.preco);
        printf("   Peso:\t\t\t\t\t%.2f kg\n", p.peso);
        printf("   Descrição:\t\t\t\t\t%s\n", p.descricao);
        printf("\n");

        total += p.preco;  // Soma o preço ao total
        sleep(1);  // Dá uma pausa antes do próximo produto
    }

    // Finaliza o cupom fiscal no arquivo
    fprintf(file, "-------------------------------------------------------------------------\n");
    fprintf(file, " TOTAL A PAGAR:\t\t\t\t\tR$ %.2f\n", total);
    fprintf(file, "-------------------------------------------------------------------------\n");

    // Imprime o total no terminal
    printf("-------------------------------------------------------------------------\n");
    printf(" TOTAL A PAGAR:\t\t\t\t\tR$ %.2f\n", total);
    printf("\n");
    printf("-------------------------------------------------------------------------\n");

    fclose(file);  // Fecha o arquivo após a escrita
    printf("\nCupom fiscal salvo com sucesso.\n");
}

