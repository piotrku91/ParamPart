#ifndef PARAMPART_H
#define PARAMPART_H

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                        CLASS ParamParted  - Header file                                                                      //
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define MAX_COMMAND_LENGTH 16
#define MAX_PARAMS 9
#define CHECK_INTEGRITY_DEFAULT_STATUS true
#define DEBUG_INTEGRITY_DEFAULT_STATUS true
#define DEBUG_DEFAULT_STATUS true

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define STRING 1
#define NUMBER 0

class ParamPart // Klasa paczki odbieranych parametrow
{

public:
  String Command;
  String Params[MAX_PARAMS];
  String DebugIntegrityDump;

  bool RType[MAX_PARAMS];
  bool CheckIntegrity;
  bool DebugEnabled;

  char DelimiterChar;
  char OpenLine;
  char CloseLine;

private:
  const uint8_t Max = MAX_PARAMS;
  uint8_t ParamReadedCount = 0;
  bool SyntaxTest = false;
  bool ReadFlag = false;

  // Public functions
public:
  void Clear();
  bool Header(String CmdName);
  bool GetReadFlag() { return ReadFlag; };
  void SetReadFlag(bool NF) { ReadFlag = NF; };
  void Readed() { ReadFlag = true; };
  bool Slicer(String *LineS);
  bool CSlicer(char Line[]);
  String Glue();

  bool SyntaxVerify() { return SyntaxTest; };
  bool Integrity(uint8_t InputExpectedParams = 0, bool Type1 = 0, bool Type2 = 0, bool Type3 = 0, bool Type4 = 0, bool Type5 = 0, bool Type6 = 0, bool Type7 = 0, bool Type8 = 0, bool Type9 = 0);

  // Private functions

private:
  void CheckParamTypes();
  void EmptyCut();

public:
  // Overload operators
  //void operator<<(char Line[]);
  void operator<<(String Line);
  const String ParamPart::operator[](uint8_t n);

  // Constructors
  ParamPart() : OpenLine('<'), DelimiterChar(';'), CloseLine('>'), DebugIntegrityDump(""), Max(MAX_PARAMS), CheckIntegrity(CHECK_INTEGRITY_DEFAULT_STATUS),
       DebugEnabled(DEBUG_DEFAULT_STATUS), ParamReadedCount(0), SyntaxTest(false), ReadFlag(false)
       {
Clear();
       };

  ParamPart(char OL, char DL, char CL)
      : OpenLine(OL), DelimiterChar(DL), CloseLine(CL), DebugIntegrityDump(""), Max(MAX_PARAMS), CheckIntegrity(CHECK_INTEGRITY_DEFAULT_STATUS),
       DebugEnabled(DEBUG_DEFAULT_STATUS), ParamReadedCount(0), SyntaxTest(false), ReadFlag(false)
  {
    Clear();
  };
};

#endif
