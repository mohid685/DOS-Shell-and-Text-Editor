#pragma once
#include "folder.h"
#include <iostream>
#include <fstream>
#include <queue>
#include <stack>
#include <list>
#include <Windows.h>
#include <conio.h>


using namespace std;

struct printJob {

    string fileName;
    int priorityLevel;
    int timeLeft;

    bool operator <(const printJob& other)const {
        return priorityLevel < other.priorityLevel || (priorityLevel == other.priorityLevel && timeLeft > other.timeLeft);
    }
};
class Tree {
public:
    Folder* root;       
    Folder* current; 
    queue<string> fileTrackerQueue;
    priority_queue<printJob> printingQueue;
    list<string> fileNames;

    Tree() {
        root = new Folder("V", NULL);  
        current = root;                   
    }
    void saveFolder( ofstream& outFile, Folder* folder) {
        outFile << "Folder:" << folder->getName() << "\n";
        for (const auto& file : folder->getFiles()) {
            outFile << "File:" << file << "\n";
        }
        for (auto* subFolder : folder->getSubFolders()) {
            saveFolder(outFile, subFolder);
        }
        outFile << "EndFolder\n";
    }
    void printPrompt(bool toggle)  {
        if (!toggle) {
            cout << current->getPath() << current->getName() << "> ";
        }
        else {
            cout << current->getPath() << current->getName() << "$ ";
        }
    }
    bool folderExists(const  string& folderName)  {
        return current->folderExists(folderName);
    }
    Folder* findFolder(const  string& folderName)  {
        return current->findSubFolder(folderName);
    }
    void setCurrentFolder(Folder* folder) {
        current = folder;
    }
    Folder* getCurrentFolder()  {
        return current;
    }
    Folder* getRoot()  {
        return root;
    }
    void displayCurrentDirectoryContents() const {
        current->displayContents();
    }
    bool createDirectory(const string& folderName) {
        return current->createSubFolder(folderName);
    }
    void formatCurrentFolder() {
        current->formatFolder();
        cout << "The current directory '" << current->getName() << "' has been formatted.\n";
    }
    void displayTree() const {
        cout << "<ROOT>" << "\n";
        root->printTree(2); 
    }
    void displayCurrentDirectoryPath() const {
        cout << "<PRESENT WORKING DIRECTORY>\n";
        current->printWorkingDirectory();
    }
    bool renameFile(const string& currName, const string& newName) {

        Folder* currFolder = getCurrentFolder();
        if (currFolder == NULL) {
            cout << "Error: Current folder is Null :/\n";
            return false;
        }

        currFolder->renameFile(currName, newName);

        queue<string> updatedQueue;

        while (!fileTrackerQueue.empty()) {
            string oldFileName = fileTrackerQueue.front();
            fileTrackerQueue.pop();

            if (oldFileName == currName) {
                updatedQueue.push(newName);
            }
            else {
                updatedQueue.push(oldFileName);
            }
        }
        fileTrackerQueue = move(updatedQueue);

        priority_queue<printJob> updatedQueue2;

        while (!printingQueue.empty()) {
            printJob oldJob = printingQueue.top();
            printingQueue.pop();

            if (oldJob.fileName == currName) {
                oldJob.fileName = newName;
            }
            updatedQueue2.push(oldJob);
        }
        printingQueue = move(updatedQueue2);

        return true;
    }
    bool removeDirectory(const string& folderName) {
        return current->removeSubfoler(folderName);
    }
    string* findFile(const string& fileName) {

        Folder* currFolder = getCurrentFolder();

        if (currFolder == NULL) {
            cout << "Error: Current folder was null:/\n";
            return NULL;
        }

        string* currFile = currFolder->findFile(fileName);

        if (currFile == NULL) {
            cout << "File " << fileName << " not found in the current directory\n";
        }
        else {
            cout << "File " << fileName << " found in the current directory\n";
        }
        return currFile;
    }
    bool copyFile(const string& sourcePath, const string& destPath) {
        Folder* currentFolder = root;
        string tempPath = sourcePath;
        string fileName;

        while (!tempPath.empty()) {
            int pos = tempPath.find("\\");
            string folderName;
            if (pos == -1) {
                folderName = tempPath;
            }
            else {
                folderName = tempPath.substr(0, pos);
            }

            if (pos == -1) {
                tempPath = "";
            }
            else {
                tempPath = tempPath.substr(pos + 1);
            }

            if (tempPath.empty()) {
                fileName = folderName;
            }
            else {
                currentFolder = currentFolder->findFolder(folderName);
                if (!currentFolder) {
                    cout << "Source path not found!!\n";
                    return false;
                }
            }
        }

        string* sourceFile = currentFolder->findFile(fileName);
        if (!sourceFile) {
            cout << "Source file not found!!\n";
            return false;
        }

        string sourceFolderName = currentFolder->getName();

        string prefixedFileName = fileName;
        if (fileName.find(sourceFolderName + "_") != 0) {
            prefixedFileName = sourceFolderName + "_" + fileName;

            currentFolder->renameFile(fileName, prefixedFileName);
            cout << "Renamed " << fileName << " to " << prefixedFileName << " in source folder.\n";
        }

        Folder* destFolder = root;
        tempPath = destPath;

        while (!tempPath.empty()) {
            int pos = tempPath.find("\\");
            string folderName;
            if (pos == -1) {
                folderName = tempPath;
            }
            else {
                folderName = tempPath.substr(0, pos);
            }

            if (pos == -1) {
                tempPath = "";
            }
            else {
                tempPath = tempPath.substr(pos + 1);
            }

            destFolder = destFolder->findFolder(folderName);
            if (!destFolder) {
                cout << "Destination path not found!!\n";
                return false;
            }
        }

        bool fileFlag = destFolder->addVFile_(prefixedFileName, *sourceFile);
        if (!fileFlag) {
            cout << "Error: Unable to copy file.\n";
            return false;
        }

        cout << "File successfully copied to the destination as " << prefixedFileName << "\n";
        return true;
    }
    Folder* findFolderByPath(const string& path) {
        Folder* currentFolder = getCurrentFolder();
        string tempPath = path;

        while (!tempPath.empty()) {
            size_t pos = tempPath.find("\\");
            string folderName = (pos == string::npos) ? tempPath : tempPath.substr(0, pos);

            tempPath = (pos == string::npos) ? "" : tempPath.substr(pos + 1);

            if (folderName.empty()) {
                continue; // Skip empty folder names (e.g., extra slashes)
            }

            currentFolder = currentFolder->findFolder(folderName);
            if (!currentFolder) {
                return nullptr; // Folder not found
            }
        }

        return currentFolder;
    }
    string extractFileName(string filePath) {

        string tempPath = filePath;
        string fileName;

        while (!tempPath.empty()) {
            int pos = tempPath.find("\\");
            string folderName;

            if (pos == -1) {
                folderName = tempPath; // Last segment, should be the file name
            }
            else {
                folderName = tempPath.substr(0, pos);
            }

            if (pos == -1) {
                tempPath = "";
            }
            else {
                tempPath = tempPath.substr(pos + 1);
            }

            if (tempPath.empty()) {
                fileName = folderName; // The last part of the path is the file name
            }
        }
        return fileName;
    }

