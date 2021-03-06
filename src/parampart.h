#pragma once
#include <Arduino.h>

/* 
Arduino Serial String Data Splitter - ParamPart
Written by Piotr K. (dajmosster@gmail.com / piotrq.eu) 
2019 - 2021
v. 3.7

Github: https://github.com/piotrku91/ParamPart/
*/

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                        CLASS ParamPart  - Header file                                                                      //
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define CHECK_INTEGRITY_DEFAULT_STATUS true
#define DEBUG_DEFAULT_STATUS true

enum class PT
{
  Num,
  Txt,
  Any
}; // Types to use with integrity checks. Use like PT::Num, PT::Txt, PT::Any as arguments in Integrity function.

class ParamPart // Arduino String Serial Data Splitter
{
protected:
  // ------------------------------------------ Errors ------------------------------------------
  enum class Errors
  {
    SyntaxError,
    UnknownCommand,
    MissingExtraParameter,
    Mismatch
  };
  const String ErrorMsgNames[4] = {{"SE!"}, {"UC!"}, {"MEP!"}, {"MM!"}}; // Error returns

  // ------------------------------------------  Types ------------------------------------------

  const String DebugNames[3] = {{"[Num]"}, {"[Txt]"}, {"[Any]"}}; // Debug types text
  PT *RType;                                                      // Params Type Table (dynamic array)

  // ------------------------------------------  Members ------------------------------------------
  const uint8_t m_Max;
  uint8_t m_ParamReadCount;
  bool m_SyntaxTest;
  bool m_ReadFlag;

  String *Params; // Params String Table (dynamic array)

  String m_tmpnewLine; // Workflow variable
  bool m_CheckIntegrity;
  bool m_DebugEnabled;
  char m_DelimiterChar;
  char m_OpenLine;
  char m_CloseLine;
  String m_Command;
  void (*Export_func)(const String &);
  String m_DebugIntegrityDump;
  String m_RawCopy;

  // ------------------------------------------ Public functions ------------------------------------------
public:
  void Clear();
  bool Header(const String &CmdName, bool Active = true);
  String Interpreter(void (*ptn_func_interpreter)(ParamPart &PP));
  String readDone(bool RtnMsg = true, String ParamRtn = "OK", String Rtn = "artn");

  // **Set functions
  void setDebugMode(bool DebugStatus) { m_DebugEnabled = DebugStatus; };
  void setIntegrityCheck(bool IntegrityStatus) { m_CheckIntegrity = IntegrityStatus; };
  void setSyntaxChars(char m_OpenLine, char Delimiter, char m_CloseLine);
  bool modifyParam(int ParamIndex, const String &Value); // Raw edit option for Params.
  void setReadFlag(bool NewFlag) { m_ReadFlag = NewFlag; };
  void setExportFunction(void (*External_Export_func)(const String &));
  void unsetExportFunction();

  // **Get functions
  uint8_t size() { return m_ParamReadCount; };
  bool getReadFlag() { return m_ReadFlag; };
  String getParam(uint8_t n) const { return Params[n]; };
  String getCommand() { return m_Command; };
  String getRawCopy() { return m_RawCopy; };
  String getDebugIntegrityDump() const { return m_DebugIntegrityDump; };
  String toJSON(); // Export Params to JSON format.
  String glue();

  // ------------------------------------- Protected functions ------------------------------------------
protected:
  bool syntaxVerify() { return m_SyntaxTest; };
  bool Slicer(String &LineS);
  bool CSlicer(char Line[]);
  void CheckParamTypes();
  void EmptyCut();
  String getFullCommand() { return m_OpenLine + m_Command + m_DelimiterChar; };
  String getCloseLine() const { return static_cast<String>(m_DelimiterChar) + static_cast<String>(m_CloseLine); };
  const String &getError(Errors Err) { return ErrorMsgNames[static_cast<int>(Err)]; };

public:
  // ------------------------------------------ Public functions (overload and more) -------------------------
  // Overload operators
  void operator<<(const char Line[]);
  void operator<<(String &Line);
  String operator[](uint8_t n) const;

  // First and last element for range-based loops
  String *begin() const { return &Params[0]; }
  String *end() const { return &Params[m_ParamReadCount]; }

  // ------------------------------------- Constructors  -------------------------------------

  // Constructor for default syntax
  ParamPart(ParamPart &) = delete;
  ParamPart() : ParamPart(9, '<', ';', '>'){};                   // Default constructor
  ParamPart(const int &size) : ParamPart(size, '<', ';', '>'){}; // Delegated to Constructor with overloaded syntax.

