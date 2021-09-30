#include <iostream>
#include <vector>
#include <windows.h>
#include <thread>

using namespace std;

int nScreenWidth = 80;
int nScreenHeight = 30;
int nFieldWidth = 12;
int nFieldHeight = 18;
int nSpeedCount = 0;
int nSpeed = 20;
wstring tetromino[7];
bool bKey[4];
char* pField;
HANDLE hConsole = 0x0;
wchar_t* screen;
bool bGameOver = false;
bool bForceDown = false;

wchar_t* initializeScreenBuffer();
void initializeField();
void initializeConsole();
void initializeTetrominos();
void clear();
bool isBorder(int x, int y);
void startGame();
void doGameTiming();
void doInputReading();
void doGameLogic();
void render();
void handlePlayerMovement();
void rotate();
void forceDown();

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
        doGameTiming();
        doInputReading();
        doGameLogic();
        render();
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

    return screen;
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

void doGameTiming() {
    this_thread::sleep_for(50ms);
    nSpeedCount++;
    bForceDown = nSpeedCount == nSpeed;
}

void doInputReading() {
    for (int i = 0; i < 4; i++) {
        char cLeft = 'SADR';   // S
        char cRight = 'A';  // A
        char cDown = 'D';   // D
        char cRotate = 'R'; // R
        //bKey[i] = +(0x8000 & GetAsyncKeyState())
    }
}

void doGameLogic() {

}

void handlePlayerMovement() {

}

void rotate() {

}

void forceDown() {

}

void render() {

}