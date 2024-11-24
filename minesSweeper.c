#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>

// defining no of cells and no of mines for mine sweeper
#define gridSize 9
#define minesNumber 8

// this function is used to add colors in our terminal
// takes a integer in input denoting the code for the color
void setConsoleColor(int color)
{
    // this hconsole is the pointer to our current output terminal
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    // this sets  color to text
    // this function is provides in the window.h header file
    SetConsoleTextAttribute(hConsole, color);
}

// to place mines in the grid
void placeMines(int grid[gridSize][gridSize], int size, int mine)
{
    // this function will generate random seed for the srand function to generate random number
    srand(time(0));

    // running loop till all the mines are placed
    while (mine > 0)
    {
        // generating random coordinates
        int xCoord = rand() % size;
        int yCoord = rand() % size;

        // ensuring that no two mines gets placed on one block
        if (grid[xCoord][yCoord] == 0)
        {
            grid[xCoord][yCoord] = -1;
            mine--;
        }
    }
}

// to mark each cell of the grid
// we will give each cell a number represeting the no of mines in its neighbourhood
void markGrid(int grid[gridSize][gridSize], int size)
{
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            if (grid[i][j] == -1)
                continue;
            int mines = 0;

            // checking for each cell around the current cell
            if (j + 1 < size && grid[i][j + 1] == -1)
                mines++;
            if (j - 1 >= 0 && grid[i][j - 1] == -1)
                mines++;
            if (i + 1 < size && grid[i + 1][j] == -1)
                mines++;
            if (i - 1 >= 0 && grid[i - 1][j] == -1)
                mines++;
            if (i - 1 >= 0 && j + 1 < size && grid[i - 1][j + 1] == -1)
                mines++;
            if (i - 1 >= 0 && j - 1 >= 0 && grid[i - 1][j - 1] == -1)
                mines++;
            if (i + 1 < size && j - 1 >= 0 && grid[i + 1][j - 1] == -1)
                mines++;
            if (i + 1 < size && j + 1 < size && grid[i + 1][j + 1] == -1)
                mines++;

            grid[i][j] = mines;
        }
    }
}

// for checking which cells are to be shown if the cell chosen by the user is a safe
// it reveals all the safe zones around the cell chosen and shows the border containing only numbers
// this is a recursive function
void checkGrid(int grid[gridSize][gridSize], int revealGrid[gridSize][gridSize], int size, int xCoord, int yCoord)
{
    // this function only executes if the cell number in input is valid
    if (xCoord >= 0 && xCoord < size && yCoord >= 0 && yCoord < size)
    {
        // stops if it founds a number greater than 0 and its not reveal till now
        if (grid[xCoord][yCoord] > 0 && revealGrid[xCoord][yCoord] == 0)
        {
            revealGrid[xCoord][yCoord] = 1;
            return;
        }

        // go to each cell with 0 and repeat the process for the adjacent cells
        if (grid[xCoord][yCoord] == 0 && revealGrid[xCoord][yCoord] == 0)
        {
            revealGrid[xCoord][yCoord] = 1;
            checkGrid(grid, revealGrid, size, xCoord, yCoord + 1);
            checkGrid(grid, revealGrid, size, xCoord, yCoord - 1);
            checkGrid(grid, revealGrid, size, xCoord + 1, yCoord);
            checkGrid(grid, revealGrid, size, xCoord - 1, yCoord);

            // for diagnols
            checkGrid(grid, revealGrid, size, xCoord + 1, yCoord + 1);
            checkGrid(grid, revealGrid, size, xCoord + 1, yCoord - 1);
            checkGrid(grid, revealGrid, size, xCoord - 1, yCoord + 1);
            checkGrid(grid, revealGrid, size, xCoord - 1, yCoord - 1);
        }
    }
}

// checking whether the user has won the game or not
int checkWin(int grid[gridSize][gridSize], int revealGrid[gridSize][gridSize], int size)
{
    // it counts number of mines and no of safezones that are revealed
    // if sum of both equals to total no of cells then the user has won the game
    int countMines = 0;
    int countSafeZonesRevealed = 0;
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            if (grid[i][j] == -1)
                countMines++;
            else if (revealGrid[i][j] == 1)
                countSafeZonesRevealed++;
        }
    }

    // returns 1 if won and 0 if not won till now
    if (countSafeZonesRevealed + countMines == size * size)
        return 1;
    else
        return 0;
}

