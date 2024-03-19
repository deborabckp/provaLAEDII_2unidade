#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HASH_SIZE 50
#define MAX_CONTACTS 75

// Estrutura para armazenar um contato
typedef struct Contact {
    char name[50];
    char phone[20];
    struct Contact *next;
} Contact;

// Estrutura para a tabela hash
typedef struct HashTable {
    Contact *table[HASH_SIZE];
} HashTable;

// Função de hash simples
unsigned int hash(const char *str) {
    unsigned int hash = 5381;
    int c;

    while ((c = *str++))
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

    return hash % HASH_SIZE;
}

// Função para inserir um contato na tabela hash
void insertContact(HashTable *hashTable, const char *name, const char *phone) {
    unsigned int index = hash(name);

    Contact *newContact = (Contact *)malloc(sizeof(Contact));
    if (newContact == NULL) {
        printf("Erro: Falha na alocação de memória\n");
        return;
    }

    strcpy(newContact->name, name);
    strcpy(newContact->phone, phone);
    newContact->next = NULL;

    // Insere no início da lista encadeada
    newContact->next = hashTable->table[index];
    hashTable->table[index] = newContact;
}

// Função para buscar um contato na tabela hash
Contact *searchContact(HashTable *hashTable, const char *name) {
    unsigned int index = hash(name);
    Contact *current = hashTable->table[index];

    while (current != NULL) {
        if (strcmp(current->name, name) == 0)
            return current;
        current = current->next;
    }

    return NULL; // Contato não encontrado
}

// Função para imprimir todos os contatos na tabela hash
void printContacts(HashTable *hashTable) {
    printf("Contatos:\n");
    for (int i = 0; i < HASH_SIZE; i++) {
        Contact *current = hashTable->table[i];
        while (current != NULL) {
            printf("Nome: %s, Telefone: %s\n", current->name, current->phone);
            current = current->next;
        }
    }
}

int main() {
    HashTable hashTable;
    int option;
    char name[50], phone[20];

    // Inicializa a tabela hash
    for (int i = 0; i < HASH_SIZE; i++)
        hashTable.table[i] = NULL;

    // Exemplo de inserção manual de contatos
    insertContact(&hashTable, "Joao", "123456789");
    insertContact(&hashTable, "Maria", "987654321");

    do {
        printf("\nMenu:\n");
        printf("1. Inserir contato\n");
        printf("2. Buscar contato\n");
        printf("3. Imprimir contatos\n");
        printf("4. Sair\n");
        printf("Escolha uma opção: ");
        scanf("%d", &option);

        switch (option) {
            case 1:
                printf("Digite o nome do contato: ");
                scanf("%s", name);
                printf("Digite o telefone do contato: ");
                scanf("%s", phone);
                insertContact(&hashTable, name, phone);
                break;
            case 2:
                printf("Digite o nome do contato que deseja buscar: ");
                scanf("%s", name);
                Contact *result = searchContact(&hashTable, name);
                if (result != NULL)
                    printf("Telefone de %s: %s\n", name, result->phone);
                else
                    printf("Contato não encontrado.\n");
                break;
            case 3:
                printContacts(&hashTable);
                break;
            case 4:
                printf("Saindo...\n");
                break;
            default:
                printf("Opção inválida.\n");
        }
    } while (option != 4);

    return 0;
}
