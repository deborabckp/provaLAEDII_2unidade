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

void inicializarTabela(Pessoa tabela[]) {
    int i;
    for(i = 0; i < TAM; i++){
        strcpy(tabela[i].nome, "");
        strcpy(tabela[i].telefone, "");
        strcpy(tabela[i].email, "");
    }
}

int funcaoHash(int chave) {
     return chave % TAM;
}

int funcaoHashString( char str[]){
    int i,tamS = strlen(str);
    unsigned int hash = 0;

    for(i=0;i<tamS;i++)
        hash +=str[i] * (i+1);
    return hash % TAM;
}

void inserir(Pessoa tabela[], Pessoa p) {
    int id =  funcaoHashString(p.nome);
    while (strlen(tabela[id].nome) > 0) {
        id = (id + 1) % TAM; 
    }
    tabela[id] = p;
}
void lerArquivo(Pessoa tabela[]) {
    FILE *arq = fopen("todosOsContatos.txt", "r");
    if (arq == NULL) {
        printf("Falha ao abrir arquivo para leitura.\n");
        
    }
    fseek(arq, 0, SEEK_SET); 
    Pessoa contato;
    while (fscanf(arq, "Nome: %[^\n], Telefone: %[^\n], Email: %[^\n]\n", contato.nome, contato.telefone, contato.email) == 3)
        inserir(tabela, contato); 
    fclose(arq);
}

Pessoa *buscar(Pessoa tabela[], char chave[]) {
    int id = funcaoHashString(chave);
    printf("\nIndice gerada: %d\n",id);
        //se for maior que zero, significa a tabela contém algum texto
    while (strlen(tabela[id].nome) > 0){
        //comparando a strinfo com a chave para ver se são iguais
        if(strcmp(tabela[id].nome,chave) == 0){
            return &tabela[id]; 
        }else{
            id = funcaoHash(id +1); 
        }
    }
    return NULL;   
}

void escreverArquivo(Pessoa tabela[]) {
    FILE *arq = fopen("todosOsContatos.txt", "a");
    if (arq == NULL) {
        printf("Falha ao abrir arquivo para escrita.\n");
        return;
    }
    
    int i;
    for(i=0;i<TAM;i++){
        if(strlen(tabela[i].nome) > 0){
            fprintf(arq,"Nome: %sTelefone: %sEmail: %s\n", tabela[i].nome, tabela[i].telefone, tabela[i].email);
            
        }
    }
    fclose(arq);
}

void imprimir(Pessoa tabela[]) {
    int i;
        for(i=0;i<TAM;i++){
        if (strlen(tabela[i].nome) > 0)
            printf("%d =\nNome: %s\nTelefone: %s\nE-mail: %s\n", i, tabela[i].nome, tabela[i].telefone, tabela[i].email);
    }
}

int main() {
    int opcao;
    Pessoa tabela[TAM], *pessoaBuscada;
    char nome[50], telefone[15], email[50];
    inicializarTabela(tabela); 
    lerArquivo(tabela); 

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
            inserir(tabela, p); 
            break;

        case 2:
            printf("Qual o nome do contato que deseja buscar: ");
            fgets(nome, 49, stdin);
            pessoaBuscada = buscar(tabela, nome);
            if (pessoaBuscada != NULL) {
                printf("Nome: %s\nTelefone: %s\nEmail: %s\n", pessoaBuscada->nome, pessoaBuscada->telefone, pessoaBuscada->email);
            } else {
                printf("Contato não encontrado.\n");
            }
            break;
        case 3:
            imprimir(tabela); 
            break;
        default:
            printf("Opcao invalida\n");
            break;
        }
    } while(opcao != 0);

    escreverArquivo(tabela); 

    return 0;
}
