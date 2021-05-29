#!/bin/bash
clear
echo "ParamPart Arduino version -> port to -> PC Version"
headerfile="${PWD}/src/parampart.h"
cppfile="${PWD}/src/parampart.cpp"

function replace ()
{
sed  -i -e "s/$1/$2/g" $headerfile
sed  -i -e "s/$1/$2/g" $cppfile
echo "Replacing " $1 " -> " $2
};

replace '#include <Arduino.h>' '#include <string>'
replace '#include "parampart.h"' '#include "parampart.hpp"'
replace 'String' 'std::string'
replace 'return static_cast<std::string>(m_DelimiterChar) + static_cast<std::string>(m_CloseLine);' 'return std::string(m_DelimiterChar,m_CloseLine); '
replace 'if ((LineS.indexOf(m_OpenLine) == -1) || (LineS.indexOf(DC) == -1) || (LineS.indexOf(m_CloseLine) == -1))' 'if ((LineS.find(m_OpenLine) == std::string::npos) || (LineS.find(DC) == std::string::npos) || (LineS.find(m_CloseLine) == std::string::npos))'
replace 'indexOf' 'find'
replace 'substring' 'substr'
replace 'remove' 'erase'
replace 'int TmpType = Params\[i\].toInt\x28\x29;' 'int TmpType; if (!(sscanf(Params[i].c_str(), "%d", \&TmpType))){ TmpType = 0;};'
replace 'static_cast<std::string>(m_ParamReadCount)' 'std::to_string(m_ParamReadCount)'
replace 'm_Command = LineS.substr(1, NextDel);' 'm_Command = LineS.substr(1, NextDel-1);'
replace 'Arduino Serial std::string Data Splitter - ParamPart' 'Arduino Serial String Data Splitter - ParamPart (PC Port Version)'
echo "Deleting useless files"
rm -rf src/parampart_ex.h
rm -rf keywords.txt
rm -rf library.properties
rm -rf examples
echo "rename hpp file"
mv src/parampart.h src/parampart.hpp





