#include <iostream>
#include <sstream>
#include <string>
#include <fstream>
#include <vector>

using namespace std;

class LTI {
    public:
      int countRecursive, countNonRecursive, signalsIndex;
      vector<double> aCoeff, bCoeff, input, output = {0.9, 0};
      int index;
      void extractLTI();
      void userInput();
      vector<double> inputSignal();
      void computeOutput();
      void FileWrite();
};

bool isInt(string s, int &value)
{
  int tempValue;
  if ((istringstream(s) >> tempValue).eof())
  {
    value = tempValue;
    return true;
  }
  else
  {
    return false;
  }
}

bool isDouble(string s, double &value)
{
  double tempValue;
  if ((istringstream(s) >> tempValue).eof())
  {
    value = tempValue;
    return true;
  }
  else
  {
    return false;
  }
}

void LTI::extractLTI() //extract LTI Specs
{
  string signalText, tempString, fileName;
  stringstream ss, ss2;
  int lineNumber = 0, counter = 0;
  double tempDouble;

  cout << "Enter file name for the LTI Specifications: ";
  getline(cin, fileName);

  fstream signalFile(fileName);

  if (!signalFile.is_open())
  {
    cout << "Invalid File Name, Please try again..." <<endl;
    extractLTI();
  }
  else if (signalFile.is_open())
  {
    while (getline(signalFile, signalText) && signalText != "")
    {
      if (lineNumber == 0)
      {
        ss << signalText;
        ss >> tempString;
        if(!isInt(tempString, countNonRecursive) || countNonRecursive < 1)
        {
          cout <<"error 1";
          cout << "Invalid LTI File!" <<endl;
          signalFile.close();
          extractLTI();
          break;
        }
        lineNumber++;
      }
      else if (lineNumber == 1)
      {
        ss.str(string());
        ss << signalText;
        ss >> tempString;
        ss.clear();
        if(!isInt(tempString, countRecursive) || countRecursive < 1)
        {
          cout <<"error 2";
          cout << "Invalid LTI File!" <<endl;
          signalFile.close();
          extractLTI();
          break;
        }
        lineNumber++;
      }
      else
      {
        ss2.str(string());
        ss2 << signalText;
        ss2 >> tempString;
        ss2.clear();

        if (isDouble(tempString, tempDouble) && counter < countNonRecursive)
        {
          bCoeff.push_back(tempDouble);
        }
        else if (counter < countNonRecursive+countRecursive)
        {
          aCoeff.push_back(tempDouble);
        }
        counter++;
        tempString = "";
      }
    }
    signalFile.close();
  }
}

void LTI::userInput() //not used yet since might not be needed
{
  string tempString;
  bool valid = 0;
  int counter = 0;
  double tempDouble;

  while(!valid)
  {
    cout << "Number of Non Recursive Coefficients: ";
    getline(cin, tempString);
    if (isInt(tempString, countNonRecursive))
    {
      break;
    }
    else
    {
      cout << "Invalid input!" <<endl;
    }
  }
  while(!valid)
  {
    cout << "Number of Recursive Coefficients: ";
    getline(cin, tempString);
    if (isInt(tempString, countRecursive))
    {
      break;
    }
    else
    {
      cout << "Invalid input!" <<endl;
    }
  }
  while(counter < countNonRecursive)
  {
    cout << "Input Non-Recursive Coefficient: ";
    getline(cin, tempString);
    if (isDouble(tempString, tempDouble))
    {
      bCoeff.push_back(tempDouble);
      counter++;
    }
    else
    {
      cout << "Invalid input!" <<endl;
    }
  }
  
  counter = 0;
  while(counter < countRecursive)
  {
    cout << "Input Recursive Coefficient: ";
    getline(cin, tempString);
    if (isDouble(tempString, tempDouble))
    {
      aCoeff.push_back(tempDouble);
      counter++;
    }
    else
    {
      cout << "Invalid input!" <<endl;
    }
  }
}

vector<double> LTI::inputSignal() //might change return type later
{
  string signalText, tempString, fileName;
  stringstream ss, ss2;
  int lineNumber = 0;
  double tempDouble, tempDouble2;

  cout << "Enter file name for the signals: ";
  getline(cin, fileName);

  fstream signalFile(fileName);

  if (!signalFile.is_open())
  {
    cout << "Invalid File Name, Please try again..." <<endl;
    input = inputSignal();
  }
  else if (signalFile.is_open())
  {
    while (getline(signalFile, signalText) && signalText != "")
    {
      if (lineNumber == 0)
      {
        ss << signalText;
        ss >> tempString;
        if(!isDouble(tempString, tempDouble) 
        && !isInt(tempString, signalsIndex))
        {
          cout << "Invalid Signal File!" <<endl;
          signalFile.close();
          input = inputSignal();
          break;
        }
        else if (!isInt(tempString, signalsIndex))
        {
          if (isDouble(tempString, tempDouble))
          {
            ss >> tempString;
            if (isDouble(tempString,tempDouble2))
            {
              cout << "Invalid Index! " << tempDouble; 
              cout << " is not a valid index." <<endl;
              signalFile.close();
              input = inputSignal();
              break;
            }
            input.push_back(tempDouble);
            signalsIndex = 0;
          }
          
        }
        else
        {
          ss >> tempString;
          if (isDouble(tempString, tempDouble))
          {
            input.push_back(tempDouble);
          }
          ss.clear();
        }
        lineNumber++;
      }
      else
      {
        ss2.str(string());
        ss2 << signalText;
        ss2 >> tempString;
        ss2.clear();

        if (isDouble(tempString, tempDouble))
        {
          input.push_back(tempDouble);
        }
        tempString = "";
      }
    }
    signalFile.close();
  }
  return input;
}

