#include <stdio.h> //Biblioteca padrão de entrada e saída, usada para printf, scanf etc
#include <stdlib.h> //Biblioteca padrão para funções utilitárias como system() e gerenciamento de memória
#include <string.h> //Biblioteca para manipulação de strings, como strlen, strcmp, strcspn.
#include <time.h> //Biblioteca para data e hora

// --- Definição da Estrutura de Contato ---
//Número máximo de contatos; caractéres do nome; caractéres do telefone
#define MAX_CONTATOS 100
#define MAX_NOME 50
#define MAX_TELEFONE 15
#define NOME_ARQUIVO "agenda.dat" // Nome do arquivo de dados

typedef struct { //cria um tipo chamado Contato, permitindo criar variáveis desse tipo
    char nome[MAX_NOME]; // Armazena o nome do contato
    char telefone[MAX_TELEFONE]; // Armazena o telefone do contato
    int ativo; // 1 para ativo, 0 para excluido/vazio
} Contato;

// Variável global para a agenda e o contador de contatos
Contato agenda[MAX_CONTATOS]; // Array que armazena todos os contatos da agenda
int total_contatos = 0; // Contador do número de contatos ativos, agenda cheia ou vazia

// --- Funções --- Protótipos são declarações que permitem usar funções antes de defini-las.
void mostrarMenu();
void carregarAgenda(); 
void salvarAgenda();
void inicializarAgenda();
int contemNumeros(const char *str);
int validarTelefone(const char *telefone);
void incluirContato();
void consultarContatos();
void consultarPorNome();
void consultarPorTelefone();
void consultarPorID();
void listarTodosContatos();
void excluirContato();

// --- Função Principal ---
int main() {
    int opcao; // Armazena a opção do usuário
    carregarAgenda(); // Carrega contatos do arquivo, se existir
    system("mode con: cols=82"); // Executa comando no Windows para definir a largura da janela do console (ou prompt de comando)
    system ("color E5"); // Altera a cor do console

    // --- Lógica de Data e Hora ---
    time_t t; // Declara uma variável chamada t do tipo time_t (dado aritmético)
    struct tm *infoTempo; // Declara um ponteiro "infoTempo" para uma estrutura do tipo struct tm - data e hora detalhada
    char buffer[80]; // Quantidade de caracteres do vetor
    time(&t); // Obtém o tempo atual em segundos desde 1970
    infoTempo = localtime(&t); // Converte para a estrutura de tempo local
    strftime(buffer, 80, "%d/%m/%Y %H:%M:%S", infoTempo); // Formato: Dia/Mês/Ano Hora:Min:Seg

    printf("\n ================================================================================\n");
    printf(" ==                                                                            ==\n");
    printf(" ==                              AGENDA DE CONTATOS                            ==\n");
    printf(" ==                                                                            ==\n");
    printf(" ==                         Data e hora: %s                   ==\n", buffer);
    printf(" ==                                                                            ==\n");
    printf(" ================================================================================\n");

    do {
        mostrarMenu();
        printf("                          Escolha uma opcao: ");
        if (scanf("%i", &opcao) != 1) { // Limpa o buffer de entrada se não ler um número inteiro
            while (getchar() != '\n');
            opcao = 0; // Define uma opção inválida
        }

        switch (opcao) { // Verifica a opção digitada pelo usuário
            case 1:
                system ("color FD");
                system("cls"); // Limpa a tela
                incluirContato(); // Chama a função 
                break;
            case 2:
                system ("color FD");
                consultarContatos();
                break;
            case 3:
                system ("color FD");
                excluirContato();
                break;
            case 4:
                system("cls");
                system ("color E5");
                salvarAgenda();
                printf("\n                          Saindo da agenda de contatos. Ate logo!\n");
                break;
            default:
                printf("\n                          Opcao invalida. Por favor, escolha uma opcao entre 1 e 4.\n");
        }
    } while (opcao != 4); // Repete até o usuário escolher sair.

    return 0;
}

