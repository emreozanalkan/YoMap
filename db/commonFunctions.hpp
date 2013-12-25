#ifndef COMMONFUNCTIONS_H
#define COMMONFUNCTIONS_H
#include <sstream>
 template <typename T>
 bool getValueFromString( const std::string & value, T & result )
 {
   std::istringstream iss( value );
   return !( iss >> result ).fail();
 }
#endif // COMMONFUNCTIONS_H
