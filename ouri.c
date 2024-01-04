#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#define ROWS 2
#define COLS 6

//variaveis globais
int board[ROWS][COLS];
int depositoA = 0, depositoB = 0;

// Define jogadores
const char jogador1 = '1';
const char jogador2 = '2';
const char computador = 'C';

// Inicializar o tabuleiro
void iniciarTabuleiro(int board[ROWS][COLS]) {
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            board[i][j] = 4;
        }
    }
}


// Escolher PvP ou PvC;
int escolherModoJogo() {
    int escolha;

    do {
        printf("Escolha o modo de jogo:\n");
        printf("1. Jogador 1 vs. Jogador 2\n");
        printf("2. Jogador 1 vs. Computador\n");
        printf("Escolha: ");
        scanf("%d", &escolha);

        if (escolha < 1 || escolha > 2) {
            printf("Escolha inválida! Tente novamente.\n");
        }

    } while (escolha < 1 || escolha > 2);

    return escolha;
}

void guardarTabuleiro() {
    FILE *arquivo;
    char nomeArquivo[25];

    printf("Insira o nome do arquivo para guardar o tabuleiro: ");
    scanf("%s", nomeArquivo);

    arquivo = fopen(nomeArquivo, "w");

    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        exit(1);
    }

    fprintf(arquivo, "%d\n", depositoA);
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            fprintf(arquivo, "%d ", board[i][j]);
        }
        fprintf(arquivo, "\n");
    }
    fprintf(arquivo, "%d\n", depositoB);

    fclose(arquivo);

    printf("Tabuleiro guardado com sucesso! O programa será encerrado.\n");
    exit(0);
}

void carregarTabuleiro(char nomeArquivo[25]) {
    FILE *arquivo;
    arquivo = fopen(nomeArquivo, "r");

    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        exit(1);
    }

    fscanf(arquivo, "%d", &depositoA);
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            fscanf(arquivo, "%d", &board[i][j]);
        }
    }
    fscanf(arquivo, "%d", &depositoB);

    fclose(arquivo);
}

// Exibir o tabuleiro
void printBoard() {
    printf("\n|---|--|--|--|--|--|--|---|\n");
    printf("|   | %d| %d| %d| %d| %d| %d|   |", board[0][0], board[0][1], board[0][2], board[0][3], board[0][4], board[0][5]);
    printf("\n| %d |-----------------| %d |\n", depositoB, depositoA);
    printf("|   | %d| %d| %d| %d| %d| %d|   |", board[1][0], board[1][1], board[1][2], board[1][3], board[1][4], board[1][5]);
    printf("\n|---|--|--|--|--|--|--|---|\n");
    printf("\n");
}

// Definir jogador
char trocarJogador(char currentPlayer) {
    return (currentPlayer == jogador1) ? jogador2 : jogador1;
}

// Função de captura de pontos
void capturarPontos(int board[ROWS][COLS], char player, int i, int x) {
    int linha = i;
    int coluna = x;

    int pedra = board[linha][x];

    if ((pedra == 2 || pedra == 3)) {
        if (i == 0) {
            if (player == jogador1) {
                depositoA += pedra;
                board[i][x] = 0;
                while (coluna > 0 && (board[linha][coluna - 1] == 2 || board[linha][coluna - 1] == 3)) {
                    pedra = board[linha][coluna - 1];
                    if (player == jogador1) {
                        depositoA += pedra;
                        board[linha][coluna - 1] = 0;
                    }
                }
            }
        } else if (i == 1) {
            if (player == jogador2 || player == computador) {
                depositoB += pedra;
                board[i][x] = 0;
                while (coluna > 0 && (board[linha][coluna - 1] == 2 || board[linha][coluna - 1] == 3)) {
                    pedra = board[linha][coluna - 1];
                    if (i == 0 && (player == jogador2 || player == computador)) {
                        depositoB += pedra;
                        board[linha][coluna - 1] = 0;
                    }
                }
            }
        }
    }
}

int linhaAdversarioVazia(int board[ROWS][COLS], char currentPlayer) {
    int linhaAdversario = (currentPlayer == jogador1) ? 1 : 0;

    for (int col = 0; col < COLS; col++) {
        if (board[linhaAdversario][col] != 0) {
            return 0; // linha não vazia
        }
    }

    return 1; // linha vazia
}

// Verifica se é possível realizar a jogada com a quantidade de pedras fornecida
int verificarPedraSuficiente(int board[ROWS][COLS], int i, int j, int quantidadePedras, char currentPlayer) {
    int coluna = j;
    char player;

    if (i == 0) {
        player = jogador1;
    } else if (i == 1 && currentPlayer == jogador2) {
        player = jogador2;
    } else {
        player = computador;
    }

    int linha = i;

    if (linhaAdversarioVazia(board, player)) {
        while (quantidadePedras > 0) {
            if (coluna < 0 || coluna >= COLS) {
                return 0; // out of bounds
            }

            if (board[linha][coluna] < COLS) {
                return 0; // não possui pedras suficientes
            }

            quantidadePedras--;
            coluna++;

            if (coluna == COLS && quantidadePedras > 0) {
                linha = (linha == 0) ? 1 : 0;
                coluna = 0;
            }
        }

        return 1; // Jogada possível           
    } else {
        return 1;
    }
}


