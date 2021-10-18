#pragma once

#if !defined (UTILS_H)
#define UTILS_H

#include <sstream>
#include <windows.h>

template <class T>

inline std::string to_string (const T& t)
{
  std::stringstream ss;

	ss << t;
  return ss.str();
} // to_string

template <class T>

inline std::string to_str (const T& t)
{
  std::stringstream ss;

	ss << t;
  return ss.str(); 
} // to_str



#endif //  !defined (UTILS_H)