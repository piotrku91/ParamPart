#include "parampart.h"

/* 
Arduino Serial String Data Splitter - ParamPart
Written by Piotr K. (dajmosster@gmail.com / piotrq.eu) 
2019 - 2021
v. 3.6

Github: https://github.com/piotrku91/ParamPart/
*/

////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline void ParamPart::EmptyCut() // Clear unused parameters
{
  int i = m_ParamReadCount;
  while (i < m_Max)
  {
    Params[i] = "";
    i++;
  };
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline void ParamPart::Clear() // Clear everyting (Prepare to next input)
{
  m_ParamReadCount = 0;
  Command = "";
  EmptyCut();
  m_SyntaxTest = false;
  setReadFlag(false);
  DebugIntegrityDump = "";
  tmpnewLine = "";
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool ParamPart::Header(const String &CmdName, bool Active) // Compare expected command with received command
{
  if (Active)
  {
    if (!(Export_func == nullptr))
      (*Export_func)(CmdName); // If export function is active, send command to external function for dump.
    return (Command == CmdName);
  }
  return false;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool ParamPart::CSlicer(char Line[]) // Main function to split line to command and parameters (example format: <name;Peter;30;190;>) - Old C String Version
{
  Clear();
  int i = 0;
  char *strtokIndx;
  char DC = DelimiterChar;
  if ((strchr(Line, DC) == 0) || (Line[0] != OpenLine))
  {
    m_SyntaxTest = false;
    Clear();
    return false;
  };

  strtokIndx = strtok(Line, &DC);
  char CommandC[24];
  strcpy(CommandC, strtokIndx + 1);
  Command = CommandC;

  char ParamC[64];
  for (; ((strtokIndx != 0) && (i < m_Max)); i++)
  {
    strtokIndx = strtok(0, &DC);
    strcpy(ParamC, strtokIndx);
    Params[i] = ParamC;
  };

  i = i - 2; // Remove > char from end, and cut counter.
  m_ParamReadCount = i;
  EmptyCut();
  m_SyntaxTest = true;
  return true;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool ParamPart::Slicer(String &LineS) // Main function to split line to command and parameters (example format: <name;Peter;30;190;>)
{
  // Clear();
  int i = 0; // Split counter
  char DC = DelimiterChar;

  if ((LineS.indexOf(OpenLine) == -1) || (LineS.indexOf(DC) == -1) || (LineS.indexOf(CloseLine) == -1)) // Check if is syntax (example chars: <   ;   > )
  // FAIL //
  {
    m_SyntaxTest = false;
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

    while ((NextDel != -1) && (i < m_Max)) // Find and assign all parameters
    {
      Params[i] = LineS.substring(0, NextDel);
      LineS.remove(0, NextDel + 1);
      NextDel = LineS.indexOf(DC);
      i++; // increment split counter
    };

    m_ParamReadCount = i; // Dump split counter to variable
    EmptyCut();
    m_SyntaxTest = true;
    return true;
  }
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////

void ParamPart::CheckParamTypes()
{
  for (int i = 0; i < m_ParamReadCount; i++) // Clear types
  {
    RType[i] = PT::Any; // Set types default to NUMBER
  };

  for (int i = 0; i < m_ParamReadCount; i++)
  {

    if (Params[i] == "0") // If String is 0 then Param eqals to integer with value 0 (don't need any casting)
    {
      RType[i] = PT::Num; // Set type as integer
    }
    else
    {

      int TmpType = Params[i].toInt(); // Cast to integer
      if (TmpType == 0)                // If is 0 then casting failed so type is string
        RType[i] = PT::Txt;            // Set type as String
      else
        RType[i] = PT::Num; // Set type
    };
  };
};

String ParamPart::Interpreter(void (*ptn_func_interpreter)(ParamPart &PP)) // This version of function returns only string with score.
{
  String tmpReturn = "";
  if (syntaxVerify())
  {                                 //   (SYNTAX OK)
    (*ptn_func_interpreter)(*this); // Execute reaction function (callback), push pointer of this class to access from external function.
    if (!(Export_func == nullptr))
      unsetExportFunction();
    if ((DebugEnabled) && (DebugIntegrityDump != ""))
      tmpReturn = DebugIntegrityDump; // Debug Integrity error print (if is ok, nothing to print)
    if ((DebugEnabled) && (!getReadFlag() && (DebugIntegrityDump == "")))
      tmpReturn = "UC! (" + Command + ")"; // Unknown command print
    Clear();                               // Clear parampart to prepare for the next input.
  }
  else
  { // (SYNTAX ERROR)

    if ((DebugEnabled))
      tmpReturn = "SE!"; // Syntax Error - missing < or ; or >
  };
  return tmpReturn;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////

String ParamPart::glue() // You can modify some parameter and stick full command from scratch (dump actual object status)
{
  //   Clear();
  String glue_hand;
  String tmpstr;

  glue_hand = OpenLine + Command; // Dump command
  glue_hand += DelimiterChar;

  for (int i; i < m_Max; i++) // Dump parameters
  {
    tmpstr = Params[i];
    if (Params[i][0] != 0)
    {

      glue_hand += tmpstr + DelimiterChar;
    };
  };

  glue_hand += CloseLine; // Close line
  return glue_hand;       // Returns remastered line
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////

String ParamPart::toJSON()
{
  String tmpJSON = "{\"name\": \"pp_exp\",\"cnt\": \"" + static_cast<String>(m_ParamReadCount) + "\",\"params\":[";
  for (int i = 0; ((i < m_ParamReadCount) && (i < m_Max)); i++)
  {
    tmpJSON += "{\"text\":\"" + Params[i] + "\"}";
    if (i != (m_ParamReadCount - 1))
      tmpJSON += ',';
  };
  tmpJSON += "]}";
  return tmpJSON;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////

String ParamPart::readDone(bool RtnMsg, String ParamRtn, String Rtn) // If command code done
{
  setReadFlag(true);
  if (RtnMsg)
    return (OpenLine + Rtn + DelimiterChar + Command + DelimiterChar + ParamRtn + DelimiterChar + CloseLine);
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////

void ParamPart::operator<<(const char Line[]) // Overload << const char[]
{
  tmpnewLine = Line;
  Slicer(tmpnewLine);
}

void ParamPart::operator<<(char Line[]) // Overload << char[]
{
  tmpnewLine = Line;
  Slicer(tmpnewLine);
}

void ParamPart::operator<<(String &Line) // Overload << String
{
  tmpnewLine = Line;
  Slicer(tmpnewLine);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////

String &ParamPart::operator[](uint8_t n) const // Overload [] - returns reference to choosed param
{

  return Params[n];
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////

void ParamPart::setSyntaxChars(char OpenLine, char Delimiter, char CloseLine)
{
  this->OpenLine = OpenLine;
  this->DelimiterChar = DelimiterChar;
  this->CloseLine = CloseLine;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////
void ParamPart::setExportFunction(void (*External_Export_func)(const String &))
{
  Export_func = External_Export_func;
  CheckIntegrity = false;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////
void ParamPart::unsetExportFunction()
{
  Export_func = nullptr;
  CheckIntegrity = true;
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////