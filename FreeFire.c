#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

// Código da Ilha – Edição Free Fire
// Nível: Mestre
// Este programa simula o gerenciamento avançado de uma mochila com componentes coletados durante a fuga de uma ilha.
// Ele introduz ordenação com critérios e busca binária para otimizar a gestão dos recursos.

#define MAX_ITENS 10
#define TAM_NOME 30
#define TAM_TIPO 20

// Struct Item:
// Representa um componente com nome, tipo, quantidade e prioridade (1 a 5).
// A prioridade indica a importância do item na montagem do plano de fuga.
typedef struct {
    char nome[TAM_NOME];
    char tipo[TAM_TIPO];
    int quantidade;
    int prioridade;
} Item;

// Enum CriterioOrdenacao:
// Define os critérios possíveis para a ordenação dos itens (nome, tipo ou prioridade).
typedef enum {
    ORDENAR_POR_NOME = 1,
    ORDENAR_POR_TIPO = 2,
    ORDENAR_POR_PRIORIDADE = 3
} CriterioOrdenacao;

// Vetor mochila:
// Armazena até 10 itens coletados.
// Variáveis de controle: numItens (quantidade atual), comparacoes (análise de desempenho), ordenadaPorNome (para controle da busca binária).
Item mochila[MAX_ITENS];
int numItens = 0;
int comparacoes = 0;
bool ordenadaPorNome = false;

void limparTela(void);
void limparBufferEntrada(void);
void pausar(void);
void lerTexto(const char mensagem[], char destino[], int tamanho);
int lerInteiro(const char mensagem[], int minimo, int maximo);
void exibirMenu(void);
void inserirItem(void);
void removerItem(void);
void listarItens(void);
void menuDeOrdenacao(void);
void insertionSort(Item vetor[], int tamanho, CriterioOrdenacao criterio);
int compararItens(Item atual, Item chave, CriterioOrdenacao criterio);
void buscaBinariaPorNome(void);
void exibirItem(Item item);

int main() {
    int opcao;

    // Menu principal com opções:
    // 1. Adicionar um item
    // 2. Remover um item
    // 3. Listar todos os itens
    // 4. Ordenar os itens por critério (nome, tipo, prioridade)
    // 5. Realizar busca binária por nome
    // 0. Sair

    // A estrutura switch trata cada opção chamando a função correspondente.
    // A ordenação e busca binária exigem que os dados estejam bem organizados.
    do {
        limparTela();
        exibirMenu();
        opcao = lerInteiro("Escolha uma opcao: ", 0, 5);

        switch (opcao) {
            case 1:
                inserirItem();
                pausar();
                break;
            case 2:
                removerItem();
                pausar();
                break;
            case 3:
                listarItens();
                pausar();
                break;
            case 4:
                menuDeOrdenacao();
                pausar();
                break;
            case 5:
                buscaBinariaPorNome();
                pausar();
                break;
            case 0:
                printf("\nTorre de fuga ativada. Encerrando o sistema...\n");
                break;
            default:
                printf("\nOpcao invalida. Tente novamente.\n");
                pausar();
                break;
        }
    } while (opcao != 0);

    return 0;
}

// limparTela():
// Simula a limpeza da tela imprimindo várias linhas em branco.
void limparTela(void) {
    for (int i = 0; i < 25; i++) {
        printf("\n");
    }
}

// Função auxiliar para limpar o restante da linha após leituras numéricas.
void limparBufferEntrada(void) {
    int caractere;

    while ((caractere = getchar()) != '\n' && caractere != EOF) {
        // Apenas descarta os caracteres restantes no buffer.
    }
}

// Função auxiliar para pausar a execução e permitir a leitura das mensagens.
void pausar(void) {
    printf("\nPressione Enter para continuar...");
    getchar();
}

// Função auxiliar para ler textos com segurança usando fgets.
void lerTexto(const char mensagem[], char destino[], int tamanho) {
    printf("%s", mensagem);

    if (fgets(destino, tamanho, stdin) != NULL) {
        destino[strcspn(destino, "\n")] = '\0';
    }
}

// Função auxiliar para ler números inteiros dentro de um intervalo permitido.
int lerInteiro(const char mensagem[], int minimo, int maximo) {
    int valor;
    int leituraValida;

    do {
        printf("%s", mensagem);
        leituraValida = scanf("%d", &valor);
        limparBufferEntrada();

        if (leituraValida != 1 || valor < minimo || valor > maximo) {
            printf("Entrada invalida. Digite um numero entre %d e %d.\n", minimo, maximo);
        }
    } while (leituraValida != 1 || valor < minimo || valor > maximo);

    return valor;
}