//Carrega os contatos do arquivo binário (agenda.dat) para a memória (array agenda)
void carregarAgenda() {
    FILE *arquivo = fopen(NOME_ARQUIVO, "rb"); // Abre para leitura binária ('rb')
    
    if (arquivo == NULL) { // Se o arquivo não existir (primeira vez rodando), inicializa o array em memória
        printf("   Arquivo de dados nao encontrado. Iniciando nova agenda.\n");
        inicializarAgenda();
        return;
    }

    // Lê todo o array 'agenda' (MAX_CONTATOS de Contato) do arquivo
    size_t lidos = fread(agenda, sizeof(Contato), MAX_CONTATOS, arquivo);
    
    fclose(arquivo); // Lê todos os contatos do arquivo e fecha o arquivo

    total_contatos = 0;
    int i;
    for (i = 0; i < MAX_CONTATOS; i++) { // Reconta os contatos ativos após a leitura
        if (agenda[i].ativo == 1) {
            total_contatos++;
        }
    }
    
    printf("\n   Agenda carregada com sucesso! Total de contatos: %i.\n", total_contatos);
}

// Salvar os contatos da memória para o binário
void salvarAgenda() {
    FILE *arquivo = fopen(NOME_ARQUIVO, "wb"); // Abre para escrita binária ('wb')
    
    if (arquivo == NULL) { // Se não conseguir abrir
        printf("                          Erro ao abrir o arquivo para salvar. Os dados nao foram persistidos.\n");
        return;
    }

    // Escreve todo o array 'agenda' (MAX_CONTATOS de Contato) no arquivo
    fwrite(agenda, sizeof(Contato), MAX_CONTATOS, arquivo);
    
    fclose(arquivo);
    printf("                          Agenda salva com sucesso no disco (%s).\n", NOME_ARQUIVO);
}

// Inicializa o array de contatos
void inicializarAgenda() {
    int i;
    for (i = 0; i < MAX_CONTATOS; i++) {
        agenda[i].ativo = 0; // Marca todas as posições como vazias.
    }
}

// Opções do menu
void mostrarMenu() {
    printf("\n                          =========== MENU ===========\n");
    printf("                          1. Incluir Contato\n");
    printf("                          2. Consultar Contatos\n");
    printf("                          3. Excluir Contato\n");
    printf("                          4. Sair (e Salvar)\n");
    printf("                          ============================\n");
}

// Validação
int contemNumeros(const char *str) { // Função não pode modificar o conteúdo original, de ponteiro caractere, com variável local "str"
    int i;
    for (i = 0; str[i] != '\0'; i++) {
        // Verifica se o caractere é um dígito de '0' a '9'
        if (str[i] >= '0' && str[i] <= '9') {
            return 1; // Número encontrado!
        }
    }
    return 0; // Nenhum número encontrado
}

int validarTelefone(const char *telefone) {
    int i;
    for (i = 0; telefone[i] != '\0'; i++) {
        // Verifica se o caractere não é um dígito de 0 a 9
        if (telefone[i] < '0' || telefone[i] > '9') {
            return 0; // Caractere inválido encontrado
        }
    }
    return 1; // Todos os caracteres são válidos
}

// Função para incluir um novo contato
void incluirContato() {
    if (total_contatos >= MAX_CONTATOS) { // Verifica se a agenda está cheia
        printf("\n                          Agenda cheia! Nao e possivel incluir mais contatos.\n");
        return;
    }

    // Encontra a primeira posição livre (onde ativo é 0)
    int i;
    for (i = 0; i < MAX_CONTATOS; i++) {
        if (agenda[i].ativo == 0) {
            break;
        }
    }
    
    char nomeTemp[MAX_NOME]; //variável temporária
    char telTemp[MAX_TELEFONE];

    // Limpa o buffer antes de ler strings
    while (getchar() != '\n'); 

printf("\n                          ===== Incluir Contato ===== \n");
    printf("                          Nome: ");
    fgets(nomeTemp, MAX_NOME, stdin); // Lê nome
    nomeTemp[strcspn(nomeTemp, "\n")] = 0; // Remove '\n'

    if (strlen(nomeTemp) < 3) { // Nome muito curto
        printf("                          ERRO: Nome deve ter no minimo 3 caracteres.\n");
        return;
    }
    if (contemNumeros(nomeTemp)) { // Evitar números
        printf("                          ERRO: O nome nao pode conter numeros.\n");
        return;
    }

    printf("                          Telefone (somente numeros): ");
    fgets(telTemp, MAX_TELEFONE, stdin); // Lê telefone
    telTemp[strcspn(telTemp, "\n")] = 0; // Remove '\n'

    if (strlen(telTemp) < 8) { // Telefone muito curto
        printf("                          ERRO: Telefone deve ter no minimo 8 digitos.\n");
        return;
    }
    if (!validarTelefone(telTemp)) {
        printf("                          ERRO: O telefone deve conter apenas numeros (0-9).\n");
        return;
    }

    //se as validações passaram, copia e salva
    strcpy(agenda[i].nome, nomeTemp); 
    strcpy(agenda[i].telefone, telTemp);

    agenda[i].ativo = 1; // Marca contato como ativo
    total_contatos++; // Incrementa contador
    printf("Contato '%s' incluido com sucesso!\n", agenda[i].nome);
    system("cls");
}

