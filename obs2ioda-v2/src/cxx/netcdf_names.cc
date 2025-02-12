 #include "netcdf_names.h"
 #include <algorithm>

std::string trim(
 const std::string &name
) {
     std::string strippedName = name;

     strippedName.erase(
             std::remove_if(
                     strippedName.begin(),
                     strippedName.end(),
                     [](unsigned char x) { return std::isspace(x); }
             ),
             strippedName.end()
     );
     return strippedName;
 }