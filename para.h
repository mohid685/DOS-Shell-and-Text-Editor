#pragma once
#include <list>
#include <iostream>
#include <conio.h>
#include <Windows.h>
#include "line.h"

void gotoRowCol(int rpos, int cpos)
{
    COORD scrn;
    HANDLE hOuput = GetStdHandle(STD_OUTPUT_HANDLE);
    scrn.X = cpos;
    scrn.Y = rpos;
    SetConsoleCursorPosition(hOuput, scrn);
}

class Document {
public:
    list<Line*> Ls;
    int cursorRow = 0;
    int cursorCol = 0;
    
    void displayDocument() const {
        int row = 0;
        for (const auto& line : Ls) {
            if (row == cursorRow) {
                cout << "-> "; 
            }
            else {
                cout << "   ";
            }
            if (row == cursorRow) {
                line->displayLine(cursorCol); 
            }
            else {
                line->displayLine(-1);
            }
            row++;
        }
        gotoRowCol(0, 210);
        cout << "| Cols: " << cursorCol << "| Rows: " << cursorRow << "|";
    }
    void addLine(Line* line) {
        Ls.push_back(line);
    }
};