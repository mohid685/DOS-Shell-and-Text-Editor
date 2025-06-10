#include "dos.h"
#include <iostream>
#include <conio.h>
#include <Windows.h>

#define BROWN 6
#define GREEN 2
#define RED 4
#define LBLUE 9
#define YELLOW 14
#define GRAY 8
#define PINK 13
#define ORANGE 12
#define GREEN 10
#define BLUE 9
#define PURPLE 5

void setConsoleColor2(int color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}
int main() {
    setConsoleColor2(GREEN);

    DOS dosShell;
    dosShell.run();

    return 0;
}