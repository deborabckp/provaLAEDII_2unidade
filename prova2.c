#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HASH_SIZE 50
#define MAX_CONTACTS 75

// Estrutura para armazenar um contato
typedef struct contatos {
    char Nome[50];
    char telefone[20];
    struct contatos *proxi;
} contatos;

// Estrutura para a tabela hash
typedef struct TabelaHash {
    contatos *tabela[HASH_SIZE];
} TabelaHash;

// Função de hash simples
unsigned int hash(const char *str) {
    unsigned int hash = 5381;
    int c;

    while ((c = *str++))
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

    return hash % HASH_SIZE;
}

// Função para inserir um contato na tabela hash
void inserir(TabelaHash *tabelahash, const char *Nome, const char *telefone) {
    unsigned int indice = hash(Nome);

    contatos *novocontato = (contatos *)malloc(sizeof(contatos));
    if (novocontato == NULL) {
        printf("Erro: Falha na alocação de memória\n");
        return;
    }

    strcpy(novocontato->Nome, Nome);
    strcpy(novocontato->telefone, telefone);
    novocontato->proxi = NULL;

    // Insere no início da lista encadeada
    novocontato->proxi = tabelahash->tabela[indice];
    tabelahash->tabela[indice] = novocontato;
}

// Função para buscar um contato na tabela hash
contatos *procurar(TabelaHash *tabelahash, const char *Nome) {
    unsigned int indice = hash(Nome);
    contatos *atual = tabelahash->tabela[indice];

    while (atual != NULL) {
        if (strcmp(atual->Nome, Nome) == 0)
            return atual;
        atual = atual->proxi;
    }

    return NULL; // Contato não encontrado
}

// Função para imprimir todos os contatos na tabela hash
void printcontatos(TabelaHash *tabelahash) {
    printf("Contatos:\n");
    for (int i = 0; i < HASH_SIZE; i++) {
        contatos *atual = tabelahash->tabela[i];
        while (atual != NULL) {
            printf("Nome: %s, Telefone: %s\n", atual->Nome, atual->telefone);
            atual = atual->proxi;
        }
    }
}

int main() {
    TabelaHash tabelahash;
    int opcao;
    char Nome[50], telefone[20];

    // Inicializa a tabela hash
    for (int i = 0; i < HASH_SIZE; i++)
        tabelahash.tabela[i] = NULL;

    // Exemplo de inserção manual de contatos
    inserir(&tabelahash, "Joao", "123456789");
    inserir(&tabelahash, "Maria", "987654321");

    do {
        printf("\nMenu:\n");
        printf("1. Inserir contato\n");
        printf("2. Buscar contato\n");
        printf("3. Imprimir contatos\n");
        printf("4. Sair\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                printf("Digite o Nome do contato: ");
                scanf("%s", Nome);
                printf("Digite o telefone do contato: ");
                scanf("%s", telefone);
                inserir(&tabelahash, Nome, telefone);
                break;
            case 2:
                printf("Digite o Nome do contato que deseja buscar: ");
                scanf("%s", Nome);
                contatos *resultado = procurar(&tabelahash, Nome);
                if (resultado != NULL)
                    printf("Telefone de %s: %s\n", Nome, resultado->telefone);
                else
                    printf("Contato não encontrado.\n");
                break;
            case 3:
                printcontatos(&tabelahash);
                break;
            case 4:
                printf("Saindo...\n");
                break;
            default:
                printf("Opção inválida.\n");
        }
    } while (opcao != 4);

    return 0;
}
