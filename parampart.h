#ifndef PARAMPART_H
#define PARAMPART_H

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                        CLASS ParamParted  - Header file                                                                      //
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define MAX_COMMAND_LENGTH 16
#define MAX_PARAM_LENGTH 16
#define MAX_PARAMS 9
#define CHECK_INTEGRITY_DEFAULT_STATUS true
#define DEBUG_INTEGRITY_DEFAULT_STATUS true
#define DEBUG_DEFAULT_STATUS true

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define STRING 1
#define NUMBER 0


class ParamPart // Klasa paczki odbieranych parametrow
{

private:
  static const uint8_t Max = MAX_PARAMS;
  uint8_t ParamReadedCount = 0;
  bool SyntaxTest = false;
  bool ReadFlag=false;

  // Private functions
  void CheckParamTypes();
  void EmptyCut();

public:
  char Command[MAX_COMMAND_LENGTH];
  char Params[Max][MAX_PARAM_LENGTH];
  bool RType[Max];
  String DebugIntegrityDump;

  bool CheckIntegrity = CHECK_INTEGRITY_DEFAULT_STATUS;
  bool DebugIntegrityEnabled = DEBUG_INTEGRITY_DEFAULT_STATUS;
  bool DebugEnabled = DEBUG_DEFAULT_STATUS;
  char DelimiterChar = ';';
  char OpenLine = '<';
  char CloseLine = '>';

  // Public functions
  void Clear();
  bool Header(char Spr[]);
  bool GetReadFlag(){return ReadFlag;};
  void SetReadFlag(bool NF){ReadFlag=NF;};
  void Readed(){ReadFlag=true;};
  bool Slicer(char Line[]);
  void Glue(char Line[]);

  bool SyntaxVerify() { return SyntaxTest; };
  bool Integrity(uint8_t InputExpectedParams = 0, bool Type1 = 0, bool Type2 = 0, bool Type3 = 0, bool Type4 = 0, bool Type5 = 0, bool Type6 = 0, bool Type7 = 0, bool Type8 = 0, bool Type9 = 0);

  // Overload operators
  void operator<<(char Line[]);
  void operator<<(String Line);
  char *operator[](uint8_t n);

  // Constructors
  ParamPart() { Clear(); };
  ParamPart(char OL, char DL, char CL):  OpenLine(OL),DelimiterChar(DL),CloseLine(CL) { Clear(); };
  ParamPart(char Line[])
  {
    Clear();
    Slicer(Line);
  };
};

#endif PARAMPART_H
