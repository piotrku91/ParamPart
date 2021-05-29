#include "parampart.h"

/* 
Arduino Serial std::string Data Splitter - ParamPart
Written by Piotr K. (dajmosster@gmail.com / piotrq.eu) 
2019 - 2021
v. 3.7

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
  m_Command = "";
  EmptyCut();
  m_SyntaxTest = false;
  setReadFlag(false);
  m_DebugIntegrityDump = "";
  m_tmpnewLine = "";
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool ParamPart::Header(const std::string &CmdName, bool Active) // Compare expected command with received command
{
  if (Active)
  {
    if (!(Export_func == nullptr))
      (*Export_func)(CmdName); // If export function is active, send command to external function for dump.
    return (m_Command == CmdName);
  }
  return false;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Old version of Slicer (Cstd::string) - It is deavtivated and will be delete in the future.
#if 0 
bool ParamPart::CSlicer(char Line[]) 
{
  Clear();
  int i = 0;
  char *strtokIndx;
  char DC = m_DelimiterChar;
  if ((strchr(Line, DC) == 0) || (Line[0] != m_OpenLine))
  {
    m_SyntaxTest = false;
    Clear();
    return false;
  };

  strtokIndx = strtok(Line, &DC);
  char m_CommandC[24];
  strcpy(m_CommandC, strtokIndx + 1);
  m_Command = m_CommandC;

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

#endif

////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool ParamPart::Slicer(std::string &LineS) // Main function to split line to command and parameters (example format: <name;Peter;30;190;>)
{
  // Clear();
  int i = 0; // Split counter
  char DC = m_DelimiterChar;

  if ((LineS.find(m_OpenLine) == std::string::npos) || (LineS.find(DC) == std::string::npos) || (LineS.find(m_CloseLine) == std::string::npos)) // Check if is syntax (example chars: <   ;   > )
  // FAIL //
  {
    m_SyntaxTest = false;
    Clear();
    return false;
  }
  else
  // PASS //
  {
    int NextDel = LineS.find(DC);             // Find first delimiter
    m_Command = LineS.substr(1, NextDel - 1); // Copy command to variable
    LineS.erase(0, NextDel + 1);              // Delete from the begin to first delimiter + 1
    NextDel = LineS.find(DC);                 // Find next delimiter

    while ((NextDel != -1) && (i < m_Max)) // Find and assign all parameters
    {
      Params[i] = LineS.substr(0, NextDel);
      LineS.erase(0, NextDel + 1);
      NextDel = LineS.find(DC);
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

    if (Params[i] == "0") // If std::string is 0 then Param eqals to integer with value 0 (don't need any casting)
    {
      RType[i] = PT::Num; // Set type as integer
    }
    else
    {

      int TmpType; // Cast to integer
      if (!(sscanf(Params[i].c_str(), "%d", &TmpType)))
      {
        TmpType = 0;
      };

      if (TmpType == 0)     // If is 0 then casting failed so type is string
        RType[i] = PT::Txt; // Set type as std::string
      else
        RType[i] = PT::Num; // Set type
    };
  };
};

std::string ParamPart::Interpreter(void (*ptn_func_interpreter)(ParamPart &PP)) // This version of function returns only string with score.
{
  std::string tmpReturn = "";
  if (syntaxVerify())
  {                                 //   (SYNTAX OK)
    (*ptn_func_interpreter)(*this); // Execute reaction function (callback), push pointer of this class to access from external function.
    if (!(Export_func == nullptr))
      unsetExportFunction();
    if ((m_DebugEnabled) && (m_DebugIntegrityDump != ""))
      tmpReturn = m_DebugIntegrityDump; // Debug Integrity error print (if is ok, nothing to print)
    if ((m_DebugEnabled) && (!getReadFlag() && (m_DebugIntegrityDump == "")))
      tmpReturn = getError(Errors::UnknownCommand) + " (" + m_Command + ")"; // Unknown command print
    Clear();                                                                 // Clear parampart to prepare for the next input.
  }
  else
  { // (SYNTAX ERROR)

    if ((m_DebugEnabled))
      tmpReturn = getError(Errors::SyntaxError); // Syntax Error - missing < or ; or >
  };
  return tmpReturn;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////

std::string ParamPart::glue() // You can modify some parameter and stick full command from scratch (dump actual object status)
{
  //   Clear();
  std::string glue_hand;
  std::string tmpstr;

  glue_hand = m_OpenLine + m_Command; // Dump command
  glue_hand += m_DelimiterChar;

  for (int i; i < m_Max; i++) // Dump parameters
  {
    tmpstr = Params[i];
    if (Params[i][0] != 0)
    {

      glue_hand += tmpstr + m_DelimiterChar;
    };
  };

  glue_hand += m_CloseLine; // Close line
  return glue_hand;         // Returns remastered line
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////

std::string ParamPart::toJSON()
{
  std::string tmpJSON = "{\"name\": \"pp_exp\",\"cnt\": \"" + std::to_string(m_ParamReadCount) + "\",\"params\":[";
  for (int i = 0; ((i < m_ParamReadCount) && (i < m_Max)); i++)
  {
    tmpJSON += "{\"" + DebugNames[static_cast<int>(RType[i])] + "\":\"" + Params[i] + "\"}";
    if (i != (m_ParamReadCount - 1))
      tmpJSON += ',';
  };
  tmpJSON += "]}";
  return tmpJSON;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////

std::string ParamPart::readDone(bool RtnMsg, std::string ParamRtn, std::string Rtn) // If command code done
{
  setReadFlag(true);
  if (RtnMsg)
    return (m_OpenLine + Rtn + m_DelimiterChar + m_Command + m_DelimiterChar + ParamRtn + m_DelimiterChar + m_CloseLine);
  return "";
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////

void ParamPart::operator<<(const char Line[]) // Overload << const char[]
{
  m_tmpnewLine = Line;
  Slicer(m_tmpnewLine);
}

void ParamPart::operator<<(std::string &Line) // Overload << std::string
{
  m_tmpnewLine = Line;
  Slicer(m_tmpnewLine);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////

std::string ParamPart::operator[](uint8_t n) const // Overload [] - returns reference to choosed param
{
  return Params[n];
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////

void ParamPart::setSyntaxChars(char OpenLine, char Delimiter, char CloseLine)
{
  this->m_OpenLine = OpenLine;
  this->m_DelimiterChar = Delimiter;
  this->m_CloseLine = CloseLine;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////
void ParamPart::setExportFunction(void (*External_Export_func)(const std::string &))
{
  Export_func = External_Export_func;
  m_CheckIntegrity = false;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////
void ParamPart::unsetExportFunction()
{
  Export_func = nullptr;
  m_CheckIntegrity = true;
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool ParamPart::modifyParam(int ParamIndex, const std::string &Value)
{
  if ((ParamIndex < m_ParamReadCount) && (ParamIndex < m_Max))
  {
    Params[ParamIndex] = Value;
    return true;
  }
  return false;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////