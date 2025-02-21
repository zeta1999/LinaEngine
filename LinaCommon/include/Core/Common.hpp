/* 
This file is a part of: Lina Engine
https://github.com/inanevin/LinaEngine

Author: Inan Evin
http://www.inanevin.com

Copyright (c) [2018-2020] [Inan Evin]

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

/* 
Class: Common

Common macros are defined here.

Timestamp: 4/7/2019 3:29:18 PM
*/

#pragma once

#ifndef Common_HPP
#define Common_HPP

/****************************************** OPTIONS ******************************************/

#ifdef LINA_COMPILER_MSVC
#define FORCEINLINE __forceinline
#elif defined(LINA_COMPILER_GCC) || defined(LINA_COMPILER_CLANG)
#define FORCEINLINE inline __attribute__ ((always_inline))
#else
#define FORCEINLINE inline
#endif

#define BIT(x) (1 << x)
#define INVALID_VALUE 0xFFFFFFFF
#define ARRAY_SIZE_IN_ELEMENTS(a) (sizeof(a)/sizeof(a[0]))
#define LINA_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)

#define NULL_COPY_AND_ASSIGN(T) \
	T(const T& other) {(void)other;} \
	void operator=(const T& other) { (void)other; }

#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
  TypeName(const TypeName&) = delete;      \
  void operator=(const TypeName&) = delete;

#define DISALLOW_COPY_ASSIGN_MOVE(TypeName) \
  TypeName(const TypeName&) = delete;      \
  void operator=(const TypeName&) = delete; \
	TypeName(TypeName&&) = delete; \
  TypeName& operator=(TypeName&&) = delete; 

#define DISALLOW_COPY_ASSIGN_NEW(TypeName) \
  TypeName(const TypeName&) = delete;      \
  void operator=(const TypeName&) = delete; \
  void* operator new(std::size_t) = delete;

#define DISALLOW_COPY_ASSIGN_NEW_MOVE(TypeName) \
  TypeName(const TypeName&) = delete;      \
  TypeName(TypeName&&) = delete; \
  TypeName& operator=(TypeName&&) = delete; \
  void operator=(const TypeName&) = delete; \
  void* operator new(std::size_t) = delete;

#endif