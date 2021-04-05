#include <iostream>
#include <sstream>
#include <string>
#include <fstream>
#include <vector>
#include "LTI_Class.h"

using namespace std;

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