// Função que mostra o sub-menu de consulta
void consultarContatos() {
    int opcao;
    
    do {
        system("cls"); 
        
        printf("\n                          ===== OPCOES DE CONSULTA =====\n");
        printf("                          1. Consultar por Nome\n");
        printf("                          2. Consultar por Telefone\n");
        printf("                          3. Consultar por ID/Indice\n");
        printf("                          4. Listar Todos os Contatos\n");
        printf("                          5. Voltar ao Menu Principal\n");
        printf("                          -------------------------------\n");
        printf("                          Escolha uma opcao: ");

        if (scanf("%i", &opcao) != 1) {
            printf("\n                          ERRO: Opcao invalida. Pressione ENTER para continuar...\n");
            while (getchar() != '\n'); // Limpa o buffer
            getchar(); // Espera o usuário pressionar Enter
            continue;
        }

        switch (opcao) {
            case 1:
                consultarPorNome();
                break;
            case 2:
                consultarPorTelefone();
                break;
            case 3:
                consultarPorID();
                break;
            case 4:
                listarTodosContatos(); 
                break;
            case 5:
                printf("\n                          Voltando ao Menu Principal...\n");
                break;
            default:
                printf("\n                          Opcao invalida. Tente novamente.\n");
                break;
        }

        if (opcao != 5) {
            printf("\n                          Pressione ENTER para continuar...\n");
            while (getchar() != '\n'); // Limpa o buffer
            getchar(); // Espera o usuário pressionar Enter
        }

    } while (opcao != 5);
}

void consultarPorNome() {
    char termo[MAX_NOME];
    int i, encontrado = 0;
    while (getchar() != '\n'); // Limpa o buffer

    printf("\n                          ===== Consultar por Nome =====\n");
    printf("                          Digite o nome ou parte dele: ");
    fgets(termo, MAX_NOME, stdin);
    termo[strcspn(termo, "\n")] = 0; // Remove '\n'

    printf("\n                          Resultados da busca por '%s':\n", termo);
    
    for (i = 0; i < MAX_CONTATOS; i++) {
        if (agenda[i].ativo == 1) {
            // strstr busca se 'termo' existe dentro da 'agenda[i].nome'
            // toupper/tolower para maiúsculas e minúsculas 
            if (strstr(agenda[i].nome, termo) != NULL) { 
                printf("                          ID [%i] | Nome: %s | Telefone: %s\n", 
                       i, agenda[i].nome, agenda[i].telefone);
                encontrado = 1;
            }
        }
    }

    if (!encontrado) {
        printf("                          Nenhum contato encontrado com o termo '%s'.\n", termo);
    }
}

