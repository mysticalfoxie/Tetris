#include <iostream>
#include <windows.h>

using namespace std;

int nScreenWidth = 80;
int nScreenHeight = 30;
int nFieldWidth = 12;
int nFieldHeight = 18;
wstring tetromino[7];
char* pField;
HANDLE hConsole = 0x0;
wchar_t* screen;
bool bGameOver = false;

wchar_t* initializeScreenBuffer();
void initializeConsole();
int main();
void initializeTetrominos();
void clear();
bool isBorder(int x, int y);
void startGame();

int main()
{
    initializeScreenBuffer();
    initializeConsole();
    initializeTetrominos();
    initializeField();

    startGame();

    clear();
}

void startGame() {

    while (!bGameOver) {

    }
}

void initializeField() {
    pField = new char[nFieldWidth * nFieldHeight];
    for (int x = 0; x < nFieldWidth; x++) // Board Boundary
        for (int y = 0; y < nFieldHeight; y++)
            pField[y * nFieldWidth + x] = isBorder(x, y) ? 9 : 0;
}

bool isBorder(int x, int y) {
    if (x == 0) return true;
    if (x == nFieldWidth - 1) return true;
    if (y == nFieldHeight - 1) return true;

    return false;
}

void initializeTetrominos() {
    tetromino[0].append(L"..X...X...X...X.");
    tetromino[1].append(L"..X..XX...X.....");
    tetromino[2].append(L".....XX..XX.....");
    tetromino[3].append(L"..X..XX..X......");
    tetromino[4].append(L".X...XX...X.....");
    tetromino[5].append(L".X...X...XX.....");
    tetromino[6].append(L"..X...X..XX.....");
}

wchar_t* initializeScreenBuffer() {
    screen = new wchar_t[nScreenWidth * nScreenHeight];
    for (int i = 0; i < nScreenWidth * nScreenHeight; i++)
        screen[i] = L' ';
}

void initializeConsole() {
    int access = GENERIC_READ | GENERIC_WRITE;
    int mode = CONSOLE_TEXTMODE_BUFFER;
    hConsole = CreateConsoleScreenBuffer(access, 0, NULL, mode, NULL);
    SetConsoleActiveScreenBuffer(hConsole);
}

void clear() {
    CloseHandle(hConsole);
    system("pause");
}