#ifndef PARAMPART_H
#define PARAMPART_H
#include <Arduino.h>

/* 
Arduino Serial String Data Splitter - ParamPart
Written by Piotr Kupczyk (dajmosster@gmail.com) 
2019 - 2021
v. 3.5

Github: https://github.com/piotrku91/ParamPart/
*/

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                        CLASS ParamPart  - Header file                                                                      //
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//#define MAX_PARAMS 9
#define CHECK_INTEGRITY_DEFAULT_STATUS true
#define DEBUG_DEFAULT_STATUS true

enum class PT {Num=0,Txt=1,Any=2}; // Types to use with integrity checks. Use like PT::Num, PT::Txt, PT::Any as arguments in Integrity function.
const String DebugNames[3] = {{"[Num]"},{"[Txt]"},{"[Any]"}};

class ParamPart // Arduino String Serial Data Splitter
{

public:
  String DebugIntegrityDump;
  String RawCopy;

protected:
  const uint8_t m_Max;
  uint8_t m_ParamReadCount;
  bool m_SyntaxTest;
  bool m_ReadFlag;

  String* Params; // Params String Table (dynamic array)
  PT* RType; // Params Type Table (dynamic array)

  String tmpnewLine; // Workflow variable

  bool CheckIntegrity;
  bool DebugEnabled;

  char DelimiterChar;
  char OpenLine;
  char CloseLine;
  String Command;
  void (*Export_func)(const String &);

  // Public functions
public: 
  void Clear();
  bool Header(const String &CmdName, bool Active = true);
  bool SyntaxVerify() { return m_SyntaxTest; };
  String Interpreter(void (*ptn_func_interpreter)(ParamPart &PP));
  String ReadDone(bool RtnMsg = true, String ParamRtn = "OK", String Rtn = "artn");
  bool Slicer(String &LineS);
  bool CSlicer(char Line[]);
 

  // **Set functions
  void SetReadFlag(bool NewFlag) { m_ReadFlag = NewFlag; };
  void SetDebugMode(bool DebugStatus) { DebugEnabled = DebugStatus; };
  void SetIntegrityCheck(bool IntegrityStatus) { CheckIntegrity = IntegrityStatus; };
  void SetSyntaxChars(char OpenLine, char Delimiter, char CloseLine);
  void SetExportFunction(void (*External_Export_func)(const String &));
  void UnSetExportFunction();

 // **Get functions
  const int size() { return m_ParamReadCount; };
  bool GetReadFlag() const { return m_ReadFlag; };
  const String GetParam(uint8_t n) const { return Params[n]; };
  const String GetCommand() { return Command; };
  const String GetFullCommand() { return OpenLine + Command + DelimiterChar; };
  const String GetCloseLine() { return static_cast<String>(DelimiterChar) + static_cast<String>(CloseLine); };
  const String toJSON(); // Export Params to JSON format.
  const String Glue();


  // Protectedfunctions
protected:
  void CheckParamTypes();
  void EmptyCut();

public:
  // Overload operators
  void operator<<(const char Line[]);
  void operator<<(char Line[]);
  void operator<<(String &Line);
  String& operator[](uint8_t n);

  // First and last element for range-based loops
  String *begin() { return &Params[0]; }
  String *end() { return &Params[m_ParamReadCount]; }

  // Constructors

  // Constructor for default syntax
  ParamPart(ParamPart&) = delete;
  ParamPart() : ParamPart(9,'<', ';', '>'){}; // Delegated to Constructor with overloaded syntax.
  ParamPart(const int& size) : ParamPart(size,'<', ';', '>'){}; // Delegated to Constructor with overloaded syntax.

  // Constructor with overloaded syntax.
  ParamPart(const int& size, char OL, char DL, char CL)
      : m_Max(size), Params(new String[m_Max]), RType(new PT[m_Max]), OpenLine(OL),  DelimiterChar(DL), CloseLine(CL), DebugIntegrityDump(""), tmpnewLine(""), CheckIntegrity(CHECK_INTEGRITY_DEFAULT_STATUS),
        DebugEnabled(DEBUG_DEFAULT_STATUS), m_ParamReadCount(0), m_SyntaxTest(false), m_ReadFlag(false), Export_func(nullptr)
  {
    Clear();
  };

  // Destructor

  ~ParamPart() {
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
      if (DebugEnabled)
      {
        if (TypeExp==PT::Txt) {
        DebugIntegrityDump += DebugNames[static_cast<int>(PT::Txt)];
        };
        if (TypeExp==PT::Num) {
        DebugIntegrityDump += DebugNames[static_cast<int>(PT::Num)];
        };
        if (TypeExp==PT::Any) {
        DebugIntegrityDump += DebugNames[static_cast<int>(PT::Any)];
        };
      };
      if ((TypeExp == RType[Counter] || (TypeExp==PT::Any)) && (tmpITest))
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
  bool Integrity(TPack &&... args) // Expanded version of Integrity, It's possible now to use more than 9 arguments (and Max is set by MAX_PARAMS). Don't need specify of amount expected parameters.
  {

    uint8_t InputExpectedParams = (sizeof...(TPack));
    if (!CheckIntegrity)
    {
      return true;
    }; // If function is disabled just pass integrity test.
    if (InputExpectedParams != m_ParamReadCount)
    {
      if (DebugEnabled)
      {
        DebugIntegrityDump = "MEP";
      };            // Missing/Extra expected parameters
      return false; // Fail integrity test.
    };

    CheckParamTypes();
    if (DebugEnabled)
    {
      DebugIntegrityDump = "E: ";
    };

    uint8_t tmpCounter = 0; // Counter for ArgAccess function.
    bool tmpITest = true;   // Examination variable

    PassF_TPack{(ArgAccess(InputExpectedParams, tmpCounter, tmpITest, args), 1)...}; // Call access function to get arguments

    if (DebugEnabled)
    {
      DebugIntegrityDump += " / R: ";
      for (int i = 0; i < m_ParamReadCount; i++)
      {
             if (RType[i]==PT::Txt) {
        DebugIntegrityDump += DebugNames[static_cast<int>(PT::Txt)];
        };
        if (RType[i]==PT::Num) {
        DebugIntegrityDump += DebugNames[static_cast<int>(PT::Num)];
        };
        if (RType[i]==PT::Any) {
        DebugIntegrityDump += DebugNames[static_cast<int>(PT::Any)];
        };
      };
      DebugIntegrityDump += " MM!"; // mismatch parameters default set
    };

    if (!tmpITest)
      return false; // Found mismatch -> Fail integrity test

    DebugIntegrityDump = ""; // if pass integrity test just clean debug (delete default MM!)
    return true;             // Parameters match -> Pass integrity test
  };

  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  //                                  Template for overload () operator Function

  template <typename... TPack>
  bool operator()(const String &CMD, const bool &Active, TPack &&... args)
  {
    return ((Header(CMD, Active)) && EIntegrity(args...));
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
    if (SyntaxVerify())
    {                                    //   (SYNTAX OK)
      (M->*ptn_func_interpreter)(*this); // Execute reaction function (callback), push pointer of this class to access from external function.
      if (!(Export_func == nullptr))
        UnSetExportFunction();
      if ((DebugEnabled) && (DebugIntegrityDump != ""))
        tmpReturn = DebugIntegrityDump; // Debug Integrity error print (if is ok, nothing to print)
      if ((DebugEnabled) && (!GetReadFlag() && (DebugIntegrityDump == "")))
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
};
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#endif
