#include "parampart_pcs.h"

/* 
Arduino Serial std::string Data Splitter - ParamPart_pcs (PC SIDE PORT)
Written by Piotr Kupczyk (dajmosster@gmail.com) 
2019 - 2020
v. 3.3.6

Port version to use on PC side.

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

////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool ParamPart::Header(const std::string& CmdName) // Compare expected command with received command 
{
return (Command==CmdName);
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool ParamPart::UseAsHeader(const std::string& CmdName,uint8_t ParamIndex)
{
return (Params[ParamIndex]==CmdName);
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool ParamPart::Header(std::string& CmdName) // Compare expected command with received command 
{
return (Command==CmdName);
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool ParamPart::CSlicer(char Line[]) // Main function to split line to command and parameters (example format: <name;Peter;30;190;>) - Old C std::string Version
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

bool ParamPart::Slicer(std::string& LineS) // Main function to split line to command and parameters (example format: <name;Peter;30;190;>)
{
 // Clear();
  int i = 0; // Split counter
  char DC = DelimiterChar;

  if ((LineS.find(OpenLine) == std::string::npos) || (LineS.find(DC) == std::string::npos) || (LineS.find(CloseLine) == std::string::npos)) // Check if is syntax (example chars: <   ;   > )
  // FAIL //
  {
    SyntaxTest = false;
    Clear();
    return false;
  }
  else
  // PASS //
  {

    int NextDel = LineS.find(DC);       // Find first delimiter
    Command = LineS.substr(1, NextDel-1); // Copy command to variable
    LineS.erase(0, NextDel + 1);          // Delete from the begin to first delimiter + 1
    NextDel = LineS.find(DC);           // Find next delimiter

    while ((NextDel != -1) && (i <= Max)) // Find and assign all parameters
    {
      Params[i] = LineS.substr(0, NextDel);
      LineS.erase(0, NextDel + 1);
      NextDel = LineS.find(DC);
      i++; // increment split counter
    };

    ParamReadCount = i; // Dump split counter to variable
    EmptyCut();
    SyntaxTest = true;
    return true;
  }
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////

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

      // Checking char by char what type is there. Just one char changes type to std::string (it is not integer)
      if ((std::isdigit(Params[i][z])) || (Params[i][0]=='-') || ((Params[i][z]=='.') && (Params[i][0]!='.')))
      {
        RType[i] = (RType[i] == 1) ? 1 : 0; // 1 - std::string, 0 - NUMBER
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
    DebugIntegrityDump = "E: " + std::to_string(Type1) + std::to_string(Type2) + std::to_string(Type3) + std::to_string(Type4) + std::to_string(Type5) + std::to_string(Type6) + std::to_string(Type7) + std::to_string(Type8) + std::to_string(Type9);
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

////////////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////// Interpreter functions//////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////



///////////////////////////////////////Version for class member reaction function///////////////////////////

// Example execution from TManager class:
/*

    typedef void (TManager::*ReaPTR)(ParamPart &PP);
    ReaPTR MemberReactionPointer= &TManager::Reaction;
    m_ParamPart_ptr->Interpreter(this,MemberReactionPointer);

*/

std::string ParamPart::Interpreter(TManager *M,void (TManager::*ptn_func_interpreter)(ParamPart &PP)) // This overloaded version for cooperate with class member function.
   { std::string tmpReturn="";
    if (SyntaxVerify())
    {                                  //   (SYNTAX OK)6
        (M->*ptn_func_interpreter)(*this); // Execute reaction function (callback), push pointer of this class to access from external function.

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
///////////////////////////////////Version for global or static reaction function///////////////////////////
///
/// // Example execution from main or static function
/*

    ParamPart Reader;
    Reader << "<somecomand;2;>";
    Reader.Interpreter(&Reaction);

    .... see readme ....

*/


std::string ParamPart::Interpreter(void (*ptn_func_interpreter)(ParamPart &PP)) // This version of function returns only std::string with score.
{
    std::string tmpReturn="";
    if (SyntaxVerify())
    {                                  //   (SYNTAX OK)6
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
////////////////////////////////////////////////////////////////////////////////////////////////////////////

std::string ParamPart::Glue() // You can modify some parameter and stick full command from scratch (dump actual object status)
{
  //   Clear();
  std::string Glue_hand;
  std::string tmpstr;

  Glue_hand = OpenLine + Command; // Dump command
  Glue_hand += DelimiterChar;

  for (int i=0; i < Max; i++) // Dump parameters
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

std::string ParamPart::ReadDone(bool RtnMsg, std::string ParamRtn, std::string Rtn) // If command code done
{
    SetReadFlag(true);
     if (RtnMsg){
        return (OpenLine + Rtn + DelimiterChar + Command + DelimiterChar + ParamRtn + DelimiterChar + CloseLine);};
        return "";
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

void ParamPart::operator<<(const std::string& Line) // Overload << std::string
{
  tmpnewLine=Line;
  Slicer(tmpnewLine);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////


#if QSTRING_SERVICE

void ParamPart::operator<<(const QString& Line) // Overload << std::string
{
  tmpnewLine=Line.toStdString();
  Slicer(tmpnewLine);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////

const QString ParamPart::operator[](uint8_t n) // Overload [] - returns pointer to choosed param
{

  return QString::fromStdString(Params[n]);
}


#else

const std::string ParamPart::operator[](uint8_t n) // Overload [] - returns pointer to choosed param
{

  return Params[n];
}




#endif
