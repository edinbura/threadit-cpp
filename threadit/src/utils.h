/*-------------------------------------------------------------------------*/
/* Copyright (C) 2021 by Ashkel Software                                   */
/* ari@ashkel.com.au                                                       */
/*                                                                         */
/* This file is part of the threadit library.                              */
/*                                                                         */
/* The threadit library is free software; you can redistribute it and/or   */
/* modify it under the terms of The Code Project Open License (CPOL) 1.02  */
/*                                                                         */
/* The threadit library is distributed in the hope that it will be useful, */
/* but WITHOUT ANY WARRANTY; without even the implied warranty of          */
/* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the CPOL       */
/* License for more details.                                               */
/*                                                                         */
/* You should have received a copy of the CPOL License along with this     */
/* software.                                                               */
/*-------------------------------------------------------------------------*/

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