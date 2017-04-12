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

#ifndef _SU_RAND_H
#define _SU_RAND_H


SU_DLL void init_genrand(uint32 s);

/* generates a random number on [0,0xffffffff]-interval */
SU_DLL uint32 genrand_int32(void);

/* generates a random number on [0,0x7fffffff]-interval */
int32 genrand_int31(void);

/* generates a random number on [0,1]-real-interval */
SU_DLL float64 genrand_real1(void);

/* generates a random number on [0,1)-real-interval */
SU_DLL float64 genrand_real2(void);

/* generates a random number on (0,1)-real-interval */
SU_DLL float64 genrand_real3(void);

/* generates a random number on [0,1) with 53-bit resolution*/
SU_DLL float64 genrand_res53(void);


#endif // _SU_RAND_H
