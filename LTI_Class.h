#ifndef LTI_CLASS_H_INCLUDED
#define LTI_CLASS_H_INCLUDED

#include "LTI_Class.cpp"

using namespace std;

class LTI {
  public:
    int countRecursive = 0, countNonRecursive = 0, signalsIndex = 0, choice;
    int prevValue = 0, counterOutput = 0;
    bool full = 0;
    vector<double> aCoeff, bCoeff, input, output;
    string FN;
    ofstream fileWrite;
    
    void extractLTI();
    void userInput();
    vector<double> inputSignal();
    void computeOutput();
    void OpenLog();
    void FileWrite();
    void PrintSpecs();
    void Menu();
    void subMenu();
    void clearMem();
    void PrintData();
};

bool isInt(string s, int &value);
bool isDouble(string s, double &value);

#endif