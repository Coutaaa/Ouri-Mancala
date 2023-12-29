#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#define ROWS 2
#define COLS 6

//inicializa o tabuleiro
int board[ROWS][COLS];
int depositoA = 0, depositoB = 0;

//definir jogadores
const char jogador1 = '1';
const char jogador2 = '2';
const char computador ='C';

//inicializar o tabuleiro
void iniciarTabuleiro(int board[ROWS][COLS]){
        for (int i = 0; i < ROWS; i++){
            for (int j = 0; j < COLS; j++){
                board[i][j]=4;
            }
        }
    }
//exibir o tabuleiro
void printBoard(){
    printf("\n|---|--|--|--|--|--|--|---|\n");
    printf("|   | %d| %d| %d| %d| %d| %d|   |", board[0][0],board[0][1],board[0][2],board[0][3],board[0][4],board[0][5]);
    printf("\n| %d |-----------------| %d |\n", depositoB, depositoA);
    printf("|   | %d| %d| %d| %d| %d| %d|   |", board[1][0],board[1][1],board[1][2],board[1][3],board[1][4],board[1][5]);
    printf("\n|---|--|--|--|--|--|--|---|\n");
    printf("\n");
}

//Definir jogador
char trocarJogador(char currentPlayer) {
    return (currentPlayer == jogador1) ? jogador2 : jogador1;
}

// função de captura de pontos
void capturarPontos(int board[ROWS][COLS], char player, int i, int x) {
    int linha = i;
    int coluna = x;

    int pedra = board[linha][x];
    
    if ((pedra == 2 || pedra == 3)) {
        if (player == jogador1 && i == 0) {
            depositoA += pedra;
            board[i][x] = 0;
            }
        } else if ( i = 1 && (player == jogador2 || player == computador)){
            depositoB += pedra;
            board[i][x] = 0;                
            }



   while (coluna > 0 && (board[linha][coluna - 1] == 2 || board[linha][coluna - 1] == 3)) {
        pedra = board[linha][coluna - 1];
        if (player == jogador1 && i == 0) {
            depositoA += pedra;
            board[linha][coluna - 1] = 0;
        } else if (player == jogador2 && i == 1) {
            depositoB += pedra;
            board[linha][coluna - 1] = 0;
        }
        coluna--;
    }

}


int validarJogada(int board[ROWS][COLS], int i, int j){
    if(board[i][j] == 1){
        for (int col = 0; col < COLS; col++) {
        if (board[i][col] > 1) {    
            return 0;
        }
    }
    return 1;    
}
    return 1;
}

int semPedraNoLado(int board[ROWS][COLS], char player){
    int linha = (player == jogador1) ? 0 : 1;

   for (int col = 0; col < COLS; col++) {
        if (board[linha][col] > 0) {
            return 0; 
        }
    }

    return 1; 
}

//Definir Jogada Jogador
void jogadareal(int board[ROWS][COLS], char player) {
    int i, j;
    if (player == jogador1){
        i=1;
    }
    else if(player == jogador2){
        i=0;
    }

    do {
        printf("Jogador %c, escolha uma coluna (1-6): ", player);
        scanf("%d", &j);

        if (j < 1 || j > 6 || board[i][j - 1] == 0 || !validarJogada(board, i, j-1)) {
            printf("Escolha inválida! Tente novamente.\n");
        }
    } while (j < 1 || j > 6 || board[i][j - 1] == 0 || !validarJogada(board, i, j-1));

    j--;
    int aux = board[i][j];
    board[i][j] = 0;
    int x = j;

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
    }
    capturarPontos(board, player, i, x);
}
 
 
//Definir Jogada Computador
void jogadacomputador(int coluna, char player) {
    //seed para jogada
    srand(time(NULL));

    int j;
    do {
        // Escolhe uma coluna aleatória para o computador
        j = rand() % coluna;
    } while (board[0][j] == 0 || !validarJogada(board, 0, j));  

    int i = 0;
    int aux = board[i][j];
    board[i][j] = 0;
    int x = j;

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
    }

    printf("O computador (%c) escolheu a coluna %d.\n", player, j+1);

    capturarPontos(board, player, i, x); 

}

//escolher PvP ou PvC;
int escolherModoJogo() {
    int escolha;

    do {
        printf("Escolha o modo de jogo:\n");
        printf("1. Jogador 1 vs. Jogador 2\n");
        printf("2. Jogador 1 vs. Computador\n");
        printf("Escolha: ");
        scanf("%d", &escolha);

        if(escolha < 1 || escolha > 2) {
            printf("Escolha inválida! Tente novamente.\n");
        }

    } while(escolha < 1 || escolha > 2);

    return escolha;
}



//verificar se há vencedor
char checkWinner() {
    if(depositoA >= 25) {
        return jogador1;
    } else if(depositoB >= 25) {
        return jogador2;
    }

    return '\0';
}



//vencedor do jogo
void gameWinner(char vencedor){
    if(vencedor  == jogador1){
        printf("Vencedor: Jogador 1! Capturou %d pedras!\n", depositoA);
    }
    else if(vencedor == jogador2){
        printf("Vencedor: Jogador 2! Capturou %d pedras!\n", depositoB);
    }
    else if(vencedor == computador){
        printf("Vencedor: Computador! Capturou %d pedras!\n", depositoB);
    }
}




//main
int main() {
    iniciarTabuleiro(board);
    int modoJogo = escolherModoJogo();
    char currentPlayer = jogador1;

    while (1) {
        printBoard();

        if (modoJogo == 1 || (modoJogo == 2 && currentPlayer == jogador1)) {
            // se for pvp ou a vez do player 1 em pvc
            jogadareal(board, currentPlayer);
        } else {
            // caso contrário, é a vez do pc
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
