#pragma once
#include <iostream>
#include <stack>
#include <queue>
#include <conio.h>
#include <Windows.h>
#include <iomanip>
#include <list>
#include <string>
#include "para.h"
#include "Header.h"
#include "dos.h"

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

using namespace std;


template <typename Iterator>
Iterator myNext(Iterator it, int n = 1) {
    while (n-- > 0) {
        ++it;
    }
    return it;
}

template <typename Iterator>
Iterator myPrev(Iterator it, int n = 1) {
    while (n-- > 0) {
        --it;
    }
    return it;
}

void setConsoleColor(int color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}


class TextEditor {
private:
    Document doc;
    string currFilePath;
    bool isReverseTraversal = false;

    stack<string> undoStk;        
    stack<string> redoStk;        
    stack<int> undoCursorRow;     
    stack<int> undoCursorCol;     
    stack<int> redoCursorRow;     
    stack<int> redoCursorCol;     

    void saveState() {
        undoStk.push(traverseDoc());
        undoCursorRow.push(doc.cursorRow);
        undoCursorCol.push(doc.cursorCol);

        while (!redoStk.empty()) {
            redoStk.pop();
        }
        while (!redoCursorRow.empty()) {
            redoCursorRow.pop();
        }
        while (!redoCursorCol.empty()) {
            redoCursorCol.pop();
        }
    }


public:
    void displayLoadingBar2() {

        system("cls");
        SetConsoleCP(437);
        SetConsoleOutputCP(437);
        int bar1 = 177;
        int bar2 = 219;

        cout << "\n\n\n\t\t\t\Encoding In Progress...";
        cout << "\n\n\n\t\t\t\t";

        for (int i = 0; i < 25; i++) {
            cout << (char)bar1;
        }

        cout << "\r";
        cout << "\t\t\t\t";

        for (int i = 0; i < 25; i++) {
            cout << (char)bar2;
            Sleep(150);
        }

        cout << "\n\t\t\t\t" << (char)1 << "!";
    }
    void displayLoadingBar3() {

        system("cls");
        SetConsoleCP(437);
        SetConsoleOutputCP(437);
        int bar1 = 177;
        int bar2 = 219;

        cout << "\n\n\n\t\t\t\Decoding In Progress...";
        cout << "\n\n\n\t\t\t\t";

        for (int i = 0; i < 25; i++) {
            cout << (char)bar1;
        }

        cout << "\r";
        cout << "\t\t\t\t";

        for (int i = 0; i < 25; i++) {
            cout << (char)bar2;
            Sleep(150);
        }

        cout << "\n\t\t\t\t" << (char)1 << "!";
    }