// exibirMenu():
// Apresenta o menu principal ao jogador, com destaque para status da ordenação.
void exibirMenu(void) {
    printf("==========================================================\n");
    printf("       PLANO DE FUGA - CODIGO DA ILHA (NIVEL MESTRE)\n");
    printf("==========================================================\n");
    printf("Itens na Mochila: %d/%d\n", numItens, MAX_ITENS);
    printf("Status da Ordenacao por Nome: %s\n\n", ordenadaPorNome ? "ORDENADO" : "NAO ORDENADO");

    printf("1. Adicionar Componente\n");
    printf("2. Descartar Componente\n");
    printf("3. Listar Componentes (Inventario)\n");
    printf("4. Organizar Mochila (Ordenar Componentes)\n");
    printf("5. Busca Binaria por Componente-Chave (por nome)\n");
    printf("0. ATIVAR TORRE DE FUGA (Sair)\n");
    printf("----------------------------------------------------------\n");
}

// inserirItem():
// Adiciona um novo componente à mochila se houver espaço.
// Solicita nome, tipo, quantidade e prioridade.
// Após inserir, marca a mochila como "não ordenada por nome".
void inserirItem(void) {
    if (numItens >= MAX_ITENS) {
        printf("\nA mochila esta cheia. Descarte algum componente antes de coletar outro.\n");
        return;
    }

    printf("\n--- Coletando Novo Componente ---\n");
    lerTexto("Nome: ", mochila[numItens].nome, TAM_NOME);
    lerTexto("Tipo (Estrutural, Eletronico, Energia, etc.): ", mochila[numItens].tipo, TAM_TIPO);
    mochila[numItens].quantidade = lerInteiro("Quantidade: ", 1, 999);
    mochila[numItens].prioridade = lerInteiro("Prioridade de Montagem (1-5): ", 1, 5);

    printf("\nComponente '%s' adicionado com sucesso!\n", mochila[numItens].nome);
    numItens++;
    ordenadaPorNome = false;

    listarItens();
}

// removerItem():
// Permite remover um componente da mochila pelo nome.
// Se encontrado, reorganiza o vetor para preencher a lacuna.
void removerItem(void) {
    char nomeBusca[TAM_NOME];
    bool encontrado = false;

    if (numItens == 0) {
        printf("\nA mochila esta vazia. Nao ha componentes para descartar.\n");
        return;
    }

    printf("\n--- Descartar Componente ---\n");
    lerTexto("Digite o nome do componente que deseja descartar: ", nomeBusca, TAM_NOME);

    for (int i = 0; i < numItens; i++) {
        if (strcmp(mochila[i].nome, nomeBusca) == 0) {
            encontrado = true;

            for (int j = i; j < numItens - 1; j++) {
                mochila[j] = mochila[j + 1];
            }

            numItens--;
            ordenadaPorNome = false;
            printf("\nComponente '%s' descartado com sucesso!\n", nomeBusca);
            listarItens();
            break;
        }
    }

    if (!encontrado) {
        printf("\nComponente '%s' nao encontrado na mochila.\n", nomeBusca);
    }
}

// listarItens():
// Exibe uma tabela formatada com todos os componentes presentes na mochila.
void listarItens(void) {
    if (numItens == 0) {
        printf("\nA mochila esta vazia. Nenhum componente foi coletado.\n");
        return;
    }

    printf("\n--- COMPONENTES NA MOCHILA (%d/%d) ---\n", numItens, MAX_ITENS);
    printf("---------------------------------------------------------------\n");
    printf("%-4s | %-29s | %-19s | %-5s | %-10s\n", "ID", "NOME", "TIPO", "QTD", "PRIORIDADE");
    printf("---------------------------------------------------------------\n");

    for (int i = 0; i < numItens; i++) {
        printf("%-4d | %-29s | %-19s | %-5d | %-10d\n",
               i + 1,
               mochila[i].nome,
               mochila[i].tipo,
               mochila[i].quantidade,
               mochila[i].prioridade);
    }

    printf("---------------------------------------------------------------\n");
}

