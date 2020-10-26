#include "parampart.h"

/* 
Arduino Serial Data Splitter - ParamPart
Created by Piotr Kupczyk (dajmosster@gmail.com) 
2019 - 2020
v. 2.4
*/ 

////////////////////////////////////////////////////////////////////////////////////////////////////////////

void ParamPart::EmptyCut()
{
  int i = ParamCount;
  while (i < Max)
  {
    strcpy(Params[i], NULL);
    i++;
  };
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////

void ParamPart::Clear()
{
  ParamCount = 0;
  strcpy(Command, NULL);
  EmptyCut();
  SyntaxTest = false;
};

bool ParamPart::Header(char Spr[])
{
  if ((strcmp(Command, Spr)) == 0)
  {
    return 1;
  };
  return 0;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool ParamPart::Slicer(char Linia[])
{
  Clear();
  int i = 0;
  char *strtokIndx;
  if (strchr(Linia, ';') == NULL)
  {
    SyntaxTest = false;
    Clear();
    return false;
  };

  strtokIndx = strtok(Linia, ";");
  strcpy(Command, strtokIndx);

  for (; ((strtokIndx != NULL) && (i <= Max)); i++)
  {
    strtokIndx = strtok(NULL, ";");
    strcpy(Params[i], strtokIndx);
  };

  i = i - 2; // Usuń znak > i nastepny zwiekszony petla.
  ParamCount = i;
  EmptyCut();
  SyntaxTest = true;
  return true;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void ParamPart::CheckParamTypes()
{
  for (int i = 0; i < Max; i++)
  {
    Type[i] = 0;
  }; //Wyzeruj Typey

  for (int i = 0; i < Max; i++)
  {

    for (int z = 0; (z < MAX_PARAM_LENGTH && Params[i][z] != NULL); z++)
    {
      // Sprawdza po znakach Type zapisany w parametrze. Gdy Type zmieniony juz na "TEKST" nie zamienia juz Type na "LICZBA" (Umożliwia zapis liczb w tekscie).
      if (isDigit(Params[i][z]))
      {
        if (Type[i] == 1)
          Type[i] = 1; // Type = String (TEKST)
        else
          Type[i] = 0; // Tyoe = Integer (LICZBA)
      }
      else
      {
        Type[i] = 1;
      };
    };
  };
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool ParamPart::Integrity(uint8_t InputExpectedParams, bool Type1, bool Type2, bool Type3, bool Type4, bool Type5, bool Type6, bool Type7, bool Type8, bool Type9)
{
  if ((!CheckIntegrity) or (InputExpectedParams == 0))
    return true; // Jeśli wyłączona lub brak parametrów zwraca zawsze prawde.

  CheckParamTypes();

  if (DebugEnabled)
  {
    // Typy z wejścia (input)
    DebugIntegrity[0] = 'I';
    DebugIntegrity[1] = ':';
    DebugIntegrity[2] = Type1;
    DebugIntegrity[3] = Type2;
    DebugIntegrity[4] = Type3;
    DebugIntegrity[5] = Type4;
    DebugIntegrity[6] = Type5;
    DebugIntegrity[7] = Type6;
    DebugIntegrity[8] = Type7;
    DebugIntegrity[9] = Type8;
    DebugIntegrity[10] = Type9;

    DebugIntegrity[11] = '/';

    // Typy spodziewane (expected)
    DebugIntegrity[11] = 'E';
    DebugIntegrity[12] = ':';

    for (int i = 13; i <= 0; i++)
    {
      DebugIntegrity[i] = Type[i - 13];
    };
  };

  ////////////////////////
  if (InputExpectedParams >= 1)
  {
    if (Type1 != Type[0])
      return false;
  }
  if (InputExpectedParams >= 2)
  {
    if (Type2 != Type[1])
      return false;
  }
  if (InputExpectedParams >= 3)
  {
    if (Type3 != Type[2])
      return false;
  }
  if (InputExpectedParams >= 4)
  {
    if (Type4 != Type[3])
      return false;
  }
  if (InputExpectedParams >= 5)
  {
    if (Type5 != Type[4])
      return false;
  }
  if (InputExpectedParams >= 6)
  {
    if (Type6 != Type[5])
      return false;
  }
  if (InputExpectedParams >= 7)
  {
    if (Type7 != Type[6])
      return false;
  }
  if (InputExpectedParams >= 8)
  {
    if (Type8 != Type[7])
      return false;
  }
  if (InputExpectedParams >= 9)
  {
    if (Type9 != Type[8])
      return false;
  }

  return true;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////

void ParamPart::Glue(char Line[])
{
  //   Clear();
  String Glue_hand;
  String tmpstr;

  Glue_hand = Command; //Zrzucanie komendy
  Glue_hand += ';';

  for (int i; i < Max; i++)
  {
    tmpstr = Params[i]; //Zrzucanie parametrów
    if (Params[i][0] != NULL)
    {

      Glue_hand += tmpstr + ';';
    };
  };

  Glue_hand += '>'; // Zakończenie komendy
  Line = strcpy(Line, Glue_hand.c_str());
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////

void ParamPart::operator<<(char Line[])
{
  Slicer(Line);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////

char *ParamPart::operator[](uint8_t n)
{
  return Params[n];
}
