//
// Copyright (c) 2016 Advanced Micro Devices, Inc. All rights reserved.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//

// This code used to reside directly in SuCountedPtr.h. I had to pull it out and put it in its own include file because the
// latest version of tolua++ choked on parsing the header file with all the #ifdefs and #undefs and stuff.

#if defined(WIN32) || defined(MACOSX)

   // tolua++ defines its own version of TEMPLATE_BIND which will generate compiler warnings for this file
   // because it only has one argument.  In order to fix the warnings, I'm undef'ing their broken version and
   // making my own
   #ifdef TOLUA_TEMPLATE_BIND
   #undef TOLUA_TEMPLATE_BIND
   #endif

   #define TOLUA_TEMPLATE_BIND(x,...)

#elif defined (__SYMBIAN32__)

   #ifdef TOLUA_TEMPLATE_BIND
   #undef TOLUA_TEMPLATE_BIND
   #endif

   #define TOLUA_TEMPLATE_BIND(x...)

#elif defined (_LINUX)

   #ifdef TOLUA_TEMPLATE_BIND
   #undef TOLUA_TEMPLATE_BIND
   #endif

   #define TOLUA_TEMPLATE_BIND(x...)

#else

   #ifndef TOLUA_TEMPLATE_BIND
   #define TOLUA_TEMPLATE_BIND(x)
   #endif

#endif
