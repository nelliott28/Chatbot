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
    string accountName;
    long long int accountNum, routingNum;
    double myChecking, mySaving, myRewards;
    queue<string> myHistory;
public:
    
    Account(string accountName, long long int accountNum, long long int routingNum, double myRewards, double myChecking, float mySaving) {
        this->accountName = accountName;
        this->accountNum = accountNum;
        this->routingNum = routingNum;
        this->myRewards = myRewards;
        this->myChecking = myChecking;
        this->mySaving = mySaving;
    }

    //Request Amount to Perform Action On
    double requestAmount(string msg){
        double input;
        char c = 'Y';
        while (true) {
            try { // Makes sure the input is a postive intger. Otherwise programe goes into infinte loop.
                cout << "What amount would you like to " << msg << "?" << endl << "$";
                if (!(cin >> input)) { // Catches non intger inputs
                    cin.clear();  // Clear the error flag
                    cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');  // Clear the input buffer
                    throw invalid_argument("Invalid input. Please enter a positive integer.");
                }
                else {
                    break;
                }
            }catch (exception& e) { // Catchs thrown errors and handels them
                cout << "Error:" << e.what() << endl;
                cout << " Do you wish to continue? Y/N" << endl;
                cin >> c;
                if (!(c == 'Y' || c == 'y')) {
                    break;
                }
            }
        }return input;
    }

    // --- ACCOUNT FUNCTIONS --- //

    //Displays Balances
    void displayBalance(char accountType){
        cout << "----- BALANCE -----" << endl;
        if (accountType == 'C'){
            cout << "Checking: $ " << myChecking << endl;
        }
        else if (accountType == 'S'){
            cout << "Savings: $ " << mySaving << endl;
        }
        else{
            cout << "Checking: $ " << myChecking << endl;
            cout << "Savings: $ " << mySaving << endl;
        }
        cout << "-------------------" << endl;
    }
    //Display Account Info
    void displayAccountInfo(){
        cout << "----- ACCOUNT INFO -----" << endl;
        cout << accountName << " : " << routingNum << " : " << accountNum << endl << endl;
        cout << "Checking: $ " << myChecking << endl;
        cout << "Savings: $ " << mySaving << endl << endl;
        cout << "My Rewards: $" << myRewards << endl;
        cout << "<<<------------>>>\t\t" << endl;
        cout << "HISTORY:" << endl;
        auto tempHist = myHistory;
        while(!tempHist.empty()){
            cout << tempHist.front() << endl;
            tempHist.pop();
        }
        cout << "------------------------" << endl;
    }
    //Deposit
    void deposit(double amnt, char accountType){
        cout << "----- DEPOSIT SLIP -----" << endl;
        if (accountType == 'C'){
            cout << "CHECKING: +$" << amnt << endl;
            cout << "NEW TOTAL: $" << myChecking + amnt << endl;
            myChecking += amnt;
            myHistory.push("CHECKING: +$" + to_string(amnt));
        }
        else if (accountType == 'S'){
            cout << "SAVINGS: +$" << amnt << endl;
            cout << "NEW TOTAL: $" << mySaving + amnt << endl;
            mySaving += amnt;
            myHistory.push("SAVINGS: +$" + to_string(amnt));
        }
        else{
            cout << "DEFAULTING TO CHECKING..." << endl << endl;
            cout << "CHECKING: +$" << amnt << endl;
            cout << "NEW TOTAL: $" << myChecking + amnt << endl;
            myChecking += amnt;
            myHistory.push("CHECKING: +$" + to_string(amnt));
        }
        cout << "------------------------" << endl;
    }
    //Withdraw
    void withdraw(double amnt){
        cout << "----- WITHDRAWAL SLIP -----" << endl;
        if (myChecking - amnt >= 0){
            //Withdraw from checking
            cout << "CHECKING: -$" << amnt << endl;
            cout << "NEW TOTAL: $" << myChecking - amnt << endl;
            myChecking -= amnt;
            myHistory.push("CHECKING: -$" + to_string(amnt));
        }
        else if ((myChecking + mySaving) - amnt >= 0){
            //Withdraw with Overdraft protection
            amnt -= myChecking;
            cout << "CHECKING: -$" << myChecking << endl;
            myHistory.push("CHECKING: -$" + to_string(myChecking));
            myChecking -= myChecking;
            mySaving -= amnt;
            cout << "OVERDRAFT: $" << amnt << endl;
            cout << "NEW CHECKING TOTAL: $" << myChecking << endl;
            cout << "NEW SAVINGS TOTAL: $" << mySaving << endl;
            myHistory.push("OVERDRAFT: $" + to_string(amnt));
            myHistory.push("SAVINGS: -$" + to_string(amnt));
        }
        else{
            //ERROR - NOT SUFFICIENT FUNDS
            cout << "!!! ERROR - INSUFFICIENT FUNDS !!!" << endl;
        }
        cout << "---------------------------" << endl;
    }
    //Pay
    void pay(double amnt){
        cout << "----- PAY -----" << endl;
        if (myChecking - amnt >= 0){
            //Pay from checking
            cout << "CHECKING: -$" << amnt << endl;
            cout << "NEW TOTAL: $" << myChecking - amnt << endl;
            myChecking -= amnt;
            myHistory.push("CHECKING: -$" + to_string(amnt));
        }
        else if ((myChecking + mySaving) - amnt >= 0){
            //Pay with Overdraft protection
            amnt -= myChecking;
            cout << "CHECKING: -$" << myChecking << endl;
            myHistory.push("CHECKING: -$" + to_string(myChecking));
            myChecking -= myChecking;
            mySaving -= amnt;
            cout << "OVERDRAFT: $" << amnt << endl;
            cout << "NEW CHECKING TOTAL: $" << myChecking << endl;
            cout << "NEW SAVINGS TOTAL: $" << mySaving << endl;
            myHistory.push("OVERDRAFT: $" + to_string(amnt));
            myHistory.push("SAVINGS: -$" + to_string(amnt));
        }
        else{
            //ERROR - NOT SUFFICIENT FUNDS
            cout << "!!! ERROR - INSUFFICIENT FUNDS !!!" << endl;
        }
        cout << "---------------" << endl;
    }
    //Transfer
    void transfer(double amnt, char from){
        cout << "----- TRANSFER -----" << endl;
        if (from == 'C' && myChecking - amnt >= 0){
            cout << "TRANSFERRING: $" << amnt << " from checking to savings account" << endl;          
            cout << "NEW CHECKING TOTAL: $" << myChecking - amnt << endl;
            cout << "NEW SAVINGS TOTAL: $" << mySaving + amnt << endl;
            myChecking -= amnt;
            mySaving += amnt;
            myHistory.push("CHECKING: -$" + to_string(amnt));
            myHistory.push("SAVINGS: +$" + to_string(amnt));
        }
        else if (from == 'S' && mySaving - amnt >= 0){
            cout << "TRANSFERRING: $" << amnt << "from savings to checking account" << endl;          
            cout << "NEW SAVINGS TOTAL: $" << mySaving - amnt << endl;
            cout << "NEW CHECKING TOTAL: $" << myChecking + amnt << endl;
            mySaving -= amnt;
            myChecking += amnt;
            myHistory.push("SAVINGS: -$" + to_string(amnt));
            myHistory.push("CHECKING: +$" + to_string(amnt));
        }
        else{
            //ERROR - INSUFFICIENT FUNDS
            cout << "!!! ERROR - INSUFFICIENT FUNDS !!!" << endl;
        }
        cout << "--------------------" << endl;
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

    //Prompt Search Algorithms AND ChatBot Responses
    void promptSearch(const string prompt, Account &account) {
        stringstream ss(prompt);
        vector<string> promptParts;
        string word;
        while (ss >> word) {
            promptParts.push_back(word);
        }

        string codeResponse;
        for (int i = 0; i < promptParts.size(); i++) {
            // --- BALANCE --- //
            if (DFS(placeToVal("BALANCE"), promptParts.at(i))) {
                for (int index = 0; index < promptParts.size(); index++) {
                    if (DFS(placeToVal("DISPLAY"), promptParts.at(index))){
                        //DISPLAY BALANCE ONLY
                        for (int minorIndex = 0; minorIndex < promptParts.size(); minorIndex++) {
                             if (DFS(placeToVal("SAVINGS"), promptParts.at(minorIndex))){
                                //DISPLAY SAVINGS ONLY
                                account.displayBalance('S');
                                return;
                             }
                             else if (DFS(placeToVal("CHECKINGS"), promptParts.at(minorIndex))){
                                //DISPLAY CHECKING ONLY
                                account.displayBalance('C');
                                return;
                             }
                             else{
                                //CONTINUE
                                continue;
                             }
                        }
                    }
                }
                //DEFAULT TO DISPLAYING BOTH BALANCES
                account.displayBalance('!');
                return;
            }
            // --- ACCOUNT INFO --- //
            else if (DFS(placeToVal("INFO"), promptParts.at(i))) {
                for (int index = 0; index < promptParts.size(); index++) {
                    if (DFS(placeToVal("DISPLAY"), promptParts.at(index))){
                        //DISPLAY ACCOUNT INFO ONLY
                        account.displayAccountInfo();
                        break;
                    }
                    else{
                        //ASK IF YOU WANT TO SEE YOUR ACCOUNT INFO
                        account.displayAccountInfo();
                        break;
                    }
                }
                return;
            }
            // --- DEPOSIT --- //
            else if (DFS(placeToVal("DEPOSIT"), promptParts.at(i))) {
                for (int index = 0; index < promptParts.size(); index++) {
                    if (DFS(placeToVal("SAVINGS"), promptParts.at(index))){
                        //DEPOSIT TO SAVINGS
                        account.deposit(account.requestAmount(promptParts.at(i)),'S');
                        return;
                    }
                    else if (DFS(placeToVal("CHECKINGS"), promptParts.at(index))){
                        //DEPOSIT TO CHECKING
                        account.deposit(account.requestAmount(promptParts.at(i)), 'C');
                        return;
                    }
                    else{
                        continue;
                    }
                }
                //DEFAULT TO ACCOUNT
                account.deposit(account.requestAmount(promptParts.at(i)), '!');
                return;
            }
            // --- WITHDRAW --- //
            else if (DFS(placeToVal("WITHDRAW"), promptParts.at(i))) {
                account.withdraw(account.requestAmount(promptParts.at(i)));
                return;
            }
            // --- TRANSFER --- //
            else if (DFS(placeToVal("TRANSFER"), promptParts.at(i))) {
                cout << "FROM what account? (SAVINGS - 'S' OR CHECKING - 'C'):  " << endl;
                char input;
                while (input != 'S' && input != 'C'){
                    cin >> input; 
                    if (input != 'S' && input != 'C'){
                        cout << "Please try again: " << endl;
                    }
                }

                if (input == 'S'){
                    //TRANSFER FROM SAVINGS TO CHECKING
                    account.transfer(account.requestAmount(promptParts.at(i)), 'S');
                }
                else {
                    //TRANSFER FROM CHECKING TO SAVING
                    account.transfer(account.requestAmount(promptParts.at(i)), 'C');
                }
                return;
            }
            // --- PAY --- //
            else if (DFS(placeToVal("PAY"), promptParts.at(i))) {
                account.pay(account.requestAmount(promptParts.at(i)));
                return;
            }
            else if (i == promptParts.size() - 1 && prompt != "q"){
                //ERROR - CHATBOT DOESN'T UNDERSTAND
                cout << "I'm sorry, I don't quite understand. Please try again..." << endl << endl;
                return;
            }
        }
    }
    
    //Search Algorithms
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
    ifstream trainingFile("TrainingData.txt");
    if (trainingFile.is_open()) {
        string line;
        while (getline(trainingFile, line)) {
            istringstream iss(line);
            string word, catWord;
            while (iss >> word) {
                if (word == "BALANCE" || word == "INFO" || word == "DEPOSIT" || word == "WITHDRAW" || word == "TRANSFER" || word == "PAY" || word == "DISPLAY" || word == "SAVINGS" || word == "CHECKINGS" || word == "FROM") {
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
    Graph myModel(100);
    createChatModel(myModel); //CHAT BOT -> MODEL / GRAPH / DATA STRUCTURE
    
    Account account("John Doe's Account", 123456789, 112233445566778899, 134.21, 156.43, 845.68);

    cout << "Hello, my name is Auto! I am your personal banking assistant." << endl; //Intro
    string prompt;
    while (prompt != "q") {
        cout << "How can I help you today?: " << endl;
        getline(cin, prompt);
        myModel.promptSearch(prompt, account); // CHAT BOT
    }    

    return 0;
}