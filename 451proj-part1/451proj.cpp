/*
File: 451proj.cpp
Project: CMSC 451 Course Project
Authors: Humza Faraz & Joshua Philip Santos
Date: 10/16/21
Emails: hfaraz1@umbc.edu & jsantos3@umbc.edu
Description:
*
DFA simulator. Creates DFA from text file then takes in user input and determines if input is validated. 
*
*/

#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <iterator>
#include <utility>
#include <vector>
#include <stdlib.h>
using namespace std;

int main(int argc, char* argv[]){
    ifstream myfile;
    if (argc < 2){
        cout << "Missing data file!" << endl;
        exit(0);
    }
    else{
        myfile.open(argv[1]);
    }

    multimap <string, pair <char, string> > mymap;
    string startState;
    vector<string> acceptStates;
    bool validFile = true;
    char inputAlphabet1, inputAlphabet2;

    if (myfile.is_open()){
        int numStates;
        string acceptState, hyphens;

        myfile >> inputAlphabet1;
        myfile >> inputAlphabet2;
        myfile >> numStates;

        //make loop for accept states
        myfile >> acceptState;
        acceptStates.push_back(acceptState);
        myfile >> hyphens;
        while (hyphens != "---"){
            acceptStates.push_back(hyphens);
            myfile >> hyphens;
        }

        //check valid hyphens
        if (hyphens != "---"){
            cout << "Invalid hyphens!" << endl;
            validFile = false;
        }

        //check valid accept states
        if ((int)acceptStates.size() > 1){
            for (int i = 0; i < ((int)acceptStates.size() - 1); i++){
                if (acceptStates[i][0] != acceptStates[i+1][0]){
                    cout << "Invalid accept states!" << endl;
                    validFile = false;
                }
            }
        }

        int count = 0;
        while(validFile == true){
            string start, transition;
            char inputAlpha;
            myfile >> start;
            if (start == "" || start == " " || start == "\n"){
                break;
            }
            if (count == 0){
                startState = start;
                count += 1;
            }
            myfile >> inputAlpha;
            myfile >> transition;

            //check for valid states
            if (start[0] != transition[0]){
                cout << "Invalid state in rule!" << endl;
                validFile = false;
            }

            if (inputAlpha != inputAlphabet1 && inputAlpha != inputAlphabet2){
                cout << "Invalid input alphabet!" << endl;
                validFile = false;
            }

            mymap.insert(make_pair( start, make_pair(inputAlpha, transition ) ) );
        }
        myfile.close();
    }
    else{
        cout << "Invalid text file!" << endl;
        exit(0);
    }

    while (validFile == true) {
        string userInput;
        cout << "Please enter an input string:" << endl;
        cin >> userInput;

        string output;
        vector<string> states;
        states.push_back(startState);
        string currentState = startState;

        //create path
        bool isValid = true;
        for (int i = 0; i < userInput.length(); i++){
            if (userInput[i] != inputAlphabet1 && userInput[i] != inputAlphabet2){
                cout << "Invalid input!" << endl;
                isValid = false;
                break;
            }
            typedef multimap<string, pair <char, string> >::iterator MMAPIterator;
            pair<MMAPIterator, MMAPIterator> result = mymap.equal_range(currentState);
            for (MMAPIterator it = result.first; it != result.second; it++){
                if (it->second.first == userInput[i]){
                    currentState = it->second.second;
                    states.push_back(currentState);
                    break;
                }
            }
        }
        
        //Check if input is accepted
        bool isFound = false;
        for (int i = 0; i < (int)acceptStates.size(); i++){
            if (currentState == acceptStates[i]){
                isFound = true;
                break;
            }
        }

        //Print path
        if (isValid == true){
            if (isFound == true){
                cout << "Input is accepted. Path:" << endl;
            }
            else{
                cout << "Input is rejected. Path:" << endl;
            }
            for (int i = 0; i < (int)states.size(); i++){
                cout << states[i] << " "; 
            }
            cout << endl;
        }
    }
    
    return 0;
}