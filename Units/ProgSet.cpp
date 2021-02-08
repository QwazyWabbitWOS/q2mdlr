#include "stdafx.h"

#include "RegClass.h"
#include "ProgSet.h"

CProgramSettings::CProgramSettings()
{
   LoadDefaults();
}

CProgramSettings::~CProgramSettings()
{
}


void CProgramSettings::LoadDefaults()
{
   m_CFGFile = "VCMDL.CFG";
   m_KEYFile = "VCMDL.KEY";
   m_PALFile = "VCMDL.PAL";
   m_PAIFile = "VCMDL.PAI";
   m_LIGFile = "VCMDL.LIG";
}

int CProgramSettings::SaveToRegistry(char *RegPath)
{
   CRegistry   Reg;

   if (!Reg.OpenKey(RegPath))
      return 0;
   
   Reg.WriteString("CFGFile", m_CFGFile);
   Reg.WriteString("KeyFile", m_KEYFile);
   Reg.WriteString("PalFile", m_PALFile);
   Reg.WriteString("LightFile", m_LIGFile);
   Reg.WriteString("PaintFile", m_PAIFile);

   Reg.CloseKey();

   return 1;
}

int CProgramSettings::LoadFromRegistry(char *RegPath)
{
   CRegistry   Reg;

   if (!Reg.OpenKey(RegPath))
      return 0;

   Reg.ReadString("CFGFile", m_CFGFile);
   Reg.ReadString("KeyFile", m_KEYFile);
   Reg.ReadString("PalFile", m_PALFile);
   Reg.ReadString("LightFile", m_LIGFile);
   Reg.ReadString("PaintFile", m_PAIFile);

   Reg.CloseKey();

   return 1;
}

