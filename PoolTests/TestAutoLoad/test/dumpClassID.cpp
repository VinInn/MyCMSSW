#include "FWCore/PluginManager/interface/PluginManager.h"
#include "FWCore/PluginManager/interface/PluginCapabilities.h"
#include "StorageSvc/DbReflex.h"

namespace pool {  void genMD5(const std::string& s, void* code);  }


int main() {

  edmplugin::PluginManager*db =  edmplugin::PluginManager::get();
  
  typedef edmplugin::PluginManager::CategoryToInfos CatToInfos;
  
  CatToInfos::const_iterator itFound = db->categoryToInfos().find("Capability");
  
  if(itFound == db->categoryToInfos().end()) {
    return;
  }
  std::string lastClass;
  const std::string cPrefix("LCGReflex/");

  for (edmplugin::PluginManager::Infos::const_iterator itInfo =
	 itFound->second.begin(),
	 itInfoEnd = itFound->second.end(); 
       itInfo != itInfoEnd; ++itInfo)
    {
      if (lastClass == itInfo->name_) {
	continue;
      }
      
      lastClass = itInfo->name_; 
      edmplugin::PluginCapabilities::get()->load(lastClass);
      const ROOT::Reflex::Type type=ROOT::Reflex::Type::ByName(className);

      std::string s;
      ROOT::Reflex::PropertyList pl = type.Properties();
      if ( pl.HasProperty("ClassID") )  {
	s = pl.PropertyAsString("ClassID");
      }
      else  {
	char buff[20];
	pool::genMD5(type.Name(ROOT::Reflex::SCOPED),buff);
	s = ((pool::Guid*)buff)->toString();
      }
      cout << s << " " << lastClass << std::endl;

    }
  
  return 0;
}
