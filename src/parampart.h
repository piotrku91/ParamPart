#ifndef PARAMPART_H
#define PARAMPART_H
#include <Arduino.h>

/* 
Arduino Serial String Data Splitter - ParamPart
Written by Piotr Kupczyk (dajmosster@gmail.com) 
2019 - 2020
v. 3.3.4

Github: https://github.com/piotrku91/ParamPart/
*/

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                        CLASS ParamPart  - Header file                                                                      //
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define MAX_PARAMS 9
#define CHECK_INTEGRITY_DEFAULT_STATUS true
#define DEBUG_DEFAULT_STATUS true

// Define some helpful const (example to use in program: P.Integrity(2,NUMBER,STRING))
#define STRING 1   
#define NUMBER 0


class ParamPart // Arduino String Serial Data Splitter
{

public:
  String Command;
  String DebugIntegrityDump;

 

private:
  const uint8_t Max;
  uint8_t ParamReadCount;
  bool SyntaxTest; 
  bool ReadFlag;
  

protected:
  String Params[MAX_PARAMS];
  String tmpnewLine; // Workflow variable
  bool CheckIntegrity;
  bool DebugEnabled;
  bool RType[MAX_PARAMS];
  char DelimiterChar;
  char OpenLine;
  char CloseLine;

  // Public functions
public:
  void Clear();
  bool Header(const String& CmdName);
  bool Header(String& CmdName);
  String ReadDone(bool RtnMsg = true, String ParamRtn = "OK", String Rtn = "artn");
  bool Slicer(String& LineS);
  bool CSlicer(char Line[]);
  String Glue();

  //Settings functions
  void SetReadFlag(bool NewFlag) { ReadFlag = NewFlag; };
  void SetDebugMode(bool DebugStatus) {DebugEnabled = DebugStatus;};
  void SetIntegrityCheck(bool IntegrityStatus) {CheckIntegrity = IntegrityStatus;};
  void SetSyntaxChars(char OpenLine, char Delimiter, char CloseLine) {this->OpenLine=OpenLine; this->DelimiterChar=DelimiterChar;this->CloseLine=CloseLine;};

  bool GetReadFlag() const { return ReadFlag; };
  const String GetParam(uint8_t n) const {return Params[n];};
  bool Integrity(uint8_t InputExpectedParams = 0, bool Type1 = 0, bool Type2 = 0, bool Type3 = 0, bool Type4 = 0, bool Type5 = 0, bool Type6 = 0, bool Type7 = 0, bool Type8 = 0, bool Type9 = 0);
  bool SyntaxVerify() { return SyntaxTest; };

  String Interpreter(void (*ptn_func_interpreter)(ParamPart &PP));
  // Private functions

private:
  void CheckParamTypes();
  void EmptyCut();

public:
  // Overload operators
  void operator<<(const char Line[]);
  void operator<<(char Line[]);
  void operator<<(String& Line);
  const String operator[](uint8_t n);

  // Constructors
  ParamPart() : OpenLine('<'), DelimiterChar(';'), CloseLine('>'), DebugIntegrityDump(""), tmpnewLine(""), Max(MAX_PARAMS), CheckIntegrity(CHECK_INTEGRITY_DEFAULT_STATUS),
                DebugEnabled(DEBUG_DEFAULT_STATUS), ParamReadCount(0), SyntaxTest(false), ReadFlag(false)
  {
    Clear();
  };

  ParamPart(char OL, char DL, char CL)
      : OpenLine(OL), DelimiterChar(DL), CloseLine(CL), DebugIntegrityDump(""), tmpnewLine(""), Max(MAX_PARAMS), CheckIntegrity(CHECK_INTEGRITY_DEFAULT_STATUS),
        DebugEnabled(DEBUG_DEFAULT_STATUS), ParamReadCount(0), SyntaxTest(false), ReadFlag(false)
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
          if (DebugEnabled){DebugIntegrityDump+=static_cast<String>(TypeExp);}; 
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


};

#endif