void consultarPorTelefone() {
    char termo[MAX_TELEFONE];
    int i, encontrado = 0;
    while (getchar() != '\n'); // Limpa o buffer

    printf("\n                          ===== Consultar por Telefone =====\n");
    printf("                          Digite o numero ou parte dele: ");
    fgets(termo, MAX_TELEFONE, stdin);
    termo[strcspn(termo, "\n")] = 0;

    printf("\n                          Resultados da busca por '%s':\n", termo);

    for (i = 0; i < MAX_CONTATOS; i++) {
        if (agenda[i].ativo == 1) {
            if (strstr(agenda[i].telefone, termo) != NULL) { 
                printf("                          ID [%i] | Nome: %s | Telefone: %s\n", 
                       i, agenda[i].nome, agenda[i].telefone);
                encontrado = 1;
            }
        }
    }

    if (!encontrado) {
        printf("                          Nenhum contato encontrado com o telefone '%s'.\n", termo);
    }
}

void consultarPorID() {
    int id;
    printf("\n                          ===== Consultar por ID =====\n");
    printf("                          Digite o ID do contato (indice de 0 a %i): ", MAX_CONTATOS - 1);
    
    // Supondo que você use scanf para IDs
    if (scanf("%i", &id) != 1) { 
        printf("                          ERRO: Entrada invalida. Digite um numero.\n");
        while (getchar() != '\n'); // Limpa o buffer
        return;
    }

    if (id < 0 || id >= MAX_CONTATOS) {
        printf("                          ERRO: ID fora do limite da agenda.\n");
    } else if (agenda[id].ativo == 1) {
        printf("\n--- Contato Encontrado ---\n");
        printf("                          ID [%i]\n", id);
        printf("                          Nome: %s\n", agenda[id].nome);
        printf("                          Telefone: %s\n", agenda[id].telefone);
    } else {
        printf("                          ID [%i] nao possui um contato ativo.\n", id);
    }
}

void listarTodosContatos() {
    system("cls");
    if (total_contatos == 0) { // Se não houver contatos
        printf("\n                          Agenda vazia. Nao ha contatos para listar.\n");
        return;
    }

    printf("\n   ================== Lista de Contatos (%i encontrado(s)) ==================   \n", total_contatos);
    printf("    ID   | Nome                                           | Telefone\n");
    printf("   ------|------------------------------------------------|-----------------   \n");
    int encontrado = 0;
    int i;
    for (i = 0; i < MAX_CONTATOS; i++) {
        if (agenda[i].ativo == 1) {
            printf("     %3i | %-44s   | %s\n", i + 1, agenda[i].nome, agenda[i].telefone);
            // %-44s alinha o nome à esquerda em um espaço de 44 caracteres.
            encontrado = 1;
        }
    }
    if (!encontrado) {
         printf("                          Nenhum contato ativo para exibir.\n");
    }
    printf("   -------------------------------------------------------------------------   \n");
}

// Função para excluir um contato (marcar como inativo)
void excluirContato() {
    int id;

    if (total_contatos == 0) { // Verifica se há contatos
        printf("\n                          Agenda vazia. Nao ha contatos para excluir.\n");
        return;
    }
    
    // Lista os contatos para que o usuário escolha o ID
    consultarContatos(); 
    
    printf("\n                          ===== Excluir Contato ===== \n");
    printf("\n                          Digite o ID do contato (1 a %i): ", MAX_CONTATOS);
    if (scanf("%i", &id) != 1) { // Verifica se digitou um número
        printf("                          ID invalido.\n");
        // Limpa o buffer de entrada
        while (getchar() != '\n');
        return;
    }

    // Ajusta o ID para o índice do array (ID 1 -> índice 0)
    int indice = id - 1;

    if (indice >= 0 && indice < MAX_CONTATOS && agenda[indice].ativo == 1) { //Permite ao usuário confirmar exclusão
        printf("                          Tem certeza que deseja excluir\n                          o contato '%s'? (s/n): ", agenda[indice].nome);
        char confirmacao;
        // Limpa o buffer
        while (getchar() != '\n'); // Limpa o buffer
        scanf(" %c", &confirmacao);

        if (confirmacao == 's' || confirmacao == 'S') { 
            agenda[indice].ativo = 0; // Marca como inativo (excluído)
            
            total_contatos--; // Decrementa contador
            printf("                          Contato '%s' excluido com sucesso!\n", agenda[indice].nome);
        } else {
            printf("                          Exclusao cancelada.\n");
        }
    } else {
        printf("                          ID %d invalido ou contato ja excluido.\n", id);
    }
    system("cls");
}