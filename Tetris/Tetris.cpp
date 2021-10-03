#include <iostream>
#include <vector>
#include <windows.h>
#include <thread>
#include <vector>
#include <stdio.h>

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
bool bRotateKeyHold = false;
DWORD dwBytesWritten = 0;
int nTetrominoCount = 0;
vector<int> vLines;
char* cKeyMapping[4]{
    new char[3] { '2', '\x28', 'S' }, //                 Index 0  -  Down
    new char[3] { '2', '\x25', 'A' }, //                 Index 1  -  Left
    new char[3] { '2', '\x27', 'D' }, //                 Index 2  -  Right
    new char[6] { '5', ' ', 'Z', 'R', 'W', '\x26' } //   Index 3  -  Rotate
};

int nCurrentT = 0;
int nCurrentR = 0;
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
int calcTetrominoIndex(int x, int y, int r);
void drawField();
void flush();
void drawTetromino();
bool causesCollision(int x, int y, int r);
void checkForceDown();
void writeToField();
void checkForLines();
void summonNewTetromino();
bool isLine(int y);
void animateLineCompletions();
void moveLinesDown(int fromY);

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
    handlePlayerMovement();
    checkForceDown();
}

bool causesCollision(int x, int y, int r) {
    for (int tx = 0; tx < 4; tx++)
        for (int ty = 0; ty < 4; ty++)
        {
            // Map Boundry
            if (!(x + tx >= 0 && x + tx < nFieldWidth)) continue;
            if (!(y + ty >= 0 && y + ty < nFieldHeight)) continue;

            // Is in bounds -> Collision Check
            if (tetromino[nCurrentT][calcTetrominoIndex(tx, ty, r)] != L'.' // Is a part of the tetromino
                && pField[(y + ty) * nFieldWidth + (x + tx)] != 0)          // and something is blocking it
                return true;
        }

    return false;
}

void handlePlayerMovement() {
    nCurrentY += (bKey[0] && !causesCollision(nCurrentX, nCurrentY + 1, nCurrentR)) ? 1 : 0; // Down
    nCurrentX -= (bKey[1] && !causesCollision(nCurrentX - 1, nCurrentY, nCurrentR)) ? 1 : 0; // Left
    nCurrentX += (bKey[2] && !causesCollision(nCurrentX + 1, nCurrentY, nCurrentR)) ? 1 : 0; // Right
    nCurrentR += (bKey[3] && !causesCollision(nCurrentX, nCurrentY, nCurrentR + 1) && !bRotateKeyHold) ? 1 : 0; // Rotate
    bRotateKeyHold = bKey[3];
}

void checkForceDown() {
    if (!bForceDown) return;

    nSpeedCount = 0; // reset force down ticker
    nTetrominoCount++; 

    if (!causesCollision(nCurrentX, nCurrentY + 1, nCurrentR))
    {
        nCurrentY++;
        return;
    }

    writeToField();
    checkForLines();
    summonNewTetromino();
    if (causesCollision(nCurrentX, nCurrentY, nCurrentR))
        bGameOver = true;
}

void summonNewTetromino() {
    nCurrentX = nFieldWidth / 2;
    nCurrentT = rand() % 7;
    nCurrentY = 0;
    nCurrentR = 0;
}

void writeToField() {
    for (int x = 0; x < 4; x++)
        for (int y = 0; y < 4; y++)
            if (tetromino[nCurrentT][calcTetrominoIndex(x, y, nCurrentR)] != L'.')
                pField[(nCurrentY + y) * nFieldWidth + (nCurrentX + x)] = nCurrentT + 1;
}

void checkForLines() {
    for (int y = 0; y < 4; y++) {
        if (nCurrentY + y >= nFieldHeight - 1) continue; // exclude out of bounds pixels
        if (!isLine(y)) continue;

        for (int x = 1; x < nFieldWidth - 1; x++)
            pField[(nCurrentY + y) * nFieldWidth + x] = 8;

        vLines.push_back(nCurrentY + y);
    }
}

bool isLine(int y) {
    bool bLine = true;
    for (int x = 1; x < nFieldWidth - 1; x++)
        bLine &= pField[(nCurrentY + y) * nFieldWidth + x] != 0;

    return bLine;
}

int calcTetrominoIndex(int x, int y, int r) {
    switch (r % 4) {
    case 0:
        return y * 4 + x;
    case 1:
        return 12 + y - (x * 4);
    case 2: 
        return 15 - (y * 4) - x;
    case 3:
        return 3 - y + (x * 4);
    }
}

void render() {
    drawField();
    drawTetromino();
    animateLineCompletions();

    flush();
}

void animateLineCompletions() {
    if (vLines.empty()) return;

    flush();
    this_thread::sleep_for(400ms);

    for (int& v : vLines)
        moveLinesDown(v);

    vLines.clear();
}

void moveLinesDown(int fromY) {
    for (int x = 1; x < nFieldWidth - 1; x++) {
        for (int y = fromY; y > 0; y--)
            pField[y * nFieldWidth + x] = pField[(y - 1) * nFieldWidth + x];

        pField[x] = 0;
    }
}

void drawField() {
    for (int x = 0; x < nFieldWidth; x++)
        for (int y = 0; y < nFieldHeight; y++)
            screen[(y + nScreenOffsetY) * nScreenWidth + (x + nScreenOffsetX)] = L" ABCDEFG=#"[pField[y * nFieldWidth + x]];
}

void flush() {
    WriteConsoleOutputCharacter(hConsole, screen, nScreenWidth * nScreenHeight, { 0, 0 }, &dwBytesWritten);
}

void drawTetromino() {
    for (int x = 0; x < 4; x++)
        for (int y = 0; y < 4; y++)
            if (tetromino[nCurrentT][calcTetrominoIndex(x, y, nCurrentR)] != L'.')
                screen[(y + nScreenOffsetY + nCurrentY) * nScreenWidth + (x + nScreenOffsetX + nCurrentX)] = nCurrentT + 65;
}