    string traverseDoc() {
        string curr;
        for (const auto& line : doc.Ls) {
            for (const auto& letter : line->Cs) {
                curr += letter.ch;
            }
            curr += '\n';
        }
        return curr;
    }
    void deserialiseDoc(const string& serialized, int cursorRow, int cursorCol) {
        doc.Ls.clear();
        Line* newLine = new Line();
        for (char ch : serialized) {
            if (ch == '\n') {
                doc.Ls.push_back(newLine);
                newLine = new Line();
            }
            else {
                letters newLetter = { ch };
                newLine->Cs.push_back(newLetter);
            }
        }
        if (!newLine->Cs.empty()) {
            doc.Ls.push_back(newLine);
        }
        doc.cursorRow = cursorRow;
        doc.cursorCol = cursorCol;
    }
    void undo() {
        if (undoStk.empty()) {
            system("cls");
            cout << "No operations to undo.\n";
            _getch();
            return;
        }

        redoStk.push(traverseDoc());
        redoCursorRow.push(doc.cursorRow);
        redoCursorCol.push(doc.cursorCol);

        string lastState = undoStk.top();
        undoStk.pop();
        int lastCursorRow = undoCursorRow.top();
        undoCursorRow.pop();
        int lastCursorCol = undoCursorCol.top();
        undoCursorCol.pop();

        deserialiseDoc(lastState, lastCursorRow, lastCursorCol);
    }
    void redo() {
        if (redoStk.empty()) {
            system("cls");
            cout << "No operations to redo.\n";
            _getch();
            return;
        }

        undoStk.push(traverseDoc());
        undoCursorRow.push(doc.cursorRow);
        undoCursorCol.push(doc.cursorCol);

        string nextState = redoStk.top();
        redoStk.pop();
        int nextCursorRow = redoCursorRow.top();
        redoCursorRow.pop();
        int nextCursorCol = redoCursorCol.top();
        redoCursorCol.pop();

        deserialiseDoc(nextState, nextCursorRow, nextCursorCol);
    }
    string reverseString(const string& str) {
        return string(str.rbegin(), str.rend());
    }
    void highlightMatch(Line* line, int startPos, int matchLen) {

        auto it = line->Cs.begin();

        for (int i = 0; i < startPos; i++) {
            if (it == line->Cs.end()) {
                return;
            }
            it++;
        }

        for (int i = 0; i < matchLen; i++) {

            if (it == line->Cs.end()) {
                return;
            }

            setConsoleColor(14);
            cout << it->ch;
            setConsoleColor(7);
            it++;
        }
        cout << "\n";
    }
    void loadFile(const string& filename) {
        currFilePath = filename;
        ifstream file(filename);
        string lineContent;

        while (getline(file, lineContent)) {
            Line* line = new Line();

            for (char c : lineContent) {
                letters l = { c };
                line->Cs.push_back(l);
            }

            doc.addLine(line);
        }

        file.close();
    }
    void saveFile(const string& fileName) {

        ofstream wrt(fileName);

        if (!wrt) {
            cout << "Error: Cannot save the file:/ \n";
            return;
        }

        for (const auto& line : doc.Ls) {
            for (const auto& sentence : line->Cs) {
                wrt << sentence.ch;
            }
            wrt << "\n";
        }
       

        wrt.close();
        cout << "File has been successfully saved!!\n";
    }
    void toUpper() {

        auto lineIT = doc.Ls.begin();

        for (int i = 0; i < doc.cursorRow; i++) {
            lineIT++;
        }

        Line* currLine = *lineIT;

        if (currLine->Cs.empty()) {
            return;                 //case when no word is present than just simply return
        }

        int col = doc.cursorCol;

        auto cursorStart = currLine->Cs.begin();
        auto cursorEnd = currLine->Cs.begin();

        auto tempIT = currLine->Cs.begin();
        
        for (int i = 0; i < col; i++) {
            tempIT++;
        }
        
        while (tempIT != currLine->Cs.begin() && myPrev(tempIT)->ch != ' ') {
            tempIT--;
        }

        cursorStart = tempIT;

        tempIT = currLine->Cs.begin();

        for (int i = 0; i < col; i++) {
            tempIT++;
        }

        while (tempIT != currLine->Cs.end() && tempIT->ch != ' ') {
            tempIT++;
        }

        cursorEnd = tempIT;

        for (auto it = cursorStart; it != cursorEnd; it++) {
            if (it->ch >= 'a' && it->ch <= 'z') {
                it->ch -= ('a' - 'A');
            }
        }
    }
    void toUpperExtend() {
        auto lineIt = myNext(doc.Ls.begin(), doc.cursorRow); 
        Line* currentLine = *lineIt;

        if (currentLine->Cs.empty()) {
            return; 
        }

        auto temp = myNext(currentLine->Cs.begin(), doc.cursorCol);

        while (temp != currentLine->Cs.end() && temp->ch != '.' && temp->ch != '?' && temp->ch != '!') {

            if (temp->ch >= 'a' && temp->ch <= 'z') {
                temp->ch -= ('a' - 'A');
            }
            temp++;
        }
    }
    void toLower() {

        auto lineIT = doc.Ls.begin();

        for (int i = 0; i < doc.cursorRow; i++) {
            lineIT++;
        }

        Line* currLine = *lineIT;

        if (currLine->Cs.empty()) {
            return;                 //case when no word is present than just simply return
        }

        int col = doc.cursorCol;

        auto cursorStart = currLine->Cs.begin();
        auto cursorEnd = currLine->Cs.begin();

        auto tempIT = currLine->Cs.begin();

        for (int i = 0; i < col; i++) {
            tempIT++;
        }

        while (tempIT != currLine->Cs.begin() && myPrev(tempIT)->ch != ' ') {
            tempIT--;
        }

        cursorStart = tempIT;

        tempIT = currLine->Cs.begin();

        for (int i = 0; i < col; i++) {
            tempIT++;
        }

        while (tempIT != currLine->Cs.end() && tempIT->ch != ' ') {
            tempIT++;
        }

        cursorEnd = tempIT;

        for (auto it = cursorStart; it != cursorEnd; it++) {
            if (it->ch >= 'A' && it->ch <= 'Z') {
                it->ch += ('a' - 'A');
            }
        }
    }
    void toLowerExtend() {
        auto lineIt = myNext(doc.Ls.begin(), doc.cursorRow);
        Line* currentLine = *lineIt;

        if (currentLine->Cs.empty()) {
            return;
        }

        auto temp = myNext(currentLine->Cs.begin(), doc.cursorCol);

        while (temp != currentLine->Cs.end() && temp->ch != '.' && temp->ch != '?' && temp->ch != '!') {

            if (temp->ch >= 'A' && temp->ch <= 'Z') {
                temp->ch += ('a' - 'A');
            }
            temp++;
        }
    }
    void findWord(const string& word) {
        bool isWordFound = false;
        int lineNo = 1;

        for (auto lineIT = doc.Ls.begin(); lineIT != doc.Ls.end(); lineIT++, lineNo++) {
            Line* currLine = *lineIT;
            string currLineContent;

            for (const auto& letter : currLine->Cs) {
                currLineContent += letter.ch;
            }

            int pos = 0;
            while (pos < currLineContent.length()) {
                bool match = true;

                for (int i = 0; i < word.length(); i++) {
                    if (pos + i >= currLineContent.length()) {
                        match = false;
                        break;
                    }

                    char lineChar = currLineContent[pos + i];
                    char wordChar = word[i];

                    
                        if (lineChar >= 'A' && lineChar <= 'Z') {
                            lineChar += ('a' - 'A'); 
                        }
                        if (wordChar >= 'A' && wordChar <= 'Z') {
                            wordChar += ('a' - 'A'); 
                        }
                    

                    if (lineChar != wordChar) {
                        match = false;
                        break;
                    }
                }

                if (match) {
                    isWordFound = true;

                    cout << "Line " << lineNo << ": ";
                    cout << currLineContent.substr(0, pos);

                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_INTENSITY);
                    cout << currLineContent.substr(pos, word.length());
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN | FOREGROUND_INTENSITY);

                    cout << currLineContent.substr(pos + word.length()) << "\n";

                    pos += word.length();
                }
                else {
                    pos++;
                }
            }
        }

        if (!isWordFound) {
            cout << "Word '" << word << "' not found in the document.\n";
        }
    }
    void findSentence(const string& sentence) {
        bool isSentenceFound = false;
        int lineNo = 1;

        string processedSentence = sentence;
       
            for (char& ch : processedSentence) {
                if (ch >= 'A' && ch <= 'Z') {
                    ch += ('a' - 'A'); 
                }
            }

        for (auto lineIT = doc.Ls.begin(); lineIT != doc.Ls.end(); lineIT++, lineNo++) {
            Line* currLine = *lineIT;
            string currLineContent;

            for (const auto& letter : currLine->Cs) {
                currLineContent += letter.ch;
            }

            string processedLine = currLineContent;

            for (char& ch : processedLine) {
                if (ch >= 'A' && ch <= 'Z') {
                    ch += ('a' - 'A');
                }
            }

            int pos = 0;
            while (pos < processedLine.length()) {
                bool match = true;

                for (int i = 0; i < processedSentence.length(); i++) {
                    if (pos + i >= processedLine.length()) {
                        match = false;
                        break;
                    }

                    if (processedLine[pos + i] != processedSentence[i]) {
                        match = false;
                        break;
                    }
                }

                if (match) {
                    isSentenceFound = true;

                    cout << "Line " << lineNo << ": ";
                    cout << currLineContent.substr(0, pos);

                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_INTENSITY);
                    cout << currLineContent.substr(pos, processedSentence.length());
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN | FOREGROUND_INTENSITY);
                    cout << currLineContent.substr(pos + processedSentence.length()) << "\n";


                    pos += processedSentence.length();
                }
                else {
                    pos++;
                }
            }
        }

        if (!isSentenceFound) {
            cout << "Sentence '" << sentence << "' not found in the document.\n";
        }
    }
    void findSubstring(const string& substring) {
        int lineNo = 1;
        int substringLen = substring.length();
        bool foundInLine = false;
        bool matched = false;

        for (const auto& line : doc.Ls) {
            int charPos = 0;
            auto start = line->Cs.begin();

            while (start != line->Cs.end()) {
                auto temp = start;
                int matchedLen = 0;

                while (temp != line->Cs.end() && matchedLen < substringLen && tolower(temp->ch) == substring[matchedLen]) {
                    temp++;
                    matchedLen++;
                }

                if (matchedLen == substringLen) {
                    if (!foundInLine) {
                        cout << "Line " << lineNo << ": ";
                        foundInLine = true;
                        matched = true;
                    }

                    for (auto it = line->Cs.begin(); it != start; it++) {
                        cout << it->ch;
                    }

                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_INTENSITY);
                    for (int i = 0; i < substringLen; i++) {
                        cout << start->ch;
                        start++;
                    }
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN | FOREGROUND_INTENSITY);
                }
                else {
                    if (foundInLine) {
                        cout << start->ch;
                    }
                    start++;
                }
            }

            if (foundInLine) {
                cout << "\n";
                foundInLine = false;
            }
            lineNo++;
        }

        if (!matched) {
            cout << "No substrings found!!\n";
            return;
        }
    }
    bool isValid(char ch) {
        return (ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z') || (ch >= '0' && ch <= '9');
    }
    void findAndReplace(const string& oldWord, const string& newWord, bool replaceAll) {
        bool found = false;

        for (auto& line : doc.Ls) {
            auto it = line->Cs.begin();
            while (it != line->Cs.end()) {
                auto start = it;
                auto temp = start;
                int matchLength = 0;

                while (temp != line->Cs.end() && matchLength < oldWord.size() && tolower(temp->ch) == tolower(oldWord[matchLength])) {
                    temp++;
                    matchLength++;
                }

                bool isWordBoundaryStart = (start == line->Cs.begin() || !isValid(myPrev(start)->ch));
                bool isWordBoundaryEnd = (temp == line->Cs.end() || !isValid(temp->ch));

                if (matchLength == oldWord.size() && isWordBoundaryStart && isWordBoundaryEnd) {
                    found = true;

                    for (int i = 0; i < matchLength; i++) {
                        start = line->Cs.erase(start);
                    }

                    for (char c : newWord) {
                        letters newLetter;
                        newLetter.ch = c;
                        line->Cs.insert(start, newLetter);
                    }

                    if (!replaceAll) {
                        return;
                    }

                    it = start;
                }
                else {
                    it++;
                }
            }
        }

        if (found) {
            cout << "Replacement completed.\n";
        }
        else {
            cout << "The word '" << oldWord << " was not found in the document.\n";
        }
    }
    int countSpecialChars() {

        int count = 0;

        for (const auto& line : doc.Ls) {
            for (const auto& letter : line->Cs) {

                char c = letter.ch;

                if (!isValid(c)) {
                    count++;
                }
            }
        }
        return count;
    }
    int countParas() {

        int paraCount = 0;
        bool inPara = false;

        for (const auto& line : doc.Ls) {

            bool isEmpty = line->Cs.empty();

            if (!isEmpty && !inPara) {
                paraCount++;
                inPara = true;
            }
            else if (isEmpty) {
                inPara = false;
            }
        }
        return paraCount;
    }
    int countSentences() {

        int count = 0;

        for (const auto& line : doc.Ls) {
            for (const auto& letter : line->Cs) {

                char c = letter.ch;

                if (c=='.' || c=='?' || c=='!') {
                    count++;
                }
            }
        }
        return count;
    }
    void addPrefix(const string& word, const string& prefix) {

        for (auto& line : doc.Ls) {

            auto it = line->Cs.begin();

            while (it != line->Cs.end()) {

                auto start = it;
                auto temp = it;

                int matchedLen = 0;

                while (temp != line->Cs.end() && matchedLen < word.size() && tolower(temp->ch) == tolower(word[matchedLen])) {

                    temp++;
                    matchedLen++;
                }

                bool isWordBoundaryStart = (start == line->Cs.begin() || !isValid(myPrev(start)->ch));
                bool isWordBoundaryEnd = (temp == line->Cs.end() || !isValid(temp->ch));

                if (matchedLen == word.size() && isWordBoundaryStart && isWordBoundaryEnd) {

                    for (char c : prefix) {
                        letters newletter;
                        newletter.ch = c;
                        line->Cs.insert(start, newletter);
                    }
                    it = temp;
                }
                else {
                    it++;
                }
            }
        }
        cout << "Prefix '" << prefix << "' added to all occurrences of" << word << "\n";
    }
    void addPostfix(const string& word, const string& postfix) {

        for (auto& line : doc.Ls) {

            auto it = line->Cs.begin();

            while (it != line->Cs.end()) {

                auto start = it;
                auto temp = it;

                int matchedLen = 0;

                while (temp != line->Cs.end() && matchedLen < word.size() && tolower(temp->ch) == tolower(word[matchedLen])) {

                    temp++;
                    matchedLen++;
                }

                bool isWordBoundaryStart = (start == line->Cs.begin() || !isValid(myPrev(start)->ch));
                bool isWordBoundaryEnd = (temp == line->Cs.end() || !isValid(temp->ch));

                if (matchedLen == word.size() && isWordBoundaryStart && isWordBoundaryEnd) {

                    for (char c : postfix) {
                        letters newletter;
                        newletter.ch = c;
                        line->Cs.insert(temp, newletter);
                    }
                    it = temp;
                }
                else {
                    it++;
                }
            }
        }
        cout << "Postfix '" << postfix << "' added to all occurrences of" << word << "\n";
    }
    int longestWord() {
        string longestWord;
        string currentWord;

        for (auto& line : doc.Ls) {
            for (auto& character : line->Cs) {
                char tempChar = tolower(character.ch); 
                if (isValid(tempChar)) {
                    currentWord += tempChar;
                }
                else {
                    if (!currentWord.empty() && currentWord.length() > longestWord.length()) {
                        longestWord = currentWord;
                    }
                    currentWord.clear();
                }
            }

            if (!currentWord.empty() && currentWord.length() > longestWord.length()) {
                longestWord = currentWord;
            }
        }

        cout << "Longest Word: " << longestWord << " (Length: " << longestWord.length() << ")\n";

        if (longestWord.empty()) {
            return 0;
        }
        else {
            return longestWord.length();
        }
    }
    int smallestWordLength() {
        int smallestLength = INT_MAX;
        int secondSmallestLength = INT_MAX;
        string smallestWord;
        string secondSmallestWord;
        string currentWord;

        for (auto& line : doc.Ls) {
            for (auto& character : line->Cs) {
                char tempChar = tolower(character.ch);

                if (isalnum(tempChar)) {  
                    currentWord += tempChar;
                }
                else {
                    if (!currentWord.empty()) {
                        if (currentWord.length() < smallestLength) {
                            secondSmallestLength = smallestLength;
                            secondSmallestWord = smallestWord;
                            smallestLength = currentWord.length();
                            smallestWord = currentWord;
                        }
                        else if (currentWord.length() < secondSmallestLength && currentWord.length() > smallestLength) {
                            secondSmallestLength = currentWord.length();
                            secondSmallestWord = currentWord;
                        }
                        currentWord.clear();
                    }
                }
            }

            if (!currentWord.empty()) {
                if (currentWord.length() < smallestLength) {
                    secondSmallestLength = smallestLength;
                    secondSmallestWord = smallestWord;
                    smallestLength = currentWord.length();
                    smallestWord = currentWord;
                }
                else if (currentWord.length() < secondSmallestLength && currentWord.length() > smallestLength) {
                    secondSmallestLength = currentWord.length();
                    secondSmallestWord = currentWord;
                }
                currentWord.clear();
            }
        }

        if (smallestLength == INT_MAX) {
            cout << "No valid words found.\n";
            return 0;
        }

        if (smallestLength == 1) {
            cout << "Second Smallest Word: " << secondSmallestWord << " (Length: " << secondSmallestLength << ")\n";
            return secondSmallestLength;
        }
        else {
            cout << "Smallest Word: " << smallestWord << " (Length: " << smallestLength << ")\n";
            return smallestLength;
        }
    }
    int largestParaLength() {

        int maxWords = 0, currentWords = 0;

        bool inPara = false;

        for (auto& line : doc.Ls) {

            if (line->Cs.empty()) {

                if (inPara) {
                    maxWords = max(maxWords, currentWords);
                    currentWords = 0;
                    inPara = false;
                }
            }
            else {

                inPara = true;
                bool inWord = false;

                for (auto& character : line->Cs) {

                    if (isValid(character.ch)) {

                        if (!inWord) {
                            currentWords++;
                            inWord = true;
                        }
                    }
                    else {
                        inWord = false;
                    }
                }
            }
        }

        if (inPara) {
            maxWords = max(maxWords, currentWords);
        }

        cout << "Longest Paragraph Length: " << maxWords << "\n";

        return maxWords;
    }
    double avgWordLength() {
        int totalWords = 0;
        int totalLength = 0;
        string currentWord;

        for (auto& line : doc.Ls) {
            for (auto& character : line->Cs) {
                char tempChar = tolower(character.ch); 
                if (isValid(tempChar)) {
                    currentWord += tempChar;
                }
                else {
                    if (!currentWord.empty()) {
                        totalLength += currentWord.length();
                        totalWords++;
                    }
                    currentWord.clear();
                }
            }

            if (!currentWord.empty()) {
                totalLength += currentWord.length();
                totalWords++;
                currentWord.clear();
            }
        }

        if (totalWords == 0) {
            cout << "No words found in the file.\n";
            return 0.0;
        }

        double avgLength = (double)(totalLength) / totalWords;
        return avgLength;
    }
    int substringCount(const string& substring) {
        if (substring.empty()) {
            cout << "Substring cannot be empty.\n";
            return 0;
        }

        string lowerSubstring;
        for (char ch : substring) {
            lowerSubstring += tolower(ch);
        }

        int count = 0;

        for (auto& line : doc.Ls) {
            string currentLine;
            for (auto& character : line->Cs) {
                currentLine += tolower(character.ch); 
            }

            int pos = currentLine.find(lowerSubstring);
            while (pos != string::npos) {
                count++;
                pos = currentLine.find(lowerSubstring, pos + 1); 
            }
        }

        cout << "Substring '" << substring << "' found " << count << " times in the file.\n";
        return count;
    }
    bool canFormWord(const string& baseWord, const string& targetWord) {
        stack<char> baseStack;
        stack<char> targetStack;

        for (char c : baseWord) {
            baseStack.push(c);
        }

        for (char c : targetWord) {
            stack<char> tempStack = baseStack;
            bool found = false;
            while (!tempStack.empty()) {
                if (tempStack.top() == c) {
                    found = true;
                    break;
                }
                tempStack.pop();
            }
            if (!found) {
                return false;
            }
        }
        return true;
    }
    void findWordWithMaxPoints() {
        ifstream file(currFilePath);
        if (!file.is_open()) {
            cout << "Error: Unable to open the file.\n";
            return;
        }

        vector<string> words;
        string word;

        while (file >> word) {
            words.push_back(word);
        }
        file.close();

        string bestWord;
        int maxPoints = 0;
        vector<string> bestWordForms;

        for (const string& baseWord : words) {
            vector<string> formedWords;
            int points = 0;

            for (const string& targetWord : words) {
                if (baseWord != targetWord && canFormWord(baseWord, targetWord)) {
                    points++;
                    formedWords.push_back(targetWord);
                }
            }

            if (points > maxPoints) {
                maxPoints = points;
                bestWord = baseWord;
                bestWordForms = formedWords;
            }
        }

        if (bestWord.empty()) {
            cout << "No word could form other words.\n";
        }
        else {
            cout << "The word with the highest points is: " << bestWord
                << " (forms " << maxPoints << " words)\n";
            cout << "Words it can form:\n";
            for (const string& formedWord : bestWordForms) {
                cout << "- " << formedWord << '\n';
            }
        }
    }
    vector<string> generateEncodingVector() {
        vector<string> encodings(128, ""); 
       srand((unsigned)time(0));


        for (unsigned char c = 0; c < 128; c++) { 
            encodings[c] = string(1, static_cast<char>(33 + rand() % 94)); 
        }

        return encodings;
    }
    void findAndReplaceFromCursor(const string& oldWord, const string& newWord, bool replaceAll) {
        bool found = false;

        auto rowIt = doc.Ls.begin();
        for (int i = 0; i < doc.cursorRow; i++) {
            rowIt++;
        }

        auto colIt = (*rowIt)->Cs.begin();
        for (int i = 0; i < doc.cursorCol; i++) {
            colIt++;
        }

        while (rowIt != doc.Ls.end()) {
            auto it = colIt; 
            while (it != (*rowIt)->Cs.end()) {
                auto start = it;
                auto temp = start;
                int matchLength = 0;

                while (temp != (*rowIt)->Cs.end() && matchLength < oldWord.size() && tolower(temp->ch) == tolower(oldWord[matchLength])) {
                    temp++;
                    matchLength++;
                }

                bool isWordBoundaryStart = (start == (*rowIt)->Cs.begin() || !isValid(myPrev(start)->ch));
                bool isWordBoundaryEnd = (temp == (*rowIt)->Cs.end() || !isValid(temp->ch));

                if (matchLength == oldWord.size() && isWordBoundaryStart && isWordBoundaryEnd) {
                    found = true;

                    for (int i = 0; i < matchLength; i++) {
                        start = (*rowIt)->Cs.erase(start);
                    }

                    for (char c : newWord) {
                        letters newLetter;
                        newLetter.ch = c;
                        (*rowIt)->Cs.insert(start, newLetter);
                    }

                    doc.cursorRow = 0;
                    for (auto iter = doc.Ls.begin(); iter != rowIt; iter++) {
                        doc.cursorRow++;
                    }

                    doc.cursorCol = 0;
                    for (auto iter = (*rowIt)->Cs.begin(); iter != start; iter++) {
                        doc.cursorCol++;
                    }

                    if (!replaceAll) {
                        cout << "Replacement completed at row " << doc.cursorRow << ", col " << doc.cursorCol << ".\n";
                        return;
                    }

                    it = start;
                }
                else {
                    it++;
                }
            }

            rowIt++;
            if (rowIt != doc.Ls.end()) {
                colIt = (*rowIt)->Cs.begin(); 
            }
        }

        if (found) {
            cout << "Replacement completed for all matches from the cursor.\n";
        }
        else {
            cout << "The word '" << oldWord << "' was not found from the current cursor position.\n";
        }
    }    
    void findAndReplaceFromCursorBackward(const string& oldWord, const string& newWord, bool replaceAll) {
        bool found = false;

        string reversedOldWord;
        for (auto it = oldWord.rbegin(); it != oldWord.rend(); it++) {
            reversedOldWord.push_back(*it);
        }

        auto rowIt = doc.Ls.begin();
        for (int i = 0; i < doc.cursorRow; i++) {
            rowIt++;
        }

        auto colIt = (*rowIt)->Cs.begin();
        for (int i = 0; i < doc.cursorCol; i++) {
            colIt++;
        }

        while (true) {
            auto it = colIt; 
            while (it != (*rowIt)->Cs.begin()) {
                auto temp = it;
                int matchLength = 0;

                while (temp != (*rowIt)->Cs.begin() && matchLength < reversedOldWord.size() && tolower(myPrev(temp)->ch) == tolower(reversedOldWord[matchLength])) {
                    temp--;
                    matchLength++;
                }

                bool isWordBoundaryStart = (temp == (*rowIt)->Cs.begin() || !isValid(myPrev(temp)->ch));
                bool isWordBoundaryEnd = (it == (*rowIt)->Cs.end() || !isValid(it->ch));

                if (matchLength == reversedOldWord.size() && isWordBoundaryStart && isWordBoundaryEnd) {
                    found = true;

                    auto matchStart = temp;
                    auto hold = temp;

                    for (int i = 0; i < matchLength; i++) {
                        matchStart = (*rowIt)->Cs.erase(matchStart);
                    }

                    for (char c : newWord) {
                        letters newLetter;
                        newLetter.ch = c;
                        (*rowIt)->Cs.insert(matchStart, newLetter);
                    }

                    doc.cursorRow = 0;
                    for (auto iter = doc.Ls.begin(); iter != rowIt; iter++) {
                        doc.cursorRow++;
                    }

                    doc.cursorCol = 0;
                    for (auto iter = (*rowIt)->Cs.begin(); iter != matchStart; iter++) {
                        doc.cursorCol++;
                    }

                    if (!replaceAll) {
                        cout << "Replacement completed at row " << doc.cursorRow << ", col " << doc.cursorCol << ".\n";
                        return;
                    }

                    it = matchStart;
                }
                else {
                    it--;
                }
            }

            if (rowIt == doc.Ls.begin()) {
                break; 
            }
            rowIt--;
            colIt = (*rowIt)->Cs.end(); 
        }

        if (found) {
            cout << "Replacement completed for all matches from the cursor.\n";
        }
        else {
            cout << "The word '" << oldWord << "' was not found from the current cursor position.\n";
        }
    }
    void encodeDocumentRLE(const Document& doc, const string& outputFile) {
        ofstream output(outputFile);

        if (!output.is_open()) {
            cout << "Error: Unable to create encoded file.\n";
            return;
        }

        for (const auto& line : doc.Ls) {
            string currentLine;
            for (const auto& letter : line->Cs) {
                currentLine += letter.ch; 
            }

            string encodedLine = "";
            int n = currentLine.length();
            for (int i = 0; i < n; ++i) {
                int count = 1;
                while (i + 1 < n && currentLine[i] == currentLine[i + 1]) {
                    count++;
                    i++;
                }
                encodedLine += to_string(count) + currentLine[i];
            }

            output << encodedLine << "\n"; 
        }

        output.close();
        cout << "File encoded successfully! Output saved to " << outputFile << "\n";
    }
    void decodeDocumentRLE(const string& encodedFile, const string& outputFile) {
        ifstream input(encodedFile);
        ofstream output(outputFile);

        if (!input.is_open()) {
            cout << "Error: Unable to open encoded file.\n";
            return;
        }

        if (!output.is_open()) {
            cout << "Error: Unable to create decoded file.\n";
            return;
        }

        string line;
        while (getline(input, line)) {
            string decodedLine = "";
            int n = line.length();

            for (int i = 0; i < n; ++i) {
                string countStr = "";
                while (isdigit(line[i]) && i < n) {
                    countStr += line[i];
                    i++;
                }
                int count = stoi(countStr); 
                char ch = line[i];         
                decodedLine += string(count, ch); 
            }

            output << decodedLine << "\n"; 
        }

        input.close();
        output.close();
        cout << "File decoded successfully! Output saved to " << outputFile << "\n";
    }
    void encodeFile() {
        if (currFilePath.empty()) {
            cout << "No file is currently loaded for encoding.\n";
            return;
        }

        encodeDocumentRLE(doc, "encodedFile.txt");

        cout << "Wanna Open The Encoded File? (1) -> yes \n";
        int ch;
        cin >> ch;

        if (ch == 1) {
            system("notepad encodedFile.txt");
            _getch();
        }
    }
    void decodeFile() {
        string encodedFilePath = "encodedFile.txt";
        string decodedFilePath = "decodedFile.txt";

        decodeDocumentRLE(encodedFilePath, decodedFilePath);

        cout << "Decoded File Generated: Wanna Open The Decoded File? (1) -> yes \n";
        int ch;
        cin >> ch;

        if (ch == 1) {
            system(("notepad " + decodedFilePath).c_str());
            _getch();
        }
    }
    vector<string> wrapText(const string& text, int width) {
        vector<string> lines;
        string::size_type start = 0, end;

        while (start < text.length()) {
            end = start + width;
            if (end >= text.length()) {
                lines.push_back(text.substr(start));
                break;
            }

            // Find the last space within the line width
            string::size_type lastSpace = text.rfind(' ', end);
            if (lastSpace == string::npos || lastSpace < start) {
                lastSpace = end; // If no space found, force wrap
            }
            lines.push_back(text.substr(start, lastSpace - start));
            start = lastSpace + 1;
        }

        return lines;
    }
    void displayCommandBox() {
        vector<pair<string, string>> commands = {
            {"Ctrl + Z", "Undo"},
            {"Ctrl + Y", "Redo"},
            {"Ctrl + U", "To Uppercase"},
            {"Ctrl + L", "To Lowercase"},
            {"Ctrl + F", "Find Word"},
            {"Ctrl + Q", "Find Sentence"},
            {"Ctrl + W", "Find Substring"},
            {"Ctrl + E", "Replace Word, Add Prefix Postfix"},
            {"Ctrl + T", "Count Paragraphs, Sentences, Special Characters, Avg Word Length"},
            {"Ctrl + A", "Find Smallest Word, Largest Paragraph Count, Longest Word"},
            {"Ctrl + S", "Find Substrings Count"},
            {"Ctrl + D", "Word Game"},
            {"Ctrl + X", "Encode File"},
            {"Ctrl + P", "Decode File"},
            {"Ctrl + C", "Find and Replace"}
        };

        const int boxWidth = 70;          
        const int commandKeyWidth = 15;  
        const int descriptionWidth = boxWidth - commandKeyWidth - 6; 

        cout << string(boxWidth, '=') << '\n';

        string title = "COMMAND BOX";
        int padding = (boxWidth - title.size()) / 2;
        cout << string(padding, ' ') << title << string(boxWidth - title.size() - padding, ' ') << '\n';

        cout << string(boxWidth, '-') << '\n';

        for (const auto& command : commands) {
            vector<string> wrappedDescription = wrapText(command.second, descriptionWidth);

            cout << "| " << setw(commandKeyWidth) << left << command.first
                << "| " << setw(descriptionWidth) << left << wrappedDescription[0] << " |\n";

            for (int i = 1; i < wrappedDescription.size(); i++) {
                cout << "| " << setw(commandKeyWidth) << " " 
                    << "| " << setw(descriptionWidth) << wrappedDescription[i] << " |\n";
            }
        }

        cout << string(boxWidth, '=') << '\n';
    }
    void display() const {
        system("CLS");
        doc.displayDocument();
    }
    void runEditor() {
        display(); 

        while (true) {
            char input = _getch(); 
            if (input == 27) { 
                system("cls");
                if (!currFilePath.empty()) {
                    saveFile(currFilePath);
                }
                else {
                    cout << "Error: No file path provided for saving:/\n";
                }
                break;
            }
            else if (input == -32) { 
                input = _getch(); 
                auto it = next(doc.Ls.begin(), doc.cursorRow); 
                auto line = *it;

                switch (input) {
                case 72: // Up 
                    if (doc.cursorRow > 0) {
                        doc.cursorRow--;
                        it = next(doc.Ls.begin(), doc.cursorRow); 
                        line = *it;
                        if (doc.cursorCol > line->Cs.size()) {
                            doc.cursorCol = line->Cs.size(); 
                        }
                    }
                    break;
                case 80: // Down 
                    if (doc.cursorRow < doc.Ls.size() - 1) {
                        doc.cursorRow++;
                        it = next(doc.Ls.begin(), doc.cursorRow); 
                        line = *it;
                        if (doc.cursorCol > line->Cs.size()) {
                            doc.cursorCol = line->Cs.size(); 
                        }
                    }
                    break;
                case 75: // Left 
                    if (doc.cursorCol > 0) {
                        doc.cursorCol--;
                    }
                    else if (doc.cursorRow > 0) { 
                        //case when we are at the start of next line and press the left arrow than 
                        //move to the end of previous line
                        doc.cursorRow--;
                        it = next(doc.Ls.begin(), doc.cursorRow); 
                        line = *it;
                        doc.cursorCol = line->Cs.size(); 
                    }
                    break;
                case 77: // Right 
                    if (doc.cursorCol < line->Cs.size()) {
                        doc.cursorCol++;
                    }
                    else if (doc.cursorRow < doc.Ls.size() - 1) { 
                        doc.cursorRow++;
                        doc.cursorCol = 0;
                    }
                    break;
                }
            }
            else if (input == 8) { //backspace
                saveState();
                auto it = next(doc.Ls.begin(), doc.cursorRow); 
                auto line = *it;

                if (doc.cursorCol > 0) { 
                    auto charIt = next(line->Cs.begin(), doc.cursorCol - 1);
                    line->Cs.erase(charIt);
                    doc.cursorCol--;
                }
                else if (doc.cursorRow > 0) { 
                    auto prevIt = prev(it); 
                    auto prevLine = *prevIt;

                    doc.cursorCol = prevLine->Cs.size(); 

                    for (auto& ch : line->Cs) {
                        prevLine->Cs.push_back(ch);
                    }

                    delete line; 
                    doc.Ls.erase(it); 
                    doc.cursorRow--; 
                }
            }
            else if (input == 13) {//enter
                saveState();
                auto it = next(doc.Ls.begin(), doc.cursorRow);
                auto line = *it;

                Line* newLine = new Line();
                
                if (doc.cursorCol < line->Cs.size()) {

                    auto splitPos = next(line->Cs.begin(), doc.cursorCol);

                    while (splitPos != line->Cs.end()) {

                        newLine->Cs.push_back(*splitPos);
                        splitPos = line->Cs.erase(splitPos);
                    }
                }
                doc.Ls.insert(next(it),newLine);
                doc.cursorRow++;
                doc.cursorCol = 0;
            }
            else if (input == 26) { //ctrl + z
                undo();
            }
            else if (input == 25) { //ctrl + y
                redo();
            }
            else if (input == 21) {//ctrl+u
                saveState();
                system("cls");
                cout << "Current Word Or String? (1)-> word (2)-> Sentence: ";
                int ch;
                cin >> ch;
                if (ch == 1) {
                    toUpper();
                }
                else if (ch == 2) {
                    toUpperExtend();
                }
            }
            else if (input == 12) {//ctrl+l
                saveState();
                system("cls");
                cout << "Current Word Or String? (1)-> word (2)-> Sentence: ";
                int ch;
                cin >> ch;
                if (ch == 1) {
                    toLower();
                }
                else if (ch == 2) {
                    toLowerExtend();
                }
            }
            else if (input == 6) { //ctrl + f
                system("cls");
                cout << "Enter Word To Find: ";
                string word;
                cin.ignore();
                getline(cin, word);
                findWord(word);
                _getch();
            }
            else if (input == 17) { //ctrl + q
                system("cls");
                cout << "Enter Sentence To Find:";
                string sentence;
                cin.ignore(); 
                getline(cin, sentence); 
                findSentence(sentence);
                _getch();
            }
            else if (input == 23) { //ctrl + w
                system("cls");
                cout << "Enter A Word To Find Substrings For:";
                string word;
                cin.ignore();
                getline(cin, word);
                findSubstring(word);
                _getch();
            }
            else if (input == 5) { // ctrl + e
                saveState();
                system("cls");

                cout << "Choose an operation:\n";
                cout << "1. Replace a word\n";
                cout << "2. Add a prefix to a word\n";
                cout << "3. Add a postfix to a word\n";
                int operation;
                cin >> operation;

                if (operation == 1) {
                    cout << "Enter the word to find: ";
                    string oldWord;
                    cin >> oldWord;

                    cout << "Enter the replacement word: ";
                    string newWord;
                    cin >> newWord;

                    if (oldWord.empty() || newWord.empty()) {
                        cout << "Words cannot be empty!!\n";
                        break;
                    }

                    cout << "Replace (1) First Occurrence or (2) All Occurrences? ";
                    int choice;
                    cin >> choice;

                    findAndReplace(oldWord, newWord, choice == 2);
                }
                else if (operation == 2) {
                    cout << "Enter the word to add a prefix to: ";
                    string word;
                    cin >> word;

                    cout << "Enter the prefix: ";
                    string prefix;
                    cin >> prefix;

                    if (word.empty() || prefix.empty()) {
                        cout << "Words cannot be empty!!\n";
                        break;
                    }

                    addPrefix(word, prefix);
                }
                else if (operation == 3) {
                    cout << "Enter the word to add a postfix to: ";
                    string word;
                    cin >> word;

                    cout << "Enter the postfix: ";
                    string postfix;
                    cin >> postfix;

                    if (word.empty() || postfix.empty()) {
                        cout << "Words cannot be empty!!\n";
                        break;
                    }

                    addPostfix(word, postfix);
                }
                else {
                    cout << "Invalid operation!\n";
                }
                _getch();
            }
            else if (input == 20) { //ctrl+t

                system("cls");

                cout << "Paragraph Count: " << countParas() << "\n";
                Sleep(1000);
                cout << "Sentence Count: " << countSentences() << "\n";
                Sleep(1000);
                cout << "Special Character Count: " << countSpecialChars() << "\n";
                Sleep(1000);
                cout << "Average Word Length: " << avgWordLength() << "\n";
                _getch();
            }
            else if (input == 1) { //ctrl + a
                system("cls");

                cout << "Choose an operation:\n";
                cout << "1. Find smallest word length\n";
                cout << "2. Find paragraph with largest word count\n";
                cout << "3. Find longest word\n";
                int choice;
                cin >> choice;

                if (choice == 1) {

                    smallestWordLength();
                }
                else if (choice == 2) {
                    largestParaLength();

                }

                else if (choice == 3) {
                    longestWord();
                }
                else {
                    cout << "Invalid choice!\n";
                }
                _getch();
            }
            else if (input == 19) { //ctrl + s

                system("cls");
                cout << "Enter A Word To Find Substrings For:";
                string word;
                cin.ignore();
                getline(cin, word);
                substringCount(word);
                _getch();
            }
            else if (input == 4) { //ctrl+d
                saveFile(currFilePath);
                system("cls");
                findWordWithMaxPoints();
                _getch();
            }
            else if (input == 2) { //ctrl + b
                system("cls");
                displayCommandBox();
                _getch();
            }

            else if (input == 24) { //ctrl+x
                system("cls");
                displayLoadingBar2();
                system("cls");
                encodeFile();
                _getch();
            }
            else if (input == 16) { //ctrl+P
                system("cls");
                displayLoadingBar3();
                system("cls");
                decodeFile();
                _getch();
            }
            else if (input == 3) {
                saveState();//ctrl + c
                system("cls");

                cout << "FIND AND REPLACE: \n";
                cout << "Enter Choice For The Following Operations: \n";
                cout << "1. ( Next, Next All )\n2. ( Previous, Previous All )\n";

                int ch;
                cin >> ch;

                if (ch >= 1 && ch <= 2) {
                    cout << "Enter the word to find: ";
                    string oldWord;
                    cin >> oldWord;

                    cout << "Enter the replacement word: ";
                    string newWord;
                    cin >> newWord;

                    if (oldWord.empty() || newWord.empty()) {
                        cout << "Words cannot be empty!!\n";
                        break;
                    }

                    cout << "Replace (1) First Occurrence or (2) All Occurrences? ";
                    int choice;
                    cin >> choice;
                    if (ch == 1) {
                    
                        findAndReplaceFromCursor(oldWord,newWord,choice==2);
                    }
                    else {
                        findAndReplaceFromCursorBackward(oldWord, newWord, choice == 2);
                    }
                }
                else {
                    cout << "Invalid Selection!!\n";
                }
                _getch();
            }
            else if (input == 29) {
                saveFile(currFilePath);
            }
           
            else { //add char at the cursor pos
                saveState();
                auto it = next(doc.Ls.begin(), doc.cursorRow); 
                auto line = *it;
                auto charIt = next(line->Cs.begin(), doc.cursorCol);
                line->addChar(input, charIt);
                doc.cursorCol++;
            }

            display();
        }
    }
};