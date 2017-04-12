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

#ifndef _Su_INTERLOCKED_COUNTER_H_
#define _Su_INTERLOCKED_COUNTER_H_

//=====================================================================================================================
/// \brief This class encapsulates an integer value with atomic increment and decrement operations
///
/// This class is used to provide a thread-safe integer variable using the atomic operations provided by
///   the Win32 API.  Atomic functions are provided for incrementing decrementing, testing, and setting
///  
/// The assignment operators for this class are also atomic, but the constructors and conversion operator are not
///
//=====================================================================================================================
class SuInterlockedCounter
{
public:

    /// Default constructor, initialzies to zero
    inline SuInterlockedCounter() : m_nValue(0) { }

    /// Constructor which provides an initial value
    inline SuInterlockedCounter( long nValue ) : m_nValue(nValue) {}

    /// Copy constructor
    inline SuInterlockedCounter( const SuInterlockedCounter& rVar ) : m_nValue( rVar.m_nValue ) {}
    
    /// Assignment operator
    inline const SuInterlockedCounter& operator=( const SuInterlockedCounter& rVar ) {
       Set( rVar.m_nValue );
       return *this;
    }

    /// Int assignment operator
    inline const SuInterlockedCounter& operator=( long nValue ) {
       Set( nValue );
       return *this;
    }

    /// Implicit conversion to long
    inline operator long() { return m_nValue; };

    /// Atomically increments the variable.  Returns the new value
    SU_DLL long Increment();

    /// Atomically decrements the variable.  Returns the new value
    SU_DLL long Decrement();

    /// Atmoically sets the counter to the value 'nSet' if the current value is equal to 'nTest'
    /// \return The previous value of the counter
    SU_DLL long TestAndSet( long nTest, long nSet ); 

    /// Atomically sets the counter to a specified value and returns the original value
    SU_DLL long Set( long nValue );

private:

    volatile long m_nValue;
};

//=====================================================================================================================
/// \brief This class encapsulates a 64-bit integer value with atomic increment and decrement operations
///
/// This class is used to provide a thread-safe 64-bit integer variable using the atomic operations provided by
///   the Win32 API.  Atomic functions are provided for incrementing decrementing, testing, and setting
///  
/// The assignment operators for this class are also atomic, but the constructors and conversion operator are not
///
//=====================================================================================================================
class SuInterlockedCounter64
{
public:

	/// Default constructor, initialzies to zero
	inline SuInterlockedCounter64() : m_nValue(0) { }

	/// Constructor which provides an initial value
	inline SuInterlockedCounter64(int64 nValue) : m_nValue(nValue) {}

	/// Copy constructor
	inline SuInterlockedCounter64(const SuInterlockedCounter64& rVar) : m_nValue(rVar.m_nValue) {}

	/// Assignment operator
	inline const SuInterlockedCounter64& operator=(const SuInterlockedCounter64& rVar) {
		Set(rVar.m_nValue);
		return *this;
	}

	/// Int assignment operator
	inline const SuInterlockedCounter64& operator=(int64 nValue) {
		Set(nValue);
		return *this;
	}

	/// Implicit conversion to long64
	inline operator int64() { return m_nValue; };

	/// Atomically increments the variable.  Returns the new value
	SU_DLL int64 Increment();

	/// Atomically decrements the variable.  Returns the new value
	SU_DLL int64 Decrement();

	/// Atmoically sets the counter to the value 'nSet' if the current value is equal to 'nTest'
	/// \return The previous value of the counter
	SU_DLL int64 TestAndSet(int64 nTest, int64 nSet);

	/// Atomically sets the counter to a specified value and returns the original value
	SU_DLL int64 Set(int64 nValue);

private:

	volatile int64 m_nValue;
};
#endif