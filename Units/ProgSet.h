#ifndef PROGSET_H
#define PROGSET_H

class CProgramSettings
{
public:
   CString  m_CFGFile, m_KEYFile, m_PALFile, m_LIGFile, m_PAIFile;

   CProgramSettings();
   ~CProgramSettings();

   void LoadDefaults();
   int SaveToRegistry(char *RegPath);
   int LoadFromRegistry(char *RegPath);
};


#endif