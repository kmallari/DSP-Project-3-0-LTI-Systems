#include <iostream>
#include <sstream>
#include <string>
#include <fstream>
#include <vector>

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
  //clear when load new LTI specs
  countNonRecursive = 0;
  countRecursive = 0;
  counterOutput = 0;
  aCoeff.clear();
  bCoeff.clear();
  fileWrite.close();
  clearMem();

  string signalText, tempString, fileName;
  stringstream ss, ss2;
  int lineNumber = 0, counter = 0;
  double tempDouble = 0;

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

    //initialize necessary previous values to 0
    for (int i = 1; i<countNonRecursive; i++)
    {
      input.push_back(0);
    }
    for (int i = 1; i<countRecursive; i++)
    {
      output.push_back(0);
    }
  }
}

void LTI::userInput() //changed to one by one input
{
  string tempString;
  bool valid = 0;
  int counter = 0;
  double tempDouble;
  
  cout << "Enter next input: ";
  getline(cin, tempString);

  if(!isDouble(tempString, tempDouble))
  {
    cout << tempString << " is an invalid input!" <<endl;
    userInput();
  }
  else
  {
    cout << "Successful!" <<endl;
    input.push_back(tempDouble);
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
  for (int i = prevValue; i<=input.size() - countNonRecursive; i++) //each y signal output
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

void LTI::Menu() // this might not be here supposedly, just here for testing purposes
{
  string temp;
  
   cout << "Select from the following";
   cout << "\n[1] Load a file containing specifications for an LTI system"
   << "\n[2] View the full details of the system"
   << "\n[3] Specify the input file"
   << "\n[4] Specify the next input interactively"
   << "\n[5] Clear the application of previous inputs and outputs to 0"
   << "\n[6] Specify log file"
   << "\n[7] Terminate the application"
   << "\nSelection: ";
   getline(cin, temp);
   if(!isInt(temp, choice))
   {
      cout << "Invalid Input! Please try again" <<endl;
      Menu();
   }
   else if(choice <=0 || choice > 7)
   {
      cout << "Choice " << choice << " is out of range! Try again" <<endl;
      Menu();
   }
}

void LTI::subMenu()
{
  if (fileWrite.is_open())
  {
    string temp;
    int subChoice;
    
    cout << "Print the output data on screen?";
    cout << "\n[1] Yes"
    << "\n[2] No"
    << "\nSelection: ";
    getline(cin, temp);
    if(!isInt(temp, subChoice))
    {
        cout << "Invalid Input! Please try again" <<endl;
        subMenu();
    }
    else if(subChoice <=0 || subChoice > 2)
    {
        cout << "Choice " << choice << " is out of range! Try again" <<endl;
        subMenu();
    }

    if (subChoice == 1)
    {
      PrintData();
    }
    else
    {
      cout << endl;
    }
  }
  else
  {
    PrintData();
  }
}

void LTI::PrintData() //same here
{
  if (full)
  {
    cout << "Input: " <<endl;
    for (int i = countNonRecursive -1, j = 0; i < input.size(); i++, j++)
    {
      cout << "x(" << j << ")\t" << input[i] << endl;
    }
    cout << "\nOutput: " <<endl;
    for (int i = countRecursive -1, j = 0; i < output.size(); i++, j++)
    {
      cout << "y(" << j << ")\t" << output[i] << endl;
    }
    cout <<endl;
  }
  else
  {
    cout << "\nOutput: " <<endl;
    for (int i = prevValue + countRecursive -1, j = prevValue; i < output.size(); i++, j++)
    {
      cout << "y(" << j << ")\t" << output[i] << endl;
    }
    cout <<endl;
  }
}

void LTI::OpenLog()
{
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
}

void LTI::FileWrite() //file write function
{
  if(fileWrite.is_open())
  {
    for (int i = counterOutput; i<output.size(); i++)
    {
      fileWrite << output[i] <<endl;
    }
    counterOutput = output.size();
  }
  else if (!FN.empty())
  {
    fileWrite.open(FN.c_str(), ios::out);

    fileWrite << 0 << " ";
    for(int i=countRecursive -1; i<output.size(); i++)
    {
      fileWrite << output[i] << endl;
    }
    counterOutput = output.size();
  }
}

void LTI::PrintSpecs() //this can probably be improved
{
  if (!full)
  {
    cout << "LTI Specifications (Max of 10 values)" <<endl;
  }
  else
  {
    cout << "LTI FULL Specifications" <<endl;
  }

  cout << "m/n\tb\ta" <<endl;
  for(int i = 0; i<countRecursive || i<countNonRecursive; i++)
  {
    if (i==10 && !full)
    {
      break;
    }
    cout << i << "\t";
    if (i == 0 && bCoeff.size() != 0)
    {
      cout << bCoeff[i] <<endl;
    }
    else if (i<countNonRecursive && i<countRecursive)
    {
      cout << bCoeff[i] << "\t" << aCoeff[i - 1] <<endl;
    }
    else if (i<countNonRecursive)
    {
      cout << bCoeff[i] <<endl;
    }
    else if (i<countRecursive && aCoeff.size() != 0)
    {
      cout << " \t" << aCoeff[i - 1] <<endl;
    }
    else
    {
      cout <<endl;
    }
  }
  cout <<endl;

  if(full)
  {
    PrintData();
  }
}

void LTI::clearMem()
{
  input.clear();
  output.clear();
  prevValue = 0;
}

int main()
{
  bool active = 1;
  LTI sample;

  cout << "Testing LTI" <<endl;
  while (active)
  {
    if(sample.choice !=2) //prevent double print, might move within menu after discussion
    {
      sample.PrintSpecs();
    }
    sample.Menu();
    if (sample.choice == 1)
    {
      //insert function for loading a file
      sample.extractLTI();
    }
    else if (sample.choice == 2)
    {
      sample.full = 1;
      sample.PrintSpecs();
      sample.full = 0;
    }
    else if (sample.choice == 3)
    {
      //code for accepting input not file
      cout << "Accepting input Signal..." <<endl;
      sample.inputSignal();
      sample.computeOutput();
      sample.subMenu();
      sample.FileWrite();
      sample.prevValue = sample.output.size() - sample.countRecursive + 1;
    }
    else if (sample.choice == 4)
    {
      cout << "Accepting Input INTERACTIVELY..." <<endl;
      sample.userInput();
      sample.computeOutput();
      sample.subMenu();
      sample.FileWrite();
      sample.prevValue = sample.output.size() - sample.countRecursive + 1;
    }
    else if (sample.choice == 5)
    {
      //clear mem
      cout << "Clearing previous input/output values..." <<endl;
      sample.clearMem();
      //initialize necessary previous values to 0
      for (int i = 1; i<sample.countNonRecursive; i++) //might move these to separate method
      {
        sample.input.push_back(0);
      }
      for (int i = 1; i<sample.countRecursive; i++)
      {
        sample.output.push_back(0);
      }
    }
    else if (sample.choice == 6)
    {
      //Open log file and writes output in memory
      sample.fileWrite.close();
      sample.OpenLog();
      sample.FileWrite();
    }
    else if (sample.choice == 7)
    {
      cout << "Exiting application..." <<endl;
      sample.fileWrite.close();
      return 0;
    }
    else
    {
      cout << "Something went wrong! R.I.P. program" <<endl;
    }
  }
}