#include <iostream>
#include <vector>
#include <windows.h>
#include <thread>

using namespace std;

int nScreenWidth = 80;
int nScreenHeight = 30;
int nScreenOffsetX = 2;
int nScreenOffsetY = 1;
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
DWORD dwBytesWritten = 0;
char* cKeyMapping[4]{
    new char[3] { '2', '\x25', 'S' }, //                 Index 0  -  Down
    new char[3] { '2', '\x27', 'A' }, //                 Index 1  -  Left
    new char[3] { '2', '\x28', 'D' }, //                 Index 2  -  Right
    new char[6] { '5', ' ', 'Z', 'R', 'W', '\x26' } //   Index 3  -  Rotate
};

int nCurrentTetromino = 0;
int nCurrentRotation = 0;
int nCurrentX = 0;
int nCurrentY = 0;

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
void drawField();
void flush();
void drawTetromino();

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
    if (x == nFieldWidth) return true;
    if (y == nFieldHeight) return true;

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
    for (int ki = 0; ki < 4; ki++) {
        bool bPressed = false;
        int nKeyCount = cKeyMapping[ki][0] - '0';
        for (int ci = 0; ci < nKeyCount; ci++)
            if (GetAsyncKeyState(cKeyMapping[ki][ci + 1]) != NULL)
                bPressed = true;

        bKey[ki] = bPressed;
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
    drawField();
    drawTetromino();

    flush();
}

void drawField() {
    for (int x = 0; x < nFieldWidth; x++)
        for (int y = 0; y < nFieldHeight; y++)
            screen[(y + nScreenOffsetY) * nScreenWidth + (x + nScreenOffsetX)] = L" ABCDEFG=#"[pField[y * nFieldWidth + x]];
}

void flush() {
    WriteConsoleOutputCharacter(hConsole, screen, nScreenWidth * nScreenHeight, { 0,0 }, &dwBytesWritten);
}

void drawTetromino() {
    for (int x = 0; x < 4; x++)
        for (int y = 0; y < 4; y++)
            if (tetromino[nCurrentTetromino][y * 4 + x] != L'.')
                screen[(y + nScreenOffsetY + nCurrentY) * nScreenWidth + (x + nScreenOffsetX + nCurrentX)] = nCurrentTetromino + 65;
}