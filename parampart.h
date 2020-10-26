#ifndef PARAMPART_H
#define PARAMPART_H

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                        KLASA ParamParted     - Plik nagłóœkowy                                                                         //
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


#define MAX_COMMAND_LENGTH 16
#define MAX_PARAM_LENGTH 16
#define MAX_PARAMS 9
#define DEBUG_DEFAULT_STATUS false


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class ParamPart // Klasa paczki odbieranych parametrow
{

private:
  static const uint8_t Max = MAX_PARAMS;
  uint8_t ParamCount = 0;
  bool SyntaxTest=false;


// Funkcje prywatne
  void CheckParamTypes();
  void EmptyCut();
  char DebugIntegrity[32];


public:
  char Command[MAX_COMMAND_LENGTH];
  char Params[Max][MAX_PARAM_LENGTH];
  bool Type[Max];


  bool CheckIntegrity = true;
  bool DebugEnabled=DEBUG_DEFAULT_STATUS;
  
// Funkcje publiczne
  void Clear();
  bool Header(char Spr[]);
  bool Slicer(char Line[]);
  void Glue(char Line[]);

  bool SyntaxVerify() {return SyntaxTest ;};
  bool Integrity(uint8_t InputExpectedParams=0, bool Type1 = 0, bool Type2 = 0, bool Type3 = 0, bool Type4 = 0, bool Type5 = 0, bool Type6 = 0, bool Type7 = 0, bool Type8 = 0, bool Type9 = 0 );
 

 // Przeładowanie operatorów
  void operator<<(char Linia[]);
  char* operator[](uint8_t n);
   
// Konstruktory
  ParamPart() { Clear(); };
  ParamPart(char Line[]) {Clear(); Slicer(Line); };
};



#endif PARAMPART_H