    void searchFile(const string& filePath) {
        Folder* currentFolder = root; // Start at the root folder
        string tempPath = filePath;
        string fileName;

        // Traverse the path to find the folder and file
        while (!tempPath.empty()) {
            int pos = tempPath.find("\\");
            string folderName;

            if (pos == -1) {
                folderName = tempPath; // Last segment, should be the file name
            }
            else {
                folderName = tempPath.substr(0, pos);
            }

            if (pos == -1) {
                tempPath = "";
            }
            else {
                tempPath = tempPath.substr(pos + 1);
            }

            if (tempPath.empty()) {
                fileName = folderName; // The last part of the path is the file name
            }
            else {
                currentFolder = currentFolder->findFolder(folderName);
                if (!currentFolder) {
                    cout << "Error: Folder '" << folderName << "' not found in the specified path.\n";
                    return;
                }
            }
        }

        // Check if the file exists in the located folder
        if (currentFolder->fileExists(fileName)) {
            string fullPath = currentFolder->getPath() + "\\" + fileName + ".txt";
            cout << "File '" << fileName << "' found at path: " << fullPath << "\n";
        }
        else {
            cout << "File '" << fileName << "' not found in the specified path.\n";
        }
    }

    bool searchFileBOOL(const string& filePath) {
        Folder* currentFolder = root; // Start at the root folder
        string tempPath = filePath;
        string fileName;

        // Traverse the path to find the folder and file
        while (!tempPath.empty()) {
            int pos = tempPath.find("\\");
            string folderName;

            if (pos == -1) {
                folderName = tempPath; // Last segment, should be the file name
            }
            else {
                folderName = tempPath.substr(0, pos);
            }

            if (pos == -1) {
                tempPath = "";
            }
            else {
                tempPath = tempPath.substr(pos + 1);
            }

            if (tempPath.empty()) {
                fileName = folderName; // The last part of the path is the file name
            }
            else {
                currentFolder = currentFolder->findFolder(folderName);
                if (!currentFolder) {
                    cout << "Error: Folder '" << folderName << "' not found in the specified path.\n";
                    return false;
                }
            }
        }

        // Check if the file exists in the located folder
        if (currentFolder->fileExists(fileName)) {
            string fullPath = currentFolder->getPath() + "\\" + fileName + ".txt";
            cout << "File '" << fileName << "' found at path: " << fullPath << "\n";
            return true;
        }
        else {
            cout << "File '" << fileName << "' not found in the specified path.\n";
            return false;
        }
    }