  // Constructor with overloaded syntax.
  ParamPart(const int &size, char OL, char DL, char CL) // Main constructor
      : m_Max(size), Params(new String[size]), RType(new PT[size]), m_OpenLine(OL), m_DelimiterChar(DL), m_CloseLine(CL), m_DebugIntegrityDump(""), m_tmpnewLine(""), m_CheckIntegrity(CHECK_INTEGRITY_DEFAULT_STATUS),
        m_DebugEnabled(DEBUG_DEFAULT_STATUS), m_ParamReadCount(0), m_SyntaxTest(false), m_ReadFlag(false), Export_func(nullptr)
  {
    Clear();
  };

  // Destructor

  ~ParamPart()
  {
    // Free tables
    delete[] Params;
    delete[] RType;
  }

  ///////////////////////////////////////////////////// TEMPLATES - FUNCTIONS /////////////////////////////////////////////////////////////////

  /*                                                  Expanded version of integrity

 */
  struct PassF_TPack
  {
    template <typename... T>
    PassF_TPack(T...) {}
  };

  void ArgAccess(const int &Amount, uint8_t &Counter, bool &tmpITest, PT TypeExp)
  {
    if ((Counter <= Amount - 1) && (Counter <= m_Max))
    {
      if (m_DebugEnabled)
      {
        if (TypeExp == PT::Txt)
        {
          m_DebugIntegrityDump += DebugNames[static_cast<int>(PT::Txt)];
        };
        if (TypeExp == PT::Num)
        {
          m_DebugIntegrityDump += DebugNames[static_cast<int>(PT::Num)];
        };
        if (TypeExp == PT::Any)
        {
          m_DebugIntegrityDump += DebugNames[static_cast<int>(PT::Any)];
        };
      };
      if ((TypeExp == RType[Counter] || (TypeExp == PT::Any)) && (tmpITest))
      {
        tmpITest = true;
      }
      else
      {
        tmpITest = false;
      }; // Examine
      Counter++;
    };
  };

  template <typename... TPack>
  bool Integrity(TPack &&...args) // Expanded version of Integrity, It's possible now to use more than 9 arguments (and Max is set by MAX_PARAMS). Don't need specify of amount expected parameters.
  {

    uint8_t InputExpectedParams = (sizeof...(TPack));
    if (!m_CheckIntegrity)
    {
      return true;
    }; // If function is disabled just pass integrity test.
    if (InputExpectedParams != m_ParamReadCount)
    {
      if (m_DebugEnabled)
      {
        m_DebugIntegrityDump = getError(Errors::MissingExtraParameter);
      };            // Missing/Extra expected parameters
      return false; // Fail integrity test.
    };

    CheckParamTypes();
    if (m_DebugEnabled)
    {
      m_DebugIntegrityDump = "E: ";
    };

    uint8_t tmpCounter = 0; // Counter for ArgAccess function.
    bool tmpITest = true;   // Examination variable

    PassF_TPack{(ArgAccess(InputExpectedParams, tmpCounter, tmpITest, args), 1)...}; // Call access function to get arguments

    if (m_DebugEnabled)
    {
      m_DebugIntegrityDump += " / R: ";
      for (int i = 0; i < m_ParamReadCount; i++)
      {
        if (RType[i] == PT::Txt)
        {
          m_DebugIntegrityDump += DebugNames[static_cast<int>(PT::Txt)];
        };
        if (RType[i] == PT::Num)
        {
          m_DebugIntegrityDump += DebugNames[static_cast<int>(PT::Num)];
        };
        if (RType[i] == PT::Any)
        {
          m_DebugIntegrityDump += DebugNames[static_cast<int>(PT::Any)];
        };
      };
      m_DebugIntegrityDump += " MM!"; // mismatch parameters default set
    };

    if (!tmpITest)
      return false; // Found mismatch -> Fail integrity test

    m_DebugIntegrityDump = ""; // if pass integrity test just clean debug (delete default MM!)
    return true;               // Parameters match -> Pass integrity test
  };

  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  //                                  Template for overload () operator Function

  template <typename... TPack>
  bool operator()(const String &CMD, const bool &Active, TPack &&...args)
  {
    return ((Header(CMD, Active)) && Integrity(args...));
  };

  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  /*                                      Special template for Reaction Function from class member

  CALL EXAMPLE:
  For example - Reaction function is a member of MainSetter class (MainSetter::Reaction).

  typedef void (MainSetter::*ReactPTR)(ParamPart &PP);
  ReactPTR MemberReactionPointer= &MainSetter::Reaction;
  OdczytParted.Interpreter(this,MemberReactionPointer);


*/
  template <typename T>
  String Interpreter(T *M, void (T::*ptn_func_interpreter)(ParamPart &PP)) // This version of function returns only string with score.
  {
    String tmpReturn = "";
    if (syntaxVerify())
    {                                    //   (SYNTAX OK)
      (M->*ptn_func_interpreter)(*this); // Execute reaction function (callback), push pointer of this class to access from external function.
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
};
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
