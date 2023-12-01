/*
CLASS:      CE 223
PROJECT:    FINAL PROJECT - Chatbot
AUTHORS:    Nicholas Elliott, Noah Smith, Alex Olson, Lucien Verrone
DATE:       29 November 2023
*/

//Import Libraries
#include<iostream>
#include<fstream>
#include<sstream>
#include<list>
#include<vector>
#include<queue>
#include<stack>

using namespace std;

class Account {
private:
    int accNum, rtnNum, rewards;
    float checking, saving, debt;
    queue<pair<float,char>> history;
public:
    string name;
    
    Account(string name0, int accNum0, int rtnNum0, int rewards0, float checking0, float saving0, float debt0) {
        name = name0;
        accNum = accNum0;
        rtnNum = rtnNum0;
        rewards = rewards0;
        checking = checking0;
        saving = saving0;
        debt = debt0;
    }

    int pullAccNum() {
        return accNum;
    }
    int pullRtnNum() {
        return rtnNum;
    }
    int pullRewards() {
        return rewards;
    }
    float checkingBlnc() {
        return checking;
    }
    float savingBlnc() {
        return saving;
    }
    float debtBlnc() {
        return debt;
    }
    queue<pair<float,char>> pullHistory() {
        return history;
    }
    void addBalance(float amnt, bool checking0) {
        if (checking0 == true) {
            checking = checking + amnt;
            rewards = rewards + int(amnt / 100);
            history.push(make_pair(amnt, 'C'));
        }
        else {
            saving = saving + amnt;
            rewards = rewards + int(amnt / 100);
            history.push(make_pair(amnt, 'S'));
        }
    }

    void removeBalance(float amnt, bool checking0) {
        if (checking0 == true) {
            checking = checking - amnt;
            history.push(make_pair(-amnt, 'C'));
        }
        else {
            saving = saving + amnt;
            history.push(make_pair(-amnt, 'S'));
        }
    }

    void moveBalance(float amnt, bool toSave) {
        if (toSave == true) {
            if (checking >= amnt) {
                checking = checking - amnt;
                saving = saving + amnt;
                history.push(make_pair(-amnt, 'C'));
                history.push(make_pair(amnt, 'S'));
            }
            else {
                cout << "Error, insufficient funds" << endl;
            }
        }
        else {
            if (saving >= amnt) {
                checking = checking + amnt;
                saving = saving - amnt;
                history.push(make_pair(amnt, 'C'));
                history.push(make_pair(-amnt, 'S'));
            }
            else {
                cout << "Error, insufficient funds" << endl;
            }
        }
    }

    void payDebt(float amnt) {
        if (checking >= amnt) {
            checking = checking - amnt;
            debt = debt - amnt;
            history.push(make_pair(-amnt, 'C'));
        }
        else {
            cout << "Error, insufficient funds" << endl;
        }
    }

};

class Graph {
public:
    int size;
    vector<string> locations;
    list<pair<int, int>>* adjlist;

