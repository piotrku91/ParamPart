#ifndef PARAMPART_H
#define PARAMPART_H

#include <Arduino.h>
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                        CLASS ParamParted  - Header file                                                                      //
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define MAX_PARAMS 9
#define CHECK_INTEGRITY_DEFAULT_STATUS true
#define DEBUG_DEFAULT_STATUS true

// Define some helpful const (example to use in program: P.Integrity(2,NUMBER,STRING))
#define STRING 1   
#define NUMBER 0

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



class ParamPart // Arduino Serial Data Splitter
{

public:
  String Command;
  String Params[MAX_PARAMS];
  String DebugIntegrityDump;

  bool RType[MAX_PARAMS];
  char DelimiterChar;
  char OpenLine;
  char CloseLine;

private:
  const uint8_t Max;
  uint8_t ParamReadedCount;
  bool SyntaxTest;
  bool ReadFlag;

protected:
  String tmpnewLine; // Workflow variable
  bool CheckIntegrity;
  bool DebugEnabled;

  // Public functions
public:
  void Clear();
  bool Header(const String& CmdName);
  bool Header(String& CmdName);
  bool GetReadFlag() const { return ReadFlag; };
  String Readed(bool RtnMsg = true, String ParamRtn = "OK", String Rtn = "artn");
  bool Slicer(String& LineS);
  bool CSlicer(char Line[]);
  String Glue();

  //Settings functions
  void SetReadFlag(bool NF) { ReadFlag = NF; };
  void SetDebugMode(bool DS) {DebugEnabled = DS;};
  void SetIntegrityCheck(bool DS) {CheckIntegrity = DS;};

  bool SyntaxVerify() { return SyntaxTest; };
  bool Integrity(uint8_t InputExpectedParams = 0, bool Type1 = 0, bool Type2 = 0, bool Type3 = 0, bool Type4 = 0, bool Type5 = 0, bool Type6 = 0, bool Type7 = 0, bool Type8 = 0, bool Type9 = 0);
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
                DebugEnabled(DEBUG_DEFAULT_STATUS), ParamReadedCount(0), SyntaxTest(false), ReadFlag(false)
  {
    Clear();
  };

  ParamPart(char OL, char DL, char CL)
      : OpenLine(OL), DelimiterChar(DL), CloseLine(CL), DebugIntegrityDump(""), tmpnewLine(""), Max(MAX_PARAMS), CheckIntegrity(CHECK_INTEGRITY_DEFAULT_STATUS),
        DebugEnabled(DEBUG_DEFAULT_STATUS), ParamReadedCount(0), SyntaxTest(false), ReadFlag(false)
  {
    Clear();
  };
};

#endif
