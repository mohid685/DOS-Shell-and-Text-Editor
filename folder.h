#pragma once
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <ctime>
#include <map>

using namespace std;

class Folder {
public:
     string name;                      
     string path;                      
     Folder* parent;                         
     vector<Folder*> subFolders;        
     vector< string> files;         
     map<string,vector< string>> fileContents;
     string getCurrentTime() const {
        time_t now = time(0);
        char buffer[26];
        ctime_s(buffer, sizeof(buffer), &now);
        return  string(buffer);
    }

public:
    Folder( string n, Folder* p = NULL)
        : name(n), parent(p) {
        if (parent) {
            path = parent->getPath() + parent->getName() + "\\";
        }
        else {
            path = "V:\\";
        }
    }
    ~Folder() {
        for (auto *subfolder : subFolders) {
            delete subfolder;
        }
        subFolders.clear();
    }
    void saveToFile( ostream& os) const {
        os << "Folder: " << name << "\n";
        for (const auto& file : files) {
            os << "File: " << file << "\n";
            for (const auto& line : fileContents.at(file)) {
                os << line << "\n";
            }
            os << "---END OF CONTENT---\n";
        }
        for (const auto& subFolder : subFolders) {
            subFolder->saveToFile(os); 
        }
    }
    void addFile(const  string& fileName) {
        if (!fileExists(fileName)) {
            files.push_back(fileName);
            fileContents[fileName] = {};  
        }
        else {
             cout << "File already exists.\n";
        }
    }
     string getName()  { 
         return name; 
     }
     string getPath()  { 
         return path; 
     }
    Folder* getParent()  { 
        return parent; 
    }
    bool folderExists(const  string& folderName) {
        for (int i = 0; i < subFolders.size(); ++i) {
            if (subFolders[i]->getName() == folderName) {
                return true;
            }
        }
        return false;
    }
    Folder* findSubFolder(const  string& folderName) {
        for (int i = 0; i < subFolders.size(); ++i) {
            if (subFolders[i]->getName() == folderName) {
                return subFolders[i];
            }
        }
        return NULL;
    }
    void addSubFolder(Folder* folder) {
        subFolders.push_back(folder);
    }
    bool fileExists(const  string& fileName) {
        for (int i = 0; i < files.size(); ++i) {
            if (files[i] == fileName) {
                return true;
            }
        }
        return false;
    }
    void convertFileExtensions(const string& oldExt, const string& newExt) {
        bool converted = false;
        for (int i = 0; i < files.size(); ++i) {
            if (files[i].size() >= oldExt.size() &&
                files[i].substr(files[i].size() - oldExt.size()) == oldExt) {
                files[i] = files[i].substr(0, files[i].size() - oldExt.size()) + newExt;
                cout << "Converted file: " << files[i] << "\n";
                converted = true;
            }
        }
        if (!converted) {
            cout << "No files with the extension '" << oldExt << "' found in the current directory.\n";
        }
    }
    const  vector< string>& getFiles() const { 
        return files; 
    }
    const  vector<Folder*>& getSubFolders() const {     
        return subFolders; 
    }
    void setFileContent(const  string& fileName, const  string& content) {
        if (fileExists(fileName)) {
            fileContents[fileName].push_back(content);
        }
        else {
             cout << "File not found.\n";
        }
    }
    const  vector< string>& getFileContent(const  string& fileName) const {
        return fileContents.at(fileName);
    }
    void deleteFile(const  string& fileName) {
        auto it =  find(files.begin(), files.end(), fileName);
        if (it != files.end()) {
            files.erase(it);
            fileContents.erase(fileName); 
             cout << "File '" << fileName << "' deleted successfully.\n";
        }
        else {
             cout << "File '" << fileName << "' not found.\n";
        }
    }
    void displayContents() const {
        if (files.empty() && subFolders.empty()) {
            cout << "The directory is empty.\n";
        }
        else {
            if (!files.empty()) {
                cout << "<FILES>\n";
                for (const auto& file : files) {
                    cout << "  " << file << "\n";
                }
            }
            if (!subFolders.empty()) {
                cout << "<DIR>\n";
                for (const auto& subFolder : subFolders) {
                    cout << "  " << subFolder->getName() << "\n";
                }
            }
        }
    }
    bool createSubFolder(const string& folderName) {
        for (const auto& subFolder : subFolders) {
            if (subFolder->getName() == folderName) {
                cout << "A subdirectory with the name \"" << folderName << "\" already exists.\n";
                return false;
            }
        }

        Folder* newFolder = new Folder(folderName, this);
        subFolders.push_back(newFolder);
        cout << "Directory \"" << folderName << "\" created successfully.\n";
        return true;
    }
    void printFileAttributes(const string& fileName) {
        if (fileExists(fileName)) {
            ifstream file(fileName);
            if (!file) {
                cout << "Error: Unable to open file '" << fileName << "' for reading.\n";
                return;
            }

            cout << "File: " << fileName << "\n";
            cout << "Path: " << path << fileName << "\n";
            cout << "Creation Time: " << getCurrentTime() << "\n";

            string owner = "NissGT35";
            cout << "Owner: " << owner << "\n";

            string line;
            int totalWords = 0;
            int lineCount = 0;

            while (getline(file, line)) {
                istringstream stream(line);
                string word;
                while (stream >> word) {
                    totalWords++;
                }
                lineCount++;
            }

            file.close();

            double avgWordCount;
            if (lineCount > 0) {
                avgWordCount = (double)(totalWords) / lineCount;
            }
            else {
                avgWordCount = 0.0;
            }

            int fileSizeKB = lineCount;

            cout << "File Size: " << fileSizeKB << " KB\n";
            cout << "Average Words Per Line: " << avgWordCount << "\n";
        }
        else {
            cout << "File '" << fileName << "' not found.\n";
        }
    }
    void formatFolder() {
        files.clear();
        fileContents.clear();

        for (auto* subFolder : subFolders) {
            subFolder->formatFolder(); 
            delete subFolder;
        }
        subFolders.clear();

        cout << "The folder '" << name << "' and all its contents have been cleared.\n";
    }
    void printTree(int indent = 0) const {
        for (int i = 0; i < indent; i++) {
            cout << " ";
        }
        cout << "<" << name << ">" << "\n";

        if (!files.empty()) {
            for (int i = 0; i < indent + 2; i++) { 
                cout << " "; 
            }
             cout << "|--- <FILES>\n";
            for (const auto& file : files) {
                for (int i = 0; i < indent + 4; i++) {
                    cout << " ";
                }
                 cout << file << "\n";
            }
        }

        if (!subFolders.empty()) {
            for (int i = 0; i < indent + 2; i++) {
                cout << " ";
            }
             cout << "|--- <DIRS>\n";
            for (const auto& subFolder : subFolders) {
                subFolder->printTree(indent + 4);
            }
        }
    }
    void printWorkingDirectory(int indent = 0) const {
        if (parent) {
            parent->printWorkingDirectory(indent + 2);
        }
        for (int i = 0; i < indent; i++) {
            cout << " ";
        }
        cout << "<" << name << ">" << "\n";
    }
    void renameFile(const string& currName, const string& newName) {

        auto it = find(files.begin(), files.end(), currName);

        if (it == files.end()) {
            cout << "File '" << currName << " was not found!!\n";
            return;
        }

        if (fileExists(newName)) {
            cout << "A file with " << newName << " already exists!!\n";
            return;
        }

        *it = newName;

        fileContents[newName] = move(fileContents[currName]);
        fileContents.erase(currName);

        cout << "File '" << currName << " has been renamed to " << newName << " successfully.\n";
    }
    bool removeSubfoler(const string& folderName) {

        for (auto it = subFolders.begin(); it != subFolders.end(); it++) {

            if ((*it)->getName() == folderName) {

                delete *it;
                subFolders.erase(it);
                cout << "Directory '" << folderName << " has been removed!!\n";
                return true;
            }
        }
        cout << "Directory '" << folderName << " not found :/\n";
        return false;
    }
    Folder* findFolder(const string& folderName) {

        for (Folder* f : subFolders) {

            if (f->name == folderName) {
                return f;
            }
        }
        return NULL;
    }
    string* findFile(const string& fileName) {

        for (string& file : files) {

            if (file == fileName) {
                return &file;
            }
        }
        return NULL;
    }
    Folder* addVFolder(const string& folderName) {
        if (findFolder(folderName)) {
            cout << "Folder '" << folderName << "' already exists.\n";
            return nullptr;
        }

        Folder* newFolder = new Folder(folderName, this);
        subFolders.push_back(newFolder);
        return newFolder;
    }
    bool addVFile(const string& fileName) {
        if (findFile(fileName)) {
            cout << "File '" << fileName << "' already exists.\n";
            return false;
        }

        ifstream inFile(fileName);
        bool fileHasContent = false;
        string line;

        if (inFile) {
            while (getline(inFile, line)) {
                fileContents[fileName].push_back(line); 
                fileHasContent = true;
            }
            inFile.close();
        }

        if (!fileHasContent) {
            fileContents[fileName] = {}; 
            ofstream outFile(fileName);

            if (!outFile) {
                cout << "Error: Unable to create file on disk.\n";
                return false;
            }

            string defaultLine = "This Is A Blank File....";
            fileContents[fileName].push_back(defaultLine); 
            outFile << defaultLine << "\n"; 

            outFile.close();
        }

        files.push_back(fileName);
        return true;
    }
    bool addVFile_(const string& fileName, const string& sourceContent) {
        if (findFile(fileName)) {
            cout << "File '" << fileName << "' already exists.\n";
            return false;
        }

        // Save the file content
        fileContents[fileName] = { sourceContent };

        ofstream outFile(fileName);
        if (!outFile) {
            cout << "Error: Unable to create file on disk.\n";
            return false;
        }

        for (const string& line : fileContents[fileName]) {
            outFile << line << "\n";
        }

        outFile.close();
        files.push_back(fileName);
        return true;
    }
    Folder* findChildFolder(const string& name) {
        for (Folder* child : subFolders) { 
            if (child->name == name) {
                return child;
            }
        }
        return nullptr;
    }
};