    //Graph Stuff
    Graph(int s) {
        adjlist = new list<pair<int, int>>[s];
        locations.resize(s);
        size = s;
    }
    void addEdge(string l1, string l2, int w) {
        adjlist[placeToVal(l1)].push_back(make_pair(placeToVal(l2), w));
        adjlist[placeToVal(l2)].push_back(make_pair(placeToVal(l1), w));
    }
    void removeEdge(string l1, string l2) {
        for (int j = 0; j < maxW(); j++) {
            adjlist[placeToVal(l1)].remove(make_pair(placeToVal(l2), j));
            adjlist[placeToVal(l2)].remove(make_pair(placeToVal(l1), j));
        }
    }
    void modEdge(string u, string v, int w) {
        removeEdge(u, v);
        addEdge(u, v, w);
    }
    void addVector(string nL) {
        size++;
        locations.resize(size);
        addLocation(nL);
        list<pair<int, int>>* temp;
        temp = adjlist;
        adjlist = new list<pair<int, int>>[size];
        for (int j = 0; j < size - 1; j++) {
            adjlist[j].swap(temp[j]);
        }
        temp = nullptr;
        delete temp;
    }
    void removeVector(string nL) {

        /*for (int i = 0; i < size; i++) {
            removeEdge(nL, valToPlace(i));
        }*/

        list<pair<int, int>>* temp;
        temp = new list<pair<int, int>>[size - 1];
        for (int k = 0; k != placeToVal(nL); k++) {
            temp[k] = adjlist[k];
        }
        for (int j = placeToVal(nL); j < size - 1; j++) {
            temp[j] = adjlist[j + 1];
        }
        adjlist = new list<pair<int, int>>[size - 1];
        adjlist = temp;
        removeLocation(nL);
    }
    int maxW() {
        int mW = 0;
        for (int i = 0; i < size; i++) {
            for (auto j : adjlist[i]) {
                if (j.second > mW) {
                    mW = j.second;
                }
            }
        }
        return mW;
    }
    void printGraph() {
        for (int q = 0; q < size; q++) {
            cout << valToPlace(q) << endl;
            for (auto it : adjlist[q]) {
                cout << "\t" << it.first << endl;
                cout << "\tCONNECTING WORD: " << valToPlace(it.first) << "\t\tWeight:" << it.second << endl;
            }
            cout << endl;
        }
    }
    void addLocation(string nL) {
        for (int i = 0; i < size; i++) {
            if (locations.at(i) == "") {
                locations.at(i) = nL;
                break;
            }
        }
    }
    void removeLocation(string nL) {
        vector<string> tempLoc;
        tempLoc.resize(size);
        for (int i = 0; i < placeToVal(nL); i++) {
            tempLoc.at(i) = (locations.at(i));
        }
        for (int u = placeToVal(nL); u < size - 1; u++) {
            tempLoc.at(u) = (locations.at(u + 1));
        }
        locations = tempLoc;
        size -= 1;
    }
    int placeToVal(string p) {
        for (int i = 0; i < size; i++) {
            if ((locations.at(i) == (p))) {
                return i;
            }
        }return -1;
    }
    string valToPlace(int v) {
        if (v > locations.size() || v < 0) {
            return "Error: Invalid input";
        }
        return locations.at(v);
    }

    //Search Algorithms
    string promptSearch(string prompt) {
        stringstream ss(prompt);
        vector<string> promptParts;
        string word;
        while (ss >> word) {
            promptParts.push_back(word);
        }

        string codeResponse;
        for (int i = 0; i < promptParts.size(); i++) {
            if (DFS(placeToVal("BALANCE"), promptParts.at(i))) {
                codeResponse = callFunc(1001);
                break;
            }
            else if (DFS(placeToVal("INFO"), promptParts.at(i))) {
                codeResponse = callFunc(1002);
                break;
            }
            else if (DFS(placeToVal("DEPOSIT"), promptParts.at(i))) {
                codeResponse = callFunc(1003);
                break;
            }
            else if (DFS(placeToVal("WITHDRAW"), promptParts.at(i))) {
                codeResponse = callFunc(1004);
                break;
            }
            else if (DFS(placeToVal("TRANSFER"), promptParts.at(i))) {
                codeResponse = callFunc(1005);
                break;
            }
            else if (DFS(placeToVal("PAY"), promptParts.at(i))) {
                codeResponse = callFunc(1006);
                break;
            }
            else {
                codeResponse = callFunc(404);
                continue;
            }
        }
        return codeResponse;
    }
    string callFunc(int code) {
        if (code == 1001) {
            return "BALANCE";
        }
        else if (code == 1002) {
            return "INFO";
        }
        else if (code == 1003) {
            return "DEPOSIT";
        }
        else if (code == 1004) {
            return "WITHDRAW";
        }
        else if (code == 1005) {
            return "TRANSFER";
        }
        else if (code == 1006) {
            return "PAY";
        }
        else {
            return "ERROR";
        }
    }
    int BFS(int startVal, string searchVal) {
        vector<bool> visited; //Visited Array
        visited.resize(size);
        queue<int> myQ; //BFS Queue

        //Set visited states to false
        for (int i = 0; i < size; i++) {
            visited[i] = false;
        }

        //Initialize values
        visited[startVal] = true;
        myQ.push(startVal);
        int nodeVal = startVal;

        //BFS Algorithm
        while (!myQ.empty()) {
            nodeVal = myQ.front();
            myQ.pop();

            for (auto it : adjlist[nodeVal]) {
                if (!visited[it.first]) {
                    myQ.push(it.first);
                    visited[it.first] = true;
                    if (searchVal == valToPlace(it.first)) {
                        return it.first;
                    }
                }
            }
        }
        return 0;
    }
    bool DFS(int startVal, string searchVal) {
        vector<bool> visited; //Visited Array
        visited.resize(size);
        stack<int> myStack; //DFS Stack

        //Set visited states to false
        for (int i = 0; i < size; i++) {
            visited[i] = false;
        }

        //Initialize values
        visited[startVal] = true;
        myStack.push(startVal);
        int nodeVal = startVal;

        while (!myStack.empty()) {
            nodeVal = myStack.top();

            myStack.pop();

            for (auto it : adjlist[nodeVal]) {
                if (!visited[it.first]) {
                    myStack.push(it.first);
                    visited[it.first] = true;
                    if (searchVal == valToPlace(it.first)) {
                        return true;
                    }
                }
            }
        }
        return false;
    }
};

