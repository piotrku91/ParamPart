#include "parampart.h"

/* 
Arduino Serial Data Splitter - ParamPart
Written by Piotr Kupczyk (dajmosster@gmail.com) 
2019 - 2020
v. 2.4
*/

////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline void ParamPart::EmptyCut() // Clear unused parameters
{
  int i = ParamReadedCount;
  while (i < Max)
  {
    strcpy(Params[i], NULL);
    i++;
  };
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline void ParamPart::Clear() // Clear everyting (Prepare to next input)
{
  ParamReadedCount = 0;
  strcpy(Command, NULL);
  EmptyCut();
  SyntaxTest = false;
  ReadFlag=false;
  DebugIntegrityDump = "";
};

bool ParamPart::Header(char Expected_Command[])
{
  if ((strcmp(Command, Expected_Command)) == 0)
  {
    return true;
  };
  return false;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool ParamPart::Slicer(char Line[]) // Main function to split line to command and parameters (example format: <name;Peter;30;190;>)
{
  Clear();
  int i = 0;
  char *strtokIndx;
  char DC = DelimiterChar;
  if ((strchr(Line, DC) == NULL) || (Line[0]!=OpenLine))
  {
    SyntaxTest = false;
    Clear();
    return false;
  };

  strtokIndx = strtok(Line, &DC);
  strcpy(Command, strtokIndx);

  for (; ((strtokIndx != NULL) && (i <= Max)); i++)
  {
    strtokIndx = strtok(NULL, &DC);
    strcpy(Params[i], strtokIndx);
  };

  i = i - 2; // Remove > char from end, and cut counter.
  ParamReadedCount = i;
  EmptyCut();
  SyntaxTest = true;
  return true;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void ParamPart::CheckParamTypes()
{
  for (int i = 0; i < Max; i++) // Clear types
  {
    RType[i] = 0;
  }; 

  for (int i = 0; i < Max; i++)
  {

    for (int z = 0; (z < MAX_PARAM_LENGTH && Params[i][z] != NULL); z++)
    {
      // Checking char by char what type is there. Just one char changes type to String (it is not integer) 
      if (isDigit(Params[i][z]))
      {
        if (RType[i] == 1)
          RType[i] = 1; // Type = String (TEKST)
        else
          RType[i] = 0; // Tyoe = Integer (LICZBA)
      }
      else
      {
        RType[i] = 1;
      };
    };
  };
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool ParamPart::Integrity(uint8_t InputExpectedParams, bool Type1, bool Type2, bool Type3, bool Type4, bool Type5, bool Type6, bool Type7, bool Type8, bool Type9)
{
  
  if (InputExpectedParams != ParamReadedCount)
  {
    if (DebugEnabled)
      DebugIntegrityDump = "MEP"; // Missing expected parameters
    return false;                 // Fail integrity test.
  };
  if (!CheckIntegrity)
    return true; // If function is disabled just pass integrity test.

  CheckParamTypes();

  if (DebugEnabled)
  {
    // Expected parameters debug
    DebugIntegrityDump = "E: " + (String)Type1 + (String)Type2 + (String)Type3 + (String)Type4 + (String)Type5 + (String)Type6 + (String)Type7 + (String)Type8 + (String)Type9;
    DebugIntegrityDump = DebugIntegrityDump + " / R: ";

    // Received parameters debug
    for (int i = 0; i < 9; i++)
    {
      DebugIntegrityDump = DebugIntegrityDump + RType[i];
    };
  };

  // If some expected parameter dismatch -> fail integrity test.
  if (InputExpectedParams >= 1)
  {
    if (Type1 != RType[0])
      return false;
  }
  if (InputExpectedParams >= 2)
  {
    if (Type2 != RType[1])
      return false;
  }
  if (InputExpectedParams >= 3)
  {
    if (Type3 != RType[2])
      return false;
  }
  if (InputExpectedParams >= 4)
  {
    if (Type4 != RType[3])
      return false;
  }
  if (InputExpectedParams >= 5)
  {
    if (Type5 != RType[4])
      return false;
  }
  if (InputExpectedParams >= 6)
  {
    if (Type6 != RType[5])
      return false;
  }
  if (InputExpectedParams >= 7)
  {
    if (Type7 != RType[6])
      return false;
  }
  if (InputExpectedParams >= 8)
  {
    if (Type8 != RType[7])
      return false;
  }
  if (InputExpectedParams >= 9)
  {
    if (Type9 != RType[8])
      return false;
  }

  return true; // Parameters match -> Pass integrity test
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////

void ParamPart::Glue(char Line[])
{
  //   Clear();
  String Glue_hand;
  String tmpstr;

  Glue_hand = Command; // Dump command 
  Glue_hand += DelimiterChar;

  for (int i; i < Max; i++) // Dump parameters
  {
    tmpstr = Params[i]; 
    if (Params[i][0] != NULL)
    {

      Glue_hand += tmpstr + DelimiterChar;
    };
  };

  Glue_hand += CloseLine; // Close line
  Line = strcpy(Line, Glue_hand.c_str());
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////

void ParamPart::operator<<(char Line[])
{
  Slicer(Line);
}


void ParamPart::operator<<(String Line)
{
  char CvLine[128];
  strcpy(CvLine, Line.c_str());
  Slicer(CvLine);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////

char *ParamPart::operator[](uint8_t n) 
{
  return Params[n];
}
