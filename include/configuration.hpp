#ifndef CONFIGURATION_HPP_DEFINED
#define CONFIGURATION_HPP_DEFINED

#include <iostream>
#include <fstream>
#include "picojson.hpp"
#include <string>
#include <iterator>

#define CONFIG_FILE_PATH "gothic.ini"

using json = picojson::value;

class Configuration
{
   json config;
public:

   bool Init()
   {
      bool initialised = false;
      if(!initialised)
      {
         std::ifstream json_file;
         json_file.open(CONFIG_FILE_PATH);
         std::istream_iterator<char> iter(json_file);
         std::string err;
         iter = picojson::parse(config, iter, std::istream_iterator<char>(), &err);
         if(!err.empty()) {
             std::cerr << err << std::endl;
         }
         initialised = true;
      }
      return initialised;
   }

   static Configuration& Get()
   {
      static Configuration instance;
      instance.Init();
      return instance;
   }

   const std::string GetElement(std::string key) const
   {
       return config.get(key).to_str();
   }
};

#define CONF_GET(x) Configuration::Get().GetElement(x).c_str()

#endif //CONFIGURATION_HPP_DEFINED