//Chat Model to Help Understand prompts/questions
void createChatModel(Graph& model) {
    vector<string> nodeVector;
    vector<string> wordVector;
    vector<string> visitComboVector;
    ifstream trainingFile("C:/Users/lucie/source/repos/CHATBOT1/TrainingData.txt");
    if (trainingFile.is_open()) {
        string line;
        while (getline(trainingFile, line)) {
            istringstream iss(line);
            string word, catWord;
            while (iss >> word) {
                if (word == "BALANCE" || word == "INFO" || word == "DEPOSIT" || word == "WITHDRAW" || word == "TRANSFER" || word == "PAY") {
                    catWord = word;
                    model.addLocation(catWord);
                }
                else {
                    model.addLocation(word);
                    model.addEdge(catWord, word, 1);
                }
            }
        }
        trainingFile.close();
    }
    else {
        cout << "Error opening file" << endl;
    }
}

int main() {
    Graph myModel(60);
    Account account("Account1", 238465246588, 513684958, 100, 189.34, 1034.33, 9090);

    createChatModel(myModel);
    myModel.printGraph();
    string prompt;
    while (prompt != "q") {
        cout << "Ask me a questions about your account?: " << endl;
        getline(cin, prompt);
        string response = myModel.promptSearch(prompt);
        char yN;
        float amount = 0;
        cout << myModel.promptSearch(prompt) << endl;
        
        
        if (response == "BALANCE") {
            cout << "Account Name: " << account.name << endl;
            cout << "Checking Balance: $" << account.checkingBlnc() << endl;
            cout << "Savings Balance: $" << account.savingBlnc() << endl;
            cout << "Debt: $" << account.debtBlnc() << endl;
            cout << "Rewards Points: " << account.pullRewards() << endl;
        }
        else if (response == "INFO") {
            cout << "Account Name: " << account.name << endl;
            cout << "Account Number: $" << account.pullAccNum() << endl;
            cout << "Routing Number: $" << account.pullRtnNum() << endl;
            cout << "Debt: $" << account.debtBlnc() << endl;
            cout << "Rewards Points: " << account.pullRewards() << endl;
        }
        else if (response == "DEPOSIT") {
            cout << "How much would you like to deposit?" << endl;
            cin >> amount;
            cout << "Would you like to deposit into your checking account? (Y/N)" << endl;
            cin >> yN;

            if (yN == 'Y' || yN == 'y') {
                account.addBalance(amount, true);
            }
            else if (yN == 'N' || yN == 'n') {
                account.addBalance(amount, false);
            }
            else {
                cout << "ERROR" << endl;
            }
        }
        else if (response == "WITHDRAW") {
            cout << "How much would you like to withdraw?" << endl;
            cin >> amount;
            cout << "Would you like to remove from your checking account? (Y/N)" << endl;
            cin >> yN;

            if (yN == 'Y' || yN == 'y') {
                account.removeBalance(amount, true);
            }
            else if (yN == 'N' || yN == 'n') {
                account.removeBalance(amount, false);
            }
            else {
                cout << "ERROR" << endl;
            }
        }
        else if (response == "TRANSFER") {
            cout << "How much would you like to transfer?" << endl;
            cin >> amount;
            cout << "Would you like to move funds to savings? (Y/N)" << endl;
            cin >> yN;

            if (yN == 'Y' || yN == 'y') {
                account.moveBalance(amount, true);
            }
            else if (yN == 'N' || yN == 'n') {
                account.removeBalance(amount, false);
            }
            else {
                cout << "ERROR" << endl;
            }
        }
        else if (response == "PAY") {
            cout << "How much would you like to pay?" << endl;
            cin >> amount;
            account.payDebt(amount);
        }
        
    }

    return 0;
}