void movimento(int aux, int i, int x){
    int y = i;
    int z = x;
    int aux2 = aux;

    while (aux > 0) {

        // Verificar qual a linha atual
        if (i == 0) {
            x--;
        } else if (i == 1) {
            x++;
        }

        // Check inBounds
        if (x > 5) {
            i = 0;
            x--;
        } else if (x < 0) {
            i = 1;
            x++;
        }

        board[i][x]++;
        aux--;

        if (aux2 >= 12 && board[i][x] == board[y][z]){
        board[i][x] = 0;
        board[i][x+1]++;
        x++;            
        }

    }
}

// Verificar se existem casas >1
int validarJogada(int board[ROWS][COLS], int i, int j) {
    if (board[i][j] == 1) {
        for (int col = 0; col < COLS; col++) {
            if (board[i][col] > 1) {
                return 0;
            }
        }
        return 1;
    }
    return 1;
}

// Definir Jogada Jogador
void jogadareal(int board[ROWS][COLS], char player) {
    int i, j;
    if (player == jogador1) {
        i = 1;
    } else if (player == jogador2) {
        i = 0;
    }

    printf("Jogador %c, escolha uma coluna (1-6): ", player);
    scanf("%d", &j);

    if (j == 0) {
        guardarTabuleiro();  // guarda o tabuleiro e o programa encerra
    } else {
        if (j < 1 || j > 6 || board[i][j - 1] == 0 || !validarJogada(board, i, j - 1) || !verificarPedraSuficiente(board, i, j, board[i][j-1], player))
        do {
                printf("Escolha inválida! Tente novamente.\n");
                scanf("%d", &j);
        }while (j < 1 || j > 6 || board[i][j - 1] == 0 || !validarJogada(board, i, j - 1) || !verificarPedraSuficiente(board, i, j, board[i][j-1], player));
    }
        j--;
        int x = j;
        int aux = board[i][j];
        board[i][j] = 0;

        movimento(aux, i, x);
        capturarPontos(board, player, i, x);

    if (linhaAdversarioVazia(board, trocarJogador(player))) {
        printf("O adversário está sem pedras no seu campo. Realize uma jogada que insira pedras no campo adversário.\n");
        jogadareal(board, trocarJogador(player));
    }
}

// Definir Jogada Computador
void jogadacomputador(int coluna, char player) {
    // Seed para jogada
    srand(time(NULL));

    int j;
    do {
        // Escolhe uma coluna aleatória para o computador
        j = rand() % coluna;
    } while (board[0][j] == 0 || !validarJogada(board, 0, j) || !verificarPedraSuficiente(board, 0, j - 1, board[0][j], player));

    int i = 0;
    int aux = board[i][j];
    board[i][j] = 0;
    int x = j;

    movimento(aux, i, x);

    printf("O computador (%c) escolheu a coluna %d.\n", player, j + 1);

    capturarPontos(board, player, i, x);

    if (linhaAdversarioVazia(board, trocarJogador(player))) {
        printf("O jogador ficou sem pedras, o computador irá inserir pedras no campo adversário.\n");
        jogadacomputador(coluna, trocarJogador(player));
    }
}


// Verificar se há vencedor
char checkWinner() {
    if (depositoA >= 25) {
        return jogador1;
    } else if (depositoB >= 25) {
        return jogador2;
    }

    return '\0';
}

// Vencedor do jogo
void gameWinner(char vencedor) {
    if (vencedor == jogador1) {
        printf("Vencedor: Jogador 1! Capturou %d pedras!\n", depositoA);
    } else if (vencedor == jogador2) {
        printf("Vencedor: Jogador 2! Capturou %d pedras!\n", depositoB);
    } else if (vencedor == computador) {
        printf("Vencedor: Computador! Capturou %d pedras!\n", depositoB);
    }
}

// Main
int main(int argc, char *argv[]) {
    if (argc == 2) {
        carregarTabuleiro(argv[1]);
    } else {
        iniciarTabuleiro(board);
    }

    int modoJogo = escolherModoJogo();
    char currentPlayer = jogador1;

    while (1) {
        printBoard();

        if (modoJogo == 1 || (modoJogo == 2 && currentPlayer == jogador1)) {
            // Se for pvp ou a vez do player 1 em pvc
            jogadareal(board, currentPlayer);
        } else {
            // Caso contrário, é a vez do pc
            jogadacomputador(COLS, currentPlayer);
        }

        char vencedor = checkWinner();
        if (vencedor != '\0') {
            gameWinner(vencedor);
            break;
        }

        currentPlayer = trocarJogador(currentPlayer);
    }

    return 0;
}
