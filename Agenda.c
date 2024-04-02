#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAM 75

typedef struct Contato {
    char nome[50];
    char telefone[15];
    char email[50];
} Pessoa;

Pessoa agenda[TAM]; 
// Declaração da tabela de contatos

// Função para inicializar a tabela de contatos
void inicializarTabela(Pessoa tabela[]) {
    int i;
    for(i = 0; i < TAM; i++){
        strcpy(tabela[i].nome, "");
        strcpy(tabela[i].telefone, "");
        strcpy(tabela[i].email, "");
    }
}

// Função de dispersão para chaves inteiras
int funcaoHash(int chave) {
     return chave % TAM;
}

// Função de dispersão para strings
int funcaoHashString( char str[]){
    int i,tamS = strlen(str);
    unsigned int hash = 0;

    for(i=0;i<tamS;i++)
        hash +=str[i] * (i+1);
    return hash % TAM;
}

// Função para ler contatos de um arquivo
void lerArquivo(Pessoa tabela[]) {
    FILE *arq = fopen("todosOsContatos.txt", "r");
    if (arq == NULL) {
        printf("Falha ao abrir arquivo para leitura.\n");
        return;
    }
    fseek(arq, 0, SEEK_SET); 
    Pessoa contato;
    while (fscanf(arq, "Nome: %[^\n], Telefone: %[^\n], Email: %[^\n]\n", contato.nome, contato.telefone, contato.email) == 3)
        inserir(tabela, contato); // Insere o contato lido na tabela
    fclose(arq);
}

// Função para inserir um contato na tabela
void inserir(Pessoa tabela[], Pessoa p) {
    int id =  funcaoHashString(p.nome);
    while (strlen(tabela[id].nome) > 0) {
        id = (id + 1) % TAM; // Tratamento de colisões por sondagem linear
    }
    tabela[id] = p; // Insere o contato na tabela
}

// Função para buscar um contato na tabela
Pessoa *buscar(Pessoa tabela[], char chave[]) {
    int id = funcaoHashString(chave);
    printf("\nIndice gerada: %d\n",id);
    while (strlen(tabela[id].nome) > 0){
        if(strcmp(tabela[id].nome,chave) == 0){
            return &tabela[id]; // Retorna o endereço do contato se encontrado
        }else{
            id = funcaoHash(id +1); // Tratamento de colisões por sondagem linear
        }
    }
    return NULL;   // Retorna NULL se o contato não for encontrado
}

// Função para escrever todos os contatos da tabela em um arquivo
void escreverArquivo(Pessoa tabela[]) {
    FILE *arq = fopen("todosOsContatos.txt", "a");
    if (arq == NULL) {
        printf("Falha ao abrir arquivo para escrita.\n");
        return;
    }
    
    int i;
    for(i=0;i<TAM;i++){
        if(tabela[i].nome[0] != '\0'){
            fprintf(arq,"Nome: %s\nTelefone: %s\nEmail: %s\n", tabela[i].nome, tabela[i].telefone, tabela[i].email);
            fprintf(arq,"\n");
        }
    }
    fclose(arq);
}

// Função para imprimir todos os contatos da tabela
void imprimir(Pessoa tabela[]) {
    int i;
    printf("\n");
    for(i=0;i<TAM;i++){
        if (strlen(tabela[i].nome) > 0)
            printf("%d =\nNome: %s\nTelefone: %s\nE-mail: %s\n", i, tabela[i].nome, tabela[i].telefone, tabela[i].email);
    }
}

int main() {
    int opcao;
    Pessoa tabela[TAM], *pessoaBuscada;
    char nome[50], telefone[15], email[50];
    inicializarTabela(tabela); // Inicializa a tabela de contatos
    lerArquivo(tabela); // Lê os contatos do arquivo

    do {
        printf("\n0 - sair\n1 - inserir\n2 - buscar\n3 - imprimir\n\n");
        scanf("%d", &opcao);
        getchar(); 

        switch (opcao) {
        case 1:
            printf("Insira o nome do contato: ");
            fgets(nome, 49, stdin);
            printf("Insira o telefone do contato: ");
            fgets(telefone, 14, stdin);
            printf("Insira o email do contato: ");
            fgets(email, 49, stdin);
            Pessoa p;
            strcpy(p.nome, nome);
            strcpy(p.telefone, telefone);
            strcpy(p.email, email);
            inserir(tabela, p); // Insere o contato na tabela
            break;

        case 2:
            printf("Qual o nome do contato que deseja buscar: ");
            fgets(nome, 49, stdin);
            pessoaBuscada = buscar(tabela, nome); // Busca o contato na tabela
            if (pessoaBuscada != NULL) {
                printf("Nome: %s\nTelefone: %s\nEmail: %s\n", pessoaBuscada->nome, pessoaBuscada->telefone, pessoaBuscada->email);
            } else {
                printf("Contato não encontrado.\n");
            }
            break;
        case 3:
            imprimir(tabela); // Imprime todos os contatos da tabela
            break;
        default:
            printf("Opcao invalida\n");
            break;
        }
    } while(opcao != 0);

    escreverArquivo(tabela); // Escreve os contatos da tabela no arquivo

    return 0;
}