// for printing grid
void printGrid(int grid[gridSize][gridSize], int revealGrid[gridSize][gridSize], int size)
{
    setConsoleColor(15);
    printf("       1  2  3  4  5  6  7  8  9  \n");
    printf("       v  v  v  v  v  v  v  v  v   \n");
    printf("     ----------------------------- \n");
    for (int i = 0; i < size; i++)
    {
        setConsoleColor(15);
        printf("%d > |  ", i + 1);
        for (int j = 0; j < size; j++)
        {
            // printing each cell based on whether its revealed according to reveal grid or not
            if (revealGrid[i][j] == 1)
            {
                if (grid[i][j] == -1)
                {
                    setConsoleColor(12); // Red for mines
                    printf("*  ");
                }
                else if (grid[i][j] == 0)
                {
                    setConsoleColor(10); // Green for safe cells
                    printf("o  ");
                }
                else
                {
                    setConsoleColor(14); // Yellow for numbers
                    printf("%d  ", grid[i][j]);
                }
            }
            else
            {
                setConsoleColor(8); // Gray for unrevealed cells
                printf("-  ");
            }
        }
        setConsoleColor(15);
        printf("|\n");
    }
    printf("     ----------------------------- \n");
}

void mineSweeper()
{
    // displaying the welcome screen
    setConsoleColor(11);
    printf("___  ____                   _____                                    \n");
    printf("|  \\/  (_)                 /  ___|                                   \n");
    printf("| .  . |_ _ __   ___ ______\\ `--.__      _____  ___ _ __   ___ _ __  \n");
    printf("| |\\/| | | '_ \\ / _ \\______|`--. \\ \\ /\\ / / _ \\/ _ \\ '_ \\ / _ \\ '__| \n");
    printf("| |  | | | | | |  __/      /\\__/ /\\ V  V /  __/  __/ |_) |  __/ |    \n");
    printf("\\_|  |_/_|_| |_|\\___|      \\____/  \\_/\\_/ \\___|\\___| .__/ \\___|_|    \n");
    printf("                                                   | |               \n");
    printf("                                                   |_|               \n");

    printf("\n");

    // displaying the instructions
    printf("HOW TO PLAY:\n");

    printf("-----------------------------------------------------------------\n");

    printf("1. The goal is to uncover all safe cells without hitting a mine.\n");
    printf("2. Numbers indicate how many mines are adjacent to a cell.\n");
    printf("3. Be strategic! Avoid cells with mines (*), or it's game over.\n");
    printf("4. Enter your move as: ROW COLUMN (e.g., 3 4).\n");
    printf("5. Clear all safe cells to win the game. Best of luck!\n");

    printf("-----------------------------------------------------------------\n");
    setConsoleColor(15);

    setConsoleColor(5);
    printf("\nLet's get started! Here's your grid:\n\n");
    setConsoleColor(15);

    // defining two matrices
    // grid        - for storing the mines and cells
    // reveal grid - for storing which cell is revealed and which is not
    int grid[gridSize][gridSize] = {0}, revealGrid[gridSize][gridSize] = {0};

    placeMines(grid, gridSize, minesNumber);

    markGrid(grid, gridSize);

    printGrid(grid, revealGrid, gridSize);

    // main gameloop
    while (1)
    {
        // inputing user choice
        int x, y;
        setConsoleColor(10);
        printf("\nEnter your move (ROW COLUMN): ");
        setConsoleColor(15);

        setConsoleColor(14);
        scanf("%d %d", &x, &y);
        printf("\n");
        setConsoleColor(15);
        x -= 1;
        y -= 1;

        // checking the user input
        if (!(x >= 0 && x < gridSize && y >= 0 && y < gridSize))
        {
            setConsoleColor(12);
            printf("\nInvalid move! Stay within the grid.\n");
            setConsoleColor(15);
            continue;
        }

        // if the cell input by user has bomb then game is over
        if (grid[x][y] == -1)
        {
            setConsoleColor(12);
            printf("\nBOOM! You hit a mine. Game Over.\n");
            printf("Revealing the whole grid!!");
            printf("\n");
            printf("\n");
            setConsoleColor(15);
            for (int i = 0; i < gridSize; i++)
                for (int j = 0; j < gridSize; j++)
                    revealGrid[i][j] = 1;
            printGrid(grid, revealGrid, gridSize);
            break;
        }

        // or reveal the grid automatically
        checkGrid(grid, revealGrid, gridSize, x, y);
        printGrid(grid, revealGrid, gridSize);

        // checking whether the user has won or not
        if (checkWin(grid, revealGrid, gridSize))
        {
            setConsoleColor(10);
            printf("\nCongratulations! You cleared the board!\n");
            setConsoleColor(15);
            break;
        }
    }
    setConsoleColor(11);
    printf("--------------------------------------------------------------");
    printf("\nThanks for playing!\n");
    setConsoleColor(15);
}

int main()
{
    mineSweeper();
    return 0;
}