void LTI::computeOutput() //computing y values
{
  for (int i = 0; i<=input.size() - countNonRecursive; i++) //each y signal output
  {
    double sum = 0, sum2 = 0;
    for (int j = 0, k = aCoeff.size() - 1; j<aCoeff.size(); j++, k--)
    {
      sum2+=output[i+j]*aCoeff[k];
    }
    for (int j = 0, k = bCoeff.size() - 1; j<bCoeff.size(); j++, k--)
    {
      sum+=input[i+j]*bCoeff[k];
    }
    output.push_back(sum-sum2);
  }
}

void menu(int &choice) // this might not be here supposedly, just here for testing purposes
{
  string temp;
   cout << "Select from the following";
   cout << "\n[1] Load a file containing specifications for an LTI system"
   << "\n[2] View the full details of the system"
   << "\n[3] Specify the next input interactively"
   << "\n[4] Clear the application of previous inputs and outputs to 0"
   << "\n[5] Print Data onto log file"
   << "\n[6] Terminate the application"
   << "\nSelection: ";
   getline(cin, temp);
   if(!isInt(temp, choice))
   {
      cout << "Invalid Input! Please try again" <<endl;
      menu(choice);
   }
   else if(choice <=0 || choice > 6)
   {
      cout << "Choice " << choice << " is out of range! Try again" <<endl;
      menu(choice);
   }
}

void PrintData(vector<double> x) //same here
{
  for (int i = 0; i < x.size(); i++)
  {
    cout << x[i] << endl;
  }
}

void LTI::FileWrite() //file write function
{
  string FN;
  bool validName = true;

  vector<string> illegalCharacters;
  illegalCharacters.push_back("<");
  illegalCharacters.push_back(">");
  illegalCharacters.push_back(":");
  illegalCharacters.push_back("\"");
  illegalCharacters.push_back("/");
  illegalCharacters.push_back("\\");
  illegalCharacters.push_back("|");
  illegalCharacters.push_back("?");
  illegalCharacters.push_back("*");

  size_t found;

  while(validName){
    cout << "File Name of Output Log file:";
    getline(cin, FN);
    for (int i = 0; i < illegalCharacters.size(); i++){
      if (FN.find(illegalCharacters.at(i)) != string::npos)
      {
        validName = false;
      }
      if (i == illegalCharacters.size() - 1 && validName == true)
      {
        validName = false;
        break;
      }
      if (i == illegalCharacters.size() - 1 && validName == false)
      {
        cout << "Your file name must not include <, >,";
        cout << ":, \", /, \\, |, ?, or *. Please try again." <<endl;
        validName = true;
      }
    }
    FN += ".log";
  }

  ofstream fileWrite(FN.c_str(), ios::out);

  fileWrite << signalsIndex - 2 << " ";
  for(int i=0; i<output.size(); i++)
    {
      fileWrite << output[i] << endl;
    }
  fileWrite.close();
}

int main()
{
  int choice;
  bool active = 1;
  LTI sample;

  cout << "Testing LTI";
  while (active)
  {
    menu(choice);
    if (choice == 1)
    {
      //insert function for loading a file
      sample.extractLTI();
    }
    else if (choice == 2)
    {
      //view full details of system
      //unofficial, change when we understand what this is supposed to be
      cout << "Non Recursive Coeffs: " << sample.countNonRecursive <<endl;
      PrintData(sample.bCoeff);
      cout << "Recursive Coeffs: " << sample.countRecursive <<endl;
      PrintData(sample.aCoeff);
      cout << "Inputs: " <<endl;
      PrintData(sample.input);
      cout << "Corresponding Output: " <<endl;
      PrintData(sample.output);
    }
    else if (choice == 3)
    {
      //code for accepting input not file
      cout << "Accepting input Signal..." <<endl;
      sample.inputSignal();
      sample.computeOutput();
    }
    else if (choice == 4)
    {
      //clear mem
      cout << "Clearing variables..." <<endl;
      sample.countNonRecursive = 0;
      sample.countRecursive = 0;
      sample.bCoeff.clear();
      sample.aCoeff.clear();
    }
    else if (choice ==5)
    {
      //Printing results onto log file
      sample.FileWrite();
    }
    else if (choice == 6)
    {
      cout << "Exiting application..." <<endl;
      return 0;
    }
    else
    {
      cout << "Something went wrong! R.I.P. program" <<endl;
    }
  }
}