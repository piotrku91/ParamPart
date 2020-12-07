#ifndef PARAMPART_H
#define PARAMPART_H

#include <string>
#include <cstring>

#define QSTRING_SERVICE 1 // Enable / Disable Qt Enviroment type QString instead of std::string in access methods

#if QSTRING_SERVICE
#include <QString>
#endif


/* 
Arduino Serial String Data Splitter - ParamPart_pcs (PC SIDE PORT)
Written by Piotr Kupczyk (dajmosster@gmail.com) 
2019 - 2020
v. 3.3.6

Port version to use on PC side.

Github: https://github.com/piotrku91/ParamPart/
*/

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                        CLASS ParamPart_PCS  - Header file                                                                      //
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define MAX_PARAMS 9
#define CHECK_INTEGRITY_DEFAULT_STATUS true
#define DEBUG_DEFAULT_STATUS true

// Define some helpful const (example to use in program: P.Integrity(2,NUMBER,STRING))
#define STRING 1   
#define NUMBER 0

//

class TManager;


class ParamPart // Arduino std::string Serial Data Splitter
{

public:
  std::string DebugIntegrityDump;


private:
  const uint8_t Max;
  uint8_t ParamReadCount;
  bool SyntaxTest; 
  bool ReadFlag;
  

protected:
  std::string Params[MAX_PARAMS];
  std::string tmpnewLine; // Workflow variable
  bool CheckIntegrity;
  bool DebugEnabled;
  bool RType[MAX_PARAMS];
  char OpenLine;
  char DelimiterChar;
  char CloseLine;
  std::string Command;


  // Public functions
public:
  void Clear();
  bool Header(const std::string& CmdName);
  bool Header(std::string& CmdName);
  const std::string GetCommand() {return Command;};
  const std::string GetFullCommand() {return OpenLine+Command+DelimiterChar;};
  const std::string  GetCloseLine() {return std::string(DelimiterChar,CloseLine);};
  bool UseAsHeader(const std::string& CmdName,uint8_t ParamIndex=0);
  std::string ReadDone(bool RtnMsg = true, std::string ParamRtn = "OK", std::string Rtn = "artn");
  bool Slicer(std::string& LineS);
  bool CSlicer(char Line[]);
  std::string Glue();

  //Settings functions
  void SetReadFlag(bool NewFlag) { ReadFlag = NewFlag; };
  void SetDebugMode(bool DebugStatus) {DebugEnabled = DebugStatus;};
  void SetIntegrityCheck(bool IntegrityStatus) {CheckIntegrity = IntegrityStatus;};
  void SetSyntaxChars(char OpenLine, char Delimiter, char CloseLine) {this->OpenLine=OpenLine; this->DelimiterChar=Delimiter;this->CloseLine=CloseLine;};

  bool GetReadFlag() const { return ReadFlag; };
  const std::string GetParam(uint8_t n) const {return Params[n];};
  bool Integrity(uint8_t InputExpectedParams = 0, bool Type1 = 0, bool Type2 = 0, bool Type3 = 0, bool Type4 = 0, bool Type5 = 0, bool Type6 = 0, bool Type7 = 0, bool Type8 = 0, bool Type9 = 0);
  bool SyntaxVerify() { return SyntaxTest; };


  std::string Interpreter(void (*ptn_func_interpreter)(ParamPart &PP)); //for Global or static function reaction function
 // std::string Interpreter(TManager *M, void (TManager::*ptn_func_interpreter)(ParamPart &PP)); //for class member reaction function
  // Private functions

private:
  void CheckParamTypes();
  void EmptyCut();

public:
  // Overload operators
  void operator<<(const char Line[]);
  void operator<<(char Line[]);
  void operator<<(const std::string& Line);


#if QSTRING_SERVICE

  void operator<<(const QString& Line);
  const QString operator[](uint8_t n);

#else

  const std::string operator[](uint8_t n);


#endif

  // Constructors

  ParamPart() : DebugIntegrityDump(""),Max(MAX_PARAMS),ParamReadCount(0),SyntaxTest(false), ReadFlag(false), tmpnewLine(""), CheckIntegrity(CHECK_INTEGRITY_DEFAULT_STATUS), DebugEnabled(DEBUG_DEFAULT_STATUS),OpenLine('<'), DelimiterChar(';'), CloseLine('>')

  {
    Clear();
  };//

  ParamPart(char OL, char DL, char CL)
      : DebugIntegrityDump(""),Max(MAX_PARAMS),ParamReadCount(0),SyntaxTest(false), ReadFlag(false), tmpnewLine(""), CheckIntegrity(CHECK_INTEGRITY_DEFAULT_STATUS), DebugEnabled(DEBUG_DEFAULT_STATUS),OpenLine(OL), DelimiterChar(DL), CloseLine(CL)
  {
    Clear();
  };

///////////////////////////////////////////////////// TEMPLATES - FUNCTIONS /////////////////////////////////////////////////////////////////
// Expanded version of integrity
struct PassF_TPack {template<typename... T> PassF_TPack(T...) {}};

void ArgAccess(const int& Amount, uint8_t& Counter, bool& tmpITest, uint8_t TypeExp)
{
    if ((Counter <= Amount-1) && (Counter <= Max))
    {
          if (DebugEnabled){DebugIntegrityDump+=std::to_string(TypeExp);}; 
          if (TypeExp==RType[Counter] && (tmpITest)) {tmpITest=true;} else {tmpITest=false;}; // Examine
        Counter++;
    };
};

template<typename... TPack> bool EIntegrity(TPack&&... args) // Expanded version of Integrity, It's possible now to use more than 9 arguments (and Max is set by MAX_PARAMS). Don't need specify of amount expected parameters.
{

 uint8_t InputExpectedParams=(sizeof...(TPack));
  if (!CheckIntegrity)  {return true; };// If function is disabled just pass integrity test.
  if (InputExpectedParams != ParamReadCount)
  {
    if (DebugEnabled) {DebugIntegrityDump = "MEP";}; // Missing/Extra expected parameters
    return false;                 // Fail integrity test.
  };

  CheckParamTypes();
  if (DebugEnabled){DebugIntegrityDump = "E: "; } ;


uint8_t tmpCounter=0; // Counter for ArgAccess function.
bool tmpITest=true; // Examination variable

PassF_TPack{ (ArgAccess(InputExpectedParams,tmpCounter,tmpITest,args),1)... }; // Call access function to get arguments

  if (DebugEnabled)
  {
    DebugIntegrityDump += " / R: ";
    for (int i = 0; i < Max; i++)
    {    DebugIntegrityDump += RType[i]; };
    DebugIntegrityDump += " MM!"; // mismatch parameters default set
  };

if (!tmpITest) return false; // Found mismatch -> Fail integrity test

DebugIntegrityDump = ""; // if pass integrity test just clean debug (delete default MM!)
 return true; // Parameters match -> Pass integrity test
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////Version for class member reaction function///////////////////////////

// Example execution from TManager class:
/*

    typedef void (TManager::*ReaPTR)(ParamPart &PP);
    ReaPTR MemberReactionPointer= &TManager::Reaction;
    m_ParamPart_ptr->Interpreter(this,MemberReactionPointer);

*/
template <typename T>
std::string Interpreter(T *M,void (T::*ptn_func_interpreter)(ParamPart &PP)) // This overloaded version for cooperate with class member function.
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

};

#endif
