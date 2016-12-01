/**
 * @file   TMagnetConfig.h
 * @brief  magnet config class
 *
 * @date   Created       : 2016-11-28 12:22:21 JST
 *         Last Modified : 2016-11-28 16:58:12 JST (kawase)
 * @author KAWASE Shoichiro <kawase@aees.kyushu-u.ac.jp>
 *
 *    (C) 2016 KAWASE Shoichiro
 */

#ifndef INCLUDE_GUARD_UUID_2557BFA3_A82D_4FAF_A8DD_7F8336CE791C
#define INCLUDE_GUARD_UUID_2557BFA3_A82D_4FAF_A8DD_7F8336CE791C

#include <string>

namespace trace {
   class TMagnetConfig;
}

class trace::TMagnetConfig {
public:
   static TMagnetConfig* GetInstance();
   void LoadFile(const char* filename);

   const char* GetFieldFile() const {return fFieldFile.c_str();}
   void SetFieldFile(const char* file) {fFieldFile = file;}
   float GetCentralField() const {return fCentralField;}
   void SetCentralField(float field) {fCentralField = field;}
   bool CentralFieldIsDefined() const;
   bool IsGood() const {return fIsGood;}

private:
   TMagnetConfig();
   TMagnetConfig(const TMagnetConfig&);            // undefined
   TMagnetConfig& operator=(const TMagnetConfig&); // undefined

   std::string fFieldFile;
   float fCentralField;
   bool fCentralFieldIsDefined;
   bool fIsGood;
};

#endif // INCLUDE_GUARD_UUID_2557BFA3_A82D_4FAF_A8DD_7F8336CE791C
