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
    strcpy(Params[i], 0);
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
  SetReadFlag(false);
  DebugIntegrityDump = "";
};

bool ParamPart::Header(String CmdName)
{
  const char *Cmd = CmdName.c_str();
  if ((strcmp(Command, Cmd)) == 0)
  {
    return true;
  };
  return false;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool ParamPart::Slicer(String *LineS) // Main function to split line to command and parameters (example format: <name;Peter;30;190;>)
{
  Clear();
  //char * Line = new char[LineS->length()];
  char Line[128];
  strcpy(Line, LineS->c_str());
  int i = 0; // Split counter
  char *strtokIndx;
  char DC = DelimiterChar;
  if ((Line[0] != OpenLine) || (strchr(Line, DC) == 0)) // Check if is syntax (example chars: <   ;   > )
  {
    SyntaxTest = false;
    Clear();
    return false;
  };

  strtokIndx = strtok(Line, &DC);
  strcpy(Command, strtokIndx + 1);

  for (; ((strtokIndx != 0) && (i <= Max)); i++)
  {
    strtokIndx = strtok(0, &DC);
    strcpy(Params[i], strtokIndx);
  };

  i = i - 2; // Remove > char from end
  ParamReadedCount = i;
  EmptyCut();
  SyntaxTest = true;
 // delete Line;
  return true;
};

void ParamPart::CheckParamTypes()
{
  for (int i = 0; i < Max; i++) // Clear types
  {
    RType[i] = 0;
  };

  for (int i = 0; i < Max; i++)
  {

    for (int z = 0; (z < MAX_PARAM_LENGTH && Params[i][z] != 0); z++)
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

  if (DebugIntegrityEnabled)
  {
    // Expected parameters debug
    DebugIntegrityDump = "E: " + (String)Type1 + (String)Type2 + (String)Type3 + (String)Type4 + (String)Type5 + (String)Type6 + (String)Type7 + (String)Type8 + (String)Type9;
    DebugIntegrityDump += " / R: ";

    // Received parameters debug
    for (int i = 0; i < 9; i++)
    {
      DebugIntegrityDump += RType[i];
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

String ParamPart::Glue()
{
  //   Clear();
  String Glue_hand;
  String tmpstr;

  Glue_hand = OpenLine + Command; // Dump command
  Glue_hand += DelimiterChar;

  for (int i; i < Max; i++) // Dump parameters
  {
    tmpstr = Params[i];
    if (Params[i][0] != 0)
    {

      Glue_hand += tmpstr + DelimiterChar;
    };
  };

  Glue_hand += CloseLine; // Close line
  return Glue_hand;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////

void ParamPart::operator<<(char Line[])
{
  Slicer(&((String)Line));
}

void ParamPart::operator<<(String Line)
{
  Slicer(&Line);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////

char *ParamPart::operator[](uint8_t n)
{
  return Params[n];
}
