#pragma once
#include "tree.h"
#include <iostream>
#include <string>
#include <ctime>
#include <fstream>
#include <ostream>
#include <queue>
#include "textEditor.h"
#include "line.h"
#include "para.h"
#include <algorithm>

using namespace std;

class DOS
{
private:
     string toLower(const  string& str) {
         string lowerStr = str;
        for (int i = 0; i < lowerStr.size(); i++) {
            char ch = str[i];
            lowerStr[i] = tolower(ch);
        }
        return lowerStr;
    }
     bool toggle;
     bool promptDone;
     string password = "GTR35";

public:
    Tree fileTree;
    DOS() {
         cout << "Name: Mohid Faisal \nRoll No: BSCS23092 \nCourse: DSA\n\n";
         toggle = false;
         promptDone = false;
        fileTree.printPrompt(toggle);
    }
    bool checkPassword(string pw) {

        if (pw.length() != password.length()) {
            return false;
        }

        for (int i = 0; i < pw.length(); i++) {
            if (pw[i] != password[i]) {
                return false;
            }
        }
        return true;
    }
    void displayLoadingBar() {

        system("cls");
        SetConsoleCP(437);
        SetConsoleOutputCP(437);
        int bar1 = 177;
        int bar2 = 219;

        cout << "\n\n\n\t\t\t\Opening Your File In Text Editor...";
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
    string extractFileName(const string& filePath) {
        size_t lastSlashPos = filePath.find_last_of("\\");
        if (lastSlashPos == string::npos) {
            // No backslash found, the input might just be the file name
            return filePath;
        }

        // Extract the substring after the last backslash
        string fileName = filePath.substr(lastSlashPos + 1);

        // Remove trailing backslash if present
        if (!fileName.empty() && fileName.back() == '\\') {
            fileName.pop_back();
        }

        return fileName;
    }
    void run() {
        bool exit = false;
        while (!exit) {
            exit = handleInput();
        }
    }
    string customSubstr(const  string& str, int start, int length) {

        if (start >= str.length()) {
            return "";
        }
        if (start + length > str.length()) {
            length = str.length() - start;
        }

         string result;

        for (int i = 0; i < length; i++) {
            result += str[start + i]; 
        }

        return result; 
    }
    void convertExtensions(const string& oldExt, const string& newExt) {
         fileTree.getCurrentFolder()->convertFileExtensions(oldExt, newExt);
     }
    void createFile(const  string& fileName) {


         int priority = 1;
         Folder* currentFolder = fileTree.getCurrentFolder();
         string uniqueFileName = currentFolder->name + "_" + fileName;

         if (currentFolder->fileExists(uniqueFileName)) {
              cout << "Error: File already exists in this directory.\n";
             return;
         }

            currentFolder->addFile(uniqueFileName);

            printJob pj;
            pj.fileName = uniqueFileName;
            pj.priorityLevel = priority;
            pj.timeLeft = 5 * priority;
           
            fileTree.printingQueue.push(pj);
            fileTree.fileTrackerQueue.push(uniqueFileName);


          string filePath = currentFolder->getPath() + fileName;

          ofstream outFile(uniqueFileName);
         if (!outFile) {
              cout << "Error: Unable to create file on disk.\n";
             return;
         }

         string line = "This Is A Blank File....";
         bool once = false;

         do {
             currentFolder->setFileContent(uniqueFileName, line);
             outFile << line << "\n";

             once = true;
         } while (!once);
         

         outFile.close();

         cout << "File '" << uniqueFileName << "' created. Opening in Text Editor...\n";

         TextEditor ed;
         ed.loadFile(uniqueFileName);
         ed.runEditor();

  
         cout << "File '" << uniqueFileName << "' created and saved at " << filePath << ".\n";

     }
    bool handleInput() {
          string command;
          getline( cin, command); 

         int spacePos = command.find(' '); 
         string cmd;

         if (spacePos != -1) {
             cmd = customSubstr(command, 0, spacePos); 
         }
         else {
             cmd = customSubstr(command, 0, command.length()); 
         }

         cmd = toLower(cmd); 

         if (cmd == "attrib") {
             if (spacePos != -1) {
                 displayAttributes(customSubstr(command, spacePos + 1, command.length() - spacePos - 1)); 
             }
             else {
                  cout << "Usage: attrib <filename>\n"; 
             }
         }
         else if (cmd == "cd") {
             if (spacePos != -1) {
                 changeDirectory(customSubstr(command, spacePos + 1, command.length() - spacePos - 1)); 
                 
             }
             else {
                  cout << "Usage: cd <foldername>\n"; 
             }
         }
         else if (cmd == "create") {
             if (spacePos != -1) {
                 //add a bool to ask user whether he wants to write to the new file, if yes than
                 //execute the text editor for writting
                 //if no than write a garbage line and save the file for later editing use.
                 createFile(customSubstr(command, spacePos + 1, command.length() - spacePos - 1));
             }
             else {
                  cout << "Usage: create <filename>\n";
             }
         }
         else if (cmd == "rmdir") {
             if (spacePos != -1) {
                 string folderName = customSubstr(command, spacePos + 1, command.length() - spacePos - 1);
                 if (folderName.empty()) {
                     cout << "Usage: rmdir <foldername>\n";
                 }
                 else {
                     fileTree.removeDirectory(folderName);
                 }
             }
             else {
                 cout << "Usage: rmdir <foldername>\n";
             }
         }
         else if (cmd == "print") {
             bool isAdded = false;
             bool go = true;
             if (spacePos != -1) {
                 command = command.substr(cmd.length() + 3, command.length());
                 string src = command.substr(0, command.find(' '));
                 string fileName = fileTree.extractFileName(src);

                 queue<string> temp = fileTree.fileTrackerQueue;

                 if (!fileTree.searchFileBOOL(src)) {
                     cout << "File '" << fileName << " does not exist in the tree structure!\n";
                     go = false;
                 }

                 if (go) {
                     while (!temp.empty()) {
                         string currFile = temp.front();

                         if (currFile == fileName) {
                             cout << "File with name " << fileName << " already exists!!\n";
                             isAdded = true;
                         }
                         temp.pop();
                     }
                     if (!isAdded) {
                         fileTree.fileTrackerQueue.push(fileName);
                         displayFileQueue();
                     }
                 }
             }
             else {
                 cout << "Usage: print <filename>\n";
             }
         }
         else if (cmd == "queue") {
             queueCommand();
         }
         else if (cmd == "pprint") {
             if (spacePos != -1) {
                 string fileName = customSubstr(command, spacePos + 1, command.length() - spacePos - 1);
                 pprintCommand(fileName);
             }
             else {
                 cout << "Usage: pprint <filename>\n";
             }
         }
         else if (cmd == "pqueue") {
             displayPrintQueue();
         }
         else if (cmd == "cls") {
             system("cls");
         }
         else if (cmd == "cd.") {
             printCurrentDirectory();
         }
         else if (cmd == "cd..") {
             changeToParentDirectory();
         }
         else if (cmd == "cd\\") {
             changeToRootDirectory();
         }
         else if (cmd == "prompt") {
             toggle = !toggle;
         }
         else if (cmd == "exit") {
             return true; 
         }
         else if (cmd == "convert") {
             if (spacePos != -1) {
                 string args = customSubstr(command, spacePos + 1, command.length() - spacePos - 1);
                 int argPos = args.find(' ');
                 if (argPos != -1) {
                     string oldExt = customSubstr(args, 0, argPos);
                     string newExt = customSubstr(args, argPos + 1, args.length() - argPos - 1);

                     convertExtensions(oldExt, newExt);
                 }
                 else {
                     cout << "Usage: convert <old_extension> <new_extension>\n";
                 }
             }
             else {
                 cout << "Usage: convert <old_extension> <new_extension>\n";
             }
         }
         else if (cmd == "del") {
             if (spacePos != -1) {
                 deleteFile(customSubstr(command, spacePos + 1, command.length() - spacePos - 1));
             }
             else {
                  cout << "Usage: del <filename>\n";
             }
         }
         else if (cmd == "dir") {
             fileTree.displayCurrentDirectoryContents();
         }
         else if (cmd.substr(0, 5) == "mkdir") {
             if (command.length() > 6) {
                 string folderName = command.substr(6); 
                 if (folderName.empty()) {
                     cout << "Error: Directory name cannot be empty.\n";
                 }
                 else {
                     fileTree.createDirectory(folderName);
                 }
             }
             else {
                 cout << "Error: Directory name cannot be empty.\n";
             }
         }
         else if (cmd == "search") {
             if (spacePos != -1) {

                 command = command.substr(cmd.length() + 3, command.length());
                 string src = command.substr(0, command.find(' '));
                 fileTree.searchFile(src);
                 //searchFile(customSubstr(command, spacePos + 1, command.length() - spacePos - 1));
             }
             else {
                  cout << "Usage: search <filename>\n";
             }
         }
         else if (cmd.substr(0, 4) == "move") {
             command = command.substr(cmd.length() + 3, command.length());
             int pos = command.find(' ');
             string src = command.substr(0, command.find(' '));
             string dest = command.substr(src.length() + 3, command.length());

             if (pos != -1) {
                 if (!fileTree.moveFile(src, dest)) {
                     cout << "Error occurred during file move.\n";
                 }
             }
             else {
                 cout << "Usage: move <source_path> <destination_path>\n";
             }
             }
         else if (cmd == "rename") {
             if (spacePos != -1) {
                 string args = customSubstr(command, spacePos + 1, command.length() - spacePos - 1);
                 int argPos = args.find(' ');
                 if (argPos != -1) {
                     string currentName = customSubstr(args, 0, argPos);
                     string newName = customSubstr(args, argPos + 1, args.length() - argPos - 1);

                     if (currentName.empty() || newName.empty()) {
                         cout << "Error: Both current and new file names are required.\n";
                     }
                     else {
                         fileTree.renameFile(currentName, newName);
                     }
                 }
                 else {
                     cout << "Usage: rename <current_name> <new_name>\n";
                 }
             }
             else {
                 cout << "Usage: rename <current_name> <new_name>\n";
             }
             }
         else if (cmd == "format") {
             fileTree.formatCurrentFolder();
             }
         else if (cmd == "tree") {
                 fileTree.displayTree();
         }
         else if (cmd == "ver") {
             cout << "Microsoft Windows [Version 10.0.22631.4460]\n(c)Microsoft Corporation.All rights reserved.\n";
         }
         else if (cmd == "pwd") {
             fileTree.displayCurrentDirectoryPath();
         }
         else if (cmd == "find") {
             if (spacePos != -1) {
                 string fileName = customSubstr(command, spacePos + 1, command.length() - spacePos - 1);
                 string* filePtr = fileTree.findFile(fileName);
                 if (filePtr != NULL) {
                     cout << "Pointer to file '" << fileName << "' retrieved.\n";
                 }
                 else {
                     cout << "File '" << fileName << "' does not exist in the current directory.\n";
                 }
             }
             else {
                 cout << "Usage: find <filename>\n";
             }
         }
         else if (cmd.substr(0, 4) == "copy") {

             command = command.substr(cmd.length() + 3, command.length());
             int pos = command.find(' ');
             string src = command.substr(0, command.find(' '));
             string dest = command.substr(src.length() + 3, command.length());

             if (pos != -1) {
                 if (!fileTree.copyFile(src, dest)) {
                     cout << "Error occurred during file copy.\n";
                 }
             }
             else {
                 cout << "Usage: copy <source_path> <destination_path>\n";
             }
         }
         else if (cmd.substr(0, 4) == "edit") {

             string fileName = customSubstr(command, spacePos + 1, command.length() - spacePos - 1);
             if (spacePos != -1) {
                 cout << "Enter Password To Load And Edit The File: ";
                 string pw;
                 cin >> pw;
                 if (checkPassword(pw)) {
                     cout << "Password Matches!!\n Press Any Key To Continue!!\n";
                     _getch();

                     if (fileTree.loadFileToList(customSubstr(command, spacePos + 1, command.length() - spacePos - 1))) {
                         //cout << "File Loaded!!\n";
                         system("cls");
                         displayLoadingBar();
                         system("cls");
                         TextEditor editor;
                         editor.loadFile(customSubstr(command, spacePos + 1, command.length() - spacePos - 1));
                         editor.runEditor();
                     }
                     else {
                         cout << "Error: cannot load the file for editing!!\n";
                     }
                 }
                 else {
                     cout << "Invalid Password!!\n";
                 }
             }
             else {
                 cout << "Usage: edit <filename>\n";
             }
         }
         else if (command == "merge") {
             string file1, file2;
             cout << "Enter the name of the first file: ";
             cin >> file1;
             cout << "Enter the name of the second file: ";
             cin >> file2;

             mergeFiles(file1, file2);

             cout << "Do You Want To Open The Merged File? (1) Yes (0) No: ";
             int ch;
             cin >> ch;

             if (ch == 1) {
                 TextEditor editor;
                 editor.loadFile(file1);
                 editor.runEditor();
             }
             else {
                 system("cls");
             }
         }
         else if (cmd == "loadtree") {
             if (fileTree.loadTree("mytree.txt")) {
                 cout << "Tree structure successfully loaded from 'tree.txt'.\n";
             }
             else {
                 cout << "Failed to load tree structure from 'tree.txt'.\n";
             }
             }
         else if (cmd == "findf") {
             if (spacePos != -1) {
                 string args = customSubstr(command, spacePos + 1, command.length() - spacePos - 1);
                 int argPos = args.find(' ');
                 if (argPos != -1) {
                     string fileName = customSubstr(args, 0, argPos);
                     string searchString = customSubstr(args, argPos + 1, args.length() - argPos - 1);

                     findfCommand(fileName, searchString);
                 }
                 else {
                     cout << "Usage: findf <filename> <search_string>\n";
                 }
             }
             else {
                 cout << "Usage: findf <filename> <search_string>\n";
             }
         }
         else if (cmd == "help") {
             displayHelp();
         }
         else {
              cout << "Unknown command: " << cmd << "\n"; 
         }

         fileTree.printPrompt(toggle);
         return false;
     }
    void displayAttributes(const  string& fileName) {
        Folder* currentFolder = fileTree.getCurrentFolder();
        if (currentFolder->fileExists(fileName)) {
            currentFolder->printFileAttributes(fileName);
        }
        else {
             cout << "File '" << fileName << "' not found.\n";
        }
    }
    void changeDirectory(const  string& folderName) {
        if (fileTree.folderExists(folderName)) {
            fileTree.setCurrentFolder(fileTree.findFolder(folderName));
        }
        else {
             cout << "Directory '" << folderName << "' does not exist.\n";
        }
    }
    void printCurrentDirectory()  {
        Folder* currentFolder = fileTree.getCurrentFolder();
         cout << "Current directory: " << currentFolder->getPath() << "\\" << currentFolder->getName() << "\n";
    }
    void changeToParentDirectory() {
        Folder* currentFolder = fileTree.getCurrentFolder();
        if (currentFolder->getParent() != NULL) {
            fileTree.setCurrentFolder(currentFolder->getParent());
        }
    }
    void changeToRootDirectory() {
        fileTree.setCurrentFolder(fileTree.getRoot());
    }
    void deleteFile(const  string& fileName) {
        Folder* currentFolder = fileTree.getCurrentFolder();
        currentFolder->deleteFile(fileName);
    }
    void searchFile(const  string& fileName) {
        Folder* currentFolder = fileTree.getCurrentFolder();

        if (currentFolder->fileExists(fileName)) {
             string filePath = currentFolder->getPath() + "/" + fileName + ".txt";
             cout << "File '" << fileName << "' found at path: " << filePath << "\n";
        }
        else {
             cout << "File '" << fileName << "' not found in the current directory.\n";
        }
    }
    void queueCommand() {

        if (fileTree.fileTrackerQueue.empty()) {
            cout << "Printing Queue Is Empty!! :/\n";
            return;
        }

        cout << "-- Print Queue --\n";
        cout << "File Name | Estimated Time Left\n";
        cout << "--------------------------------\n";

        queue<string> tempQueue = fileTree.fileTrackerQueue;
        int defaultTime = 5;

        while (!tempQueue.empty()) {

            string currFile = tempQueue.front();
            tempQueue.pop();

            cout << currFile << " | " << defaultTime << "s\n";
        }
    }
    void pprintCommand(const string& fileName) {
        
        bool isAdded = false;

        priority_queue<printJob> temp = fileTree.printingQueue;

        while(!temp.empty()){
        
            printJob oldFile = temp.top();

            if (oldFile.fileName == fileName) {
                cout << "File " << fileName << " already exists!! \n";
                isAdded = true;
            }
            temp.pop();
        }

        if (!isAdded) {
            printJob pj;
            pj.fileName = fileName;
            pj.priorityLevel = 1;
            pj.timeLeft = 5;

            fileTree.printingQueue.push(pj);
            cout << "File " << fileName << " added to the priority queue!!\n";
            displayPrintQueue();
        }
    }
    void updatePrintQueue() {

        vector<printJob> pjs;

        while (!fileTree.printingQueue.empty()) {

            printJob pj = fileTree.printingQueue.top();
            fileTree.printingQueue.pop();

            pj.timeLeft = max(0, pj.timeLeft - 1);

            if (pj.timeLeft <= 3) {
                pj.priorityLevel = 2;
            }
            pjs.push_back(pj);
        }

        sort(pjs.begin(), pjs.end());

        for (const auto& pj : pjs) {
            fileTree.printingQueue.push(pj);
        }
    }
    void displayFileQueue() {

        if (fileTree.fileTrackerQueue.empty()) {
            cout << "The file queue is empty!! \n";
            return;
        }

        cout << "-- Current File Queue Status: --\n";
        queue<string> tempQueue = fileTree.fileTrackerQueue;

        cout << "Files: \n";
        while (!tempQueue.empty()) {
            cout << "|_" << tempQueue.front() << "\n";
            tempQueue.pop();
        }
    }
    void displayPrintQueue() {

        updatePrintQueue();

        if (fileTree.printingQueue.empty()) {
            cout << "The print queue is empty.\n";
            return;
        }

        cout << "-- Priority Print Queue --\n";
        cout << "Priority | Time Left | File Name\n";
        cout << "----------------------------------\n";

        auto tempQueue = fileTree.printingQueue;
        while (!tempQueue.empty()) {
            printJob job = tempQueue.top();
            tempQueue.pop();

            cout << "   " << job.priorityLevel << "      |    " << job.timeLeft << "s    | " << job.fileName << "\n";
        }
    }
    void mergeFiles(const string& file1, const string& file2) {

        auto& files = fileTree.current->files;

        if (find(files.begin(), files.end(), file1) == files.end()) {
            cout << "Error: File '" << file1 << "' does not exist in the current directory.\n";
            return;
        }

        if (find(files.begin(), files.end(), file2) == files.end()) {
            cout << "Error: File '" << file2 << "' does not exist in the current directory.\n";
            return;
        }

        ofstream outFile(file1, ios::app);
        if (!outFile) {
            cout << "Error: Unable to open file '" << file1 << "' for writing.\n";
            return;
        }

        ifstream inFile(file2);
        if (!inFile) {
            cout << "Error: Unable to open file '" << file2 << "' for reading.\n";
            return;
        }

        string line;
        while (getline(inFile, line)) {
            outFile << line << '\n';
        }

        inFile.close();
        outFile.close();

        cout << "Successfully merged '" << file2 << "' into '" << file1 << "'.\n";
    }
    string DloadFileContent(const string& filePath) {
        ifstream file(filePath); 
        if (!file) {
            return ""; 
        }

        string content = "";
        char ch;

        while (file.get(ch)) {
            content += ch; 
        }

        file.close(); 
        return content;
    }
    void findfCommand(const string& fileName, const string& searchString) {
        string fileContent;
        bool fileLoaded = false;

        if (fileName.empty()) {
            cout << "Error: No file is currently open.\n";
            return;

            fileLoaded = !fileContent.empty();
        }
        else {
            string* filePtr = fileTree.findFile(fileName);
            if (filePtr != nullptr) {
                fileContent = DloadFileContent(fileName);
                fileLoaded = !fileContent.empty();
            }
            else {
                cout << "Error: File '" << fileName << "' not found.\n";
                return;
            }
        }

        if (!fileLoaded) {
            cout << "Error: Unable to load the file.\n";
            return;
        }

        string lowerContent = toLower(fileContent);
        string lowerSearch = toLower(searchString);

        int pos = lowerContent.find(lowerSearch);
        if (pos != string::npos) {
            cout << "Search string '" << searchString << "' found in the file.\n";
        }
        else {
            cout << "Search string '" << searchString << "' not found in the file.\n";
        }
    }
    void displayHelp()  {
        cout << "+----------------------------------------------------------+\n";
        cout << "|                    Command Help Menu                     |\n";
        cout << "+---------------------+------------------------------------+\n";
        cout << "| Command             | Description                        |\n";
        cout << "+---------------------+------------------------------------+\n";
        cout << "| attrib <filename>   | Displays attributes of a file.     |\n";
        cout << "| cd <foldername>     | Changes the current directory.     |\n";
        cout << "| cd.                 | Displays the current directory.    |\n";
        cout << "| cd..                | Moves to the parent directory.     |\n";
        cout << "| cd\\                 | Moves to the root directory.       |\n";
        cout << "| create <filename>   | Creates a new file.                |\n";
        cout << "| rmdir <foldername>  | Removes a directory.               |\n";
        cout << "| print <filename>    | Adds a file to the print queue.    |\n";
        cout << "| queue               | Displays all queued files.         |\n";
        cout << "| pprint <filename>   | Prints the specified file.         |\n";
        cout << "| pqueue              | Displays the print queue.          |\n";
        cout << "| cls                 | Clears the screen.                 |\n";
        cout << "| prompt              | Toggles the custom prompt.         |\n";
        cout << "| exit                | Exits the shell.                   |\n";
        cout << "| convert <old> <new> | Converts file extensions.          |\n";
        cout << "| del <filename>      | Deletes a file.                    |\n";
        cout << "| dir                 | Displays contents of the directory.|\n";
        cout << "| mkdir <foldername>  | Creates a new directory.           |\n";
        cout << "| search <filename>   | Searches for a file.               |\n";
        cout << "| move <src> <dest>   | Moves a file to a new location.    |\n";
        cout << "| rename <old> <new>  | Renames a file.                    |\n";
        cout << "| format              | Formats the current folder.        |\n";
        cout << "| tree                | Displays the folder structure.     |\n";
        cout << "| ver                 | Displays the system version.       |\n";
        cout << "| pwd                 | Displays the current path.         |\n";
        cout << "| find <filename>     | Finds a file                       |\n";
        cout << "| copy <src> <dest>   | Copies a file to a new location.   |\n";
        cout << "| edit <filename>     | Opens the text editor for editing. |\n";
       // cout << "| merge               | Merges two files.                  |\n";
        //cout << "| loadtree            | Loads the folder tree from a file. |\n";
        cout << "| findf <file> <str>  | Finds a string in the file.        |\n";
        cout << "| help                | Displays this help menu.           |\n";
        cout << "+---------------------+------------------------------------+\n";
        cout << "| Tip: Use commands carefully to avoid unintended actions. |\n";
        cout << "+----------------------------------------------------------+\n";
    }
};