#ifndef ERROR_H
#define ERROR_H

#include "os_specific_libs.h"
#include <string>

class Error {
public:
  static void Check (const std::string& msg);
};
#endif