// menuDeOrdenacao():
// Permite ao jogador escolher como deseja ordenar os itens.
// Utiliza a função insertionSort() com o critério selecionado.
// Exibe a quantidade de comparações feitas (análise de desempenho).
void menuDeOrdenacao(void) {
    int opcao;

    if (numItens < 2) {
        printf("\nE necessario ter pelo menos dois componentes para ordenar a mochila.\n");
        return;
    }

    printf("\n--- Estrategia de Organizacao ---\n");
    printf("Como deseja ordenar os componentes?\n");
    printf("1. Por Nome (Ordem Alfabetica)\n");
    printf("2. Por Tipo\n");
    printf("3. Por Prioridade de Montagem\n");
    printf("0. Cancelar\n");

    opcao = lerInteiro("Escolha o criterio: ", 0, 3);

    if (opcao == 0) {
        printf("\nOrdenacao cancelada.\n");
        return;
    }

    comparacoes = 0;
    insertionSort(mochila, numItens, (CriterioOrdenacao) opcao);

    if (opcao == ORDENAR_POR_NOME) {
        ordenadaPorNome = true;
        printf("\nMochila organizada por nome. A busca binaria esta liberada.\n");
    } else {
        ordenadaPorNome = false;
        printf("\nMochila organizada, mas nao por nome. A busca binaria continua bloqueada.\n");
    }

    printf("Comparacoes realizadas na ordenacao: %d\n", comparacoes);
    listarItens();
}

// insertionSort():
// Implementação do algoritmo de ordenação por inserção.
// Funciona com diferentes critérios de ordenação:
// - Por nome (ordem alfabética)
// - Por tipo (ordem alfabética)
// - Por prioridade (da mais alta para a mais baixa)
void insertionSort(Item vetor[], int tamanho, CriterioOrdenacao criterio) {
    for (int i = 1; i < tamanho; i++) {
        Item chave = vetor[i];
        int j = i - 1;

        while (j >= 0 && compararItens(vetor[j], chave, criterio) > 0) {
            vetor[j + 1] = vetor[j];
            j--;
        }

        vetor[j + 1] = chave;
    }
}

// Função auxiliar usada pelo insertionSort para comparar dois itens de acordo com o critério escolhido.
int compararItens(Item atual, Item chave, CriterioOrdenacao criterio) {
    comparacoes++;

    switch (criterio) {
        case ORDENAR_POR_NOME:
            return strcmp(atual.nome, chave.nome);
        case ORDENAR_POR_TIPO:
            return strcmp(atual.tipo, chave.tipo);
        case ORDENAR_POR_PRIORIDADE:
            // Prioridade mais alta primeiro: 5 vem antes de 4, 3, 2 e 1.
            return chave.prioridade - atual.prioridade;
        default:
            return 0;
    }
}

// buscaBinariaPorNome():
// Realiza busca binária por nome, desde que a mochila esteja ordenada por nome.
// Se encontrar, exibe os dados do item buscado.
// Caso contrário, informa que não encontrou o item.
void buscaBinariaPorNome(void) {
    char nomeBusca[TAM_NOME];
    int inicio = 0;
    int fim = numItens - 1;
    bool encontrado = false;

    if (numItens == 0) {
        printf("\nA mochila esta vazia. Nao ha componentes para buscar.\n");
        return;
    }

    if (!ordenadaPorNome) {
        printf("\nALERTA: A busca binaria requer que a mochila esteja ordenada por NOME.\n");
        printf("Use a Opcao 4 para organizar a mochila primeiro.\n");
        return;
    }

    printf("\n--- Busca Binaria por Componente-Chave ---\n");
    lerTexto("Nome do componente a buscar: ", nomeBusca, TAM_NOME);

    comparacoes = 0;

    while (inicio <= fim) {
        int meio = inicio + (fim - inicio) / 2;
        int resultadoComparacao = strcmp(mochila[meio].nome, nomeBusca);
        comparacoes++;

        if (resultadoComparacao == 0) {
            printf("\nCOMPONENTE-CHAVE ENCONTRADO!\n");
            exibirItem(mochila[meio]);
            printf("Comparacoes realizadas na busca binaria: %d\n", comparacoes);
            printf("\nA presenca desse componente confirma que a torre pode seguir para a proxima etapa.\n");
            encontrado = true;
            break;
        } else if (resultadoComparacao < 0) {
            inicio = meio + 1;
        } else {
            fim = meio - 1;
        }
    }

    if (!encontrado) {
        printf("\nComponente-chave '%s' nao encontrado.\n", nomeBusca);
        printf("Comparacoes realizadas na busca binaria: %d\n", comparacoes);
        printf("Verifique o inventario antes de tentar ativar a torre de fuga.\n");
    }
}

// Função auxiliar para exibir os dados completos de um único item encontrado.
void exibirItem(Item item) {
    printf("Nome: %s\n", item.nome);
    printf("Tipo: %s\n", item.tipo);
    printf("Quantidade: %d\n", item.quantidade);
    printf("Prioridade: %d\n", item.prioridade);
}