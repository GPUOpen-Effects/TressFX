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

//--------------------------------------------------------------------------------------
// File: StringTokenizer.h
//--------------------------------------------------------------------------------------

#include "StringTokenizer.h"

using namespace std;

int StringTokenizer(const std::wstring& input, const std::wstring& delimiter,
                std::vector<std::wstring>& results, bool includeEmpties)
{
    int iPos = 0;
    int newPos = -1;
    int sizeS2 = (int)delimiter.size();
    int isize = (int)input.size();

    if( isize == 0 || sizeS2 == 0 )
        return 0;

    vector<int> positions;
    newPos = (int)input.find(delimiter, 0);

    if( newPos < 0 )
        return 0;

    int numFound = 0;

    while( newPos >= iPos )
    {
        numFound++;
        positions.push_back(newPos);
        iPos = newPos;
        newPos = (int)input.find(delimiter, iPos+sizeS2);
    }

    if( numFound == 0 )
        return 0;

    for( int i=0; i <= (int)positions.size(); ++i )
    {
        wstring s(L"");

        if( i == 0 )
        {
            s = input.substr( i, positions[i] );
        }
        else
        {
            int offset = positions[i-1] + sizeS2;

            if( offset < isize )
            {
                if( i == (int)positions.size() )
                {
                    s = input.substr(offset);
                }
                else if( i > 0 )
                {
                    s = input.substr( positions[i-1] + sizeS2,
                          positions[i] - positions[i-1] - sizeS2 );
                }
            }
        }

        if( includeEmpties || ( s.size() > 0 ) )
        {
            results.push_back(s);
        }
    }

    return ++numFound;
}
