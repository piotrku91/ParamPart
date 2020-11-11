#include "parampart.h"

/* 
Arduino Serial String Data Splitter - ParamPart
Written by Piotr Kupczyk (dajmosster@gmail.com) 
2019 - 2020
v. 3.3.4

Github: https://github.com/piotrku91/ParamPart/
*/

////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline void ParamPart::EmptyCut() // Clear unused parameters
{
  int i = ParamReadCount;
  while (i < Max)
  {
    Params[i] = "";
    i++;
  };
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline void ParamPart::Clear() // Clear everyting (Prepare to next input)
{
  ParamReadCount = 0;
  Command = "";
  EmptyCut();
  SyntaxTest = false;
  SetReadFlag(false);
  DebugIntegrityDump = "";
  tmpnewLine="";
};

bool ParamPart::Header(const String& CmdName) // Compare expected command with received command 
{
return (Command==CmdName);
};

bool ParamPart::Header(String& CmdName) // Compare expected command with received command 
{
return (Command==CmdName);
};

bool ParamPart::CSlicer(char Line[]) // Main function to split line to command and parameters (example format: <name;Peter;30;190;>) - Old C String Version
{
  Clear();
  int i = 0;
  char *strtokIndx;
  char DC = DelimiterChar;
  if ((strchr(Line, DC) == 0) || (Line[0] != OpenLine))
  {
    SyntaxTest = false;
    Clear();
    return false;
  };

  strtokIndx = strtok(Line, &DC);
  char CommandC[24];
  strcpy(CommandC, strtokIndx + 1);
  Command = CommandC;

  char ParamC[64];
  for (; ((strtokIndx != 0) && (i <= Max)); i++)
  {
    strtokIndx = strtok(0, &DC);
    strcpy(ParamC, strtokIndx);
    Params[i] = ParamC;
  };

  i = i - 2; // Remove > char from end, and cut counter.
  ParamReadCount = i;
  EmptyCut();
  SyntaxTest = true;
  return true;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool ParamPart::Slicer(String& LineS) // Main function to split line to command and parameters (example format: <name;Peter;30;190;>)
{
 // Clear();
  int i = 0; // Split counter
  char DC = DelimiterChar;

  if ((LineS.indexOf(OpenLine) == -1) || (LineS.indexOf(DC) == -1) || (LineS.indexOf(CloseLine) == -1)) // Check if is syntax (example chars: <   ;   > )
  // FAIL //
  {
    SyntaxTest = false;
    Clear();
    return false;
  }
  else
  // PASS //
  {

    int NextDel = LineS.indexOf(DC);       // Find first delimiter
    Command = LineS.substring(1, NextDel); // Copy command to variable
    LineS.remove(0, NextDel + 1);          // Delete from the begin to first delimiter + 1
    NextDel = LineS.indexOf(DC);           // Find next delimiter

    while ((NextDel != -1) && (i <= Max)) // Find and assign all parameters
    {
      Params[i] = LineS.substring(0, NextDel);
      LineS.remove(0, NextDel + 1);
      NextDel = LineS.indexOf(DC);
      i++; // increment split counter
    };

    ParamReadCount = i; // Dump split counter to variable
    EmptyCut();
    SyntaxTest = true;
    return true;
  }
};

void ParamPart::CheckParamTypes()
{
  for (int i = 0; i < Max; i++) // Clear types
  {
    RType[i] = 0; // Set types default to NUMBER
  };

  for (int i = 0; i < Max; i++)
  {

    for (int z = 0; (Params[i][z] != 0); z++)
    {

      // Checking char by char what type is there. Just one char changes type to String (it is not integer)
      if ((isDigit(Params[i][z])) || (Params[i][0]=='-') || ((Params[i][z]=='.') && (Params[i][0]!='.')))
      {
        RType[i] = (RType[i] == 1) ? 1 : 0; // 1 - STRING, 0 - NUMBER
      }
      else
      {
        RType[i] = 1;
      };


    };


  };
};

bool ParamPart::Integrity(uint8_t InputExpectedParams, bool Type1, bool Type2, bool Type3, bool Type4, bool Type5, bool Type6, bool Type7, bool Type8, bool Type9)
{

   if (!CheckIntegrity)
    return true; // If function is disabled just pass integrity test.
  

  if (InputExpectedParams != ParamReadCount)
  {
    if (DebugEnabled)
      DebugIntegrityDump = "MEP"; // Missing/Extra expected parameters
    return false;                 // Fail integrity test.
  };
 

  CheckParamTypes();

  if (DebugEnabled)
  {
    // Expected parameters debug
    DebugIntegrityDump = "E: " + (String)Type1 + (String)Type2 + (String)Type3 + (String)Type4 + (String)Type5 + (String)Type6 + (String)Type7 + (String)Type8 + (String)Type9;
    DebugIntegrityDump += " / R: ";

    // Received parameters debug
    for (int i = 0; i < 9; i++)
    {
      DebugIntegrityDump += RType[i];
    };
    DebugIntegrityDump += " MM!"; // mismatch parameters
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

  DebugIntegrityDump = ""; // if pass integrity test just clean debug

  return true; // Parameters match -> Pass integrity test
};




String ParamPart::Interpreter(void (*ptn_func_interpreter)(ParamPart &PP)) // This version of function returns only string with score.
{
    String tmpReturn="";
    if (SyntaxVerify())
    {                                  //   (SYNTAX OK)
        (*ptn_func_interpreter)(*this); // Execute reaction function (callback), push pointer of this class to access from external function.

        if ((DebugEnabled) && (DebugIntegrityDump != ""))
            tmpReturn=DebugIntegrityDump; // Debug Integrity error print (if is ok, nothing to print)
        if ((DebugEnabled) && (!GetReadFlag() && (DebugIntegrityDump == "")))
            tmpReturn="UC! (" + Command + ")"; // Unknown command print
        Clear();                                          // Clear parampart to prepare for the next input.
    }
    else
    { // (SYNTAX ERROR)

        if ((DebugEnabled))
            tmpReturn="SE!"; // Syntax Error - missing < or ; or >
    };
    return tmpReturn;
};




////////////////////////////////////////////////////////////////////////////////////////////////////////////

String ParamPart::Glue() // You can modify some parameter and stick full command from scratch (dump actual object status)
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
  return Glue_hand;       // Returns remastered line
};

String ParamPart::ReadDone(bool RtnMsg, String ParamRtn, String Rtn) // If command code done
{
    SetReadFlag(true);
     if (RtnMsg)
        return (OpenLine + Rtn + DelimiterChar + Command + DelimiterChar + ParamRtn + DelimiterChar + CloseLine);
};


////////////////////////////////////////////////////////////////////////////////////////////////////////////

 void ParamPart::operator<<(const char Line[]) // Overload << const char[]
 {
  tmpnewLine=Line;
  Slicer(tmpnewLine);
}

 void ParamPart::operator<<(char Line[]) // Overload << char[]
 {
  tmpnewLine=Line;
  Slicer(tmpnewLine);
}

void ParamPart::operator<<(String& Line) // Overload << String
{
  tmpnewLine=Line;
  Slicer(tmpnewLine);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////

const String ParamPart::operator[](uint8_t n) // Overload [] - returns pointer to choosed param
{

  return Params[n];
}