    bool fileExists(const  string& filename) {
         queue< string> tempQueue = fileTrackerQueue;
        while (!tempQueue.empty()) {
            if (tempQueue.front() == filename) {
                return true;
            }
            tempQueue.pop();
        }
        return false;
    }
    bool loadFileToList(const  string& filename) {
        for (auto& name : fileNames) {
            if (name == filename) {
                name = filename;
                return true; 
            }
        }
        fileNames.push_back(filename);
        return true;
    }
    bool moveFile(const string& sourcePath, const string& destPath) {
        Folder* currentFolder = root;
        string tempPath = sourcePath;
        string fileName;

        while (!tempPath.empty()) {
            int pos = tempPath.find("\\");
            string folderName;
            if (pos == -1) {
                folderName = tempPath;
            }
            else {
                folderName = tempPath.substr(0, pos);
            }

            if (pos == -1) {
                tempPath = "";
            }
            else {
                tempPath = tempPath.substr(pos + 1);
            }

            if (tempPath.empty()) {
                fileName = folderName;
            }
            else {
                currentFolder = currentFolder->findFolder(folderName);
                if (!currentFolder) {
                    cout << "Source path not found!!\n";
                    return false;
                }
            }
        }

        string* sourceFile = currentFolder->findFile(fileName);
        if (!sourceFile) {
            cout << "Source file not found!!\n";
            return false;
        }

        string sourceFolderName = currentFolder->getName();

        string prefixedFileName = fileName;
        if (fileName.find(sourceFolderName + "_") != 0) {
            prefixedFileName = sourceFolderName + "_" + fileName;

            currentFolder->renameFile(fileName, prefixedFileName);
            cout << "Renamed " << fileName << " to " << prefixedFileName << " in source folder.\n";
        }

        Folder* destFolder = root;
        tempPath = destPath;

        while (!tempPath.empty()) {
            int pos = tempPath.find("\\");
            string folderName;
            if (pos == -1) {
                folderName = tempPath;
            }
            else {
                folderName = tempPath.substr(0, pos);
            }

            if (pos == -1) {
                tempPath = "";
            }
            else {
                tempPath = tempPath.substr(pos + 1);
            }

            destFolder = destFolder->findFolder(folderName);
            if (!destFolder) {
                cout << "Destination path not found!!\n";
                return false;
            }
        }

        bool fileFlag = destFolder->addVFile_(prefixedFileName, *sourceFile);
        if (!fileFlag) {
            cout << "Error: Unable to move file.\n";
            return false;
        }

        currentFolder->deleteFile(prefixedFileName);
        cout << "File successfully moved to the destination as " << prefixedFileName << "\n";
        return true;
    }
    bool loadTree(const string& fileName) {

        ifstream file(fileName);
        if (!file.is_open()) {
            cout << "Error: Could not open file '" << fileName << "'.\n";
            return false;
        }

        string line;

        while (getline(file, line)) {
            if (line.empty()) {
                continue;
            }

            if (line.front() != 'V' || line[1] != '\\') {
                cout << "Error: Invalid absolute path '" << line << "'.\n"; //first check the paths, if the paths is incorrect than no need to 
                                                                            //to execute return back
                continue;
            }

            vector<string> pathComponents;
            stringstream ss(line);
            string segment;

            while (getline(ss, segment, '\\')) {
                if (!segment.empty()) {
                    pathComponents.push_back(segment);
                }
            }

            if (pathComponents.empty() || pathComponents[0] != "V") {     //hard coding the "V" is the root directory for our tree structure
                cout << "Error: Path does not start with root 'V': " << line << "\n";
                continue;
            }

            Folder* currentFolder = root;

            for (int i = 1; i < pathComponents.size(); i++) {
                const string name = pathComponents[i];

                if (i == pathComponents.size() - 1 && name.find('.') != string::npos) { //mechanism for extracting the file from the path look for the 
                                                                                        //dot sign which indicates that the current word is a file
                    if (!currentFolder->addVFile(name)) {
                        cout << "Error: Could not add file '" << name << "' in folder '" << currentFolder->name << "'.\n";
                        return false;
                    }
                }
                else {
                    Folder* nextFolder = currentFolder->findChildFolder(name);
                    if (!nextFolder) {
                        nextFolder = currentFolder->addVFolder(name);
                    }
                    if (!nextFolder) {
                        cout << "Error: Could not create or retrieve folder '" << name << "' under '" << currentFolder->name << "'.\n";
                        return false;
                    }
                    currentFolder = nextFolder;
                }
            }
        }

        file.close();
        return true;
    }

};