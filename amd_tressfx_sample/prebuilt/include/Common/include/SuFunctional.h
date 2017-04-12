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

#ifndef _SU_FUNCTIONAL_H_
#define _SU_FUNCTIONAL_H_

#ifndef SU_USE_NATIVE_STL

//=================================================================================================================================
/// \ingroup Common
/// \brief A base class for a binary function.  This is a sushi equivalent of std::binary_function
//=================================================================================================================================
template< class FirstArgType, class SecondArgType, class ResultType >
class SuBinaryFunction
{
public:
   typedef FirstArgType     first_argument_type ;
   typedef SecondArgType    second_argument_type ;
   typedef ResultType       result_type;
};


//=================================================================================================================================
/// \ingroup Common
/// \brief A base class for a unary function.  This is a sushi equivalent of std::unary_function
//=================================================================================================================================
template< class ArgType, class ResultType >
class SuUnaryFunction
{
public:
   typedef ArgType     argument_type;
   typedef ResultType  result_type;
};


//=================================================================================================================================
/// \ingroup Common
/// \brief A predicate functor which compares two objects using their < operator.  This is a sushi equivalent of std::less()
//=================================================================================================================================
template< class Type >
class SuLess : public SuBinaryFunction<Type,Type,bool>
{
public:
   /// Default constructor for SuLess() functor
   SuLess() {};

   /// Binary function operator compares two objects using operator<
   bool operator()( const Type& a, const Type& b ) const
   {
      return a < b;
   };
};


//=================================================================================================================================
/// \ingroup Common
/// \brief A predicate functor which compares two objects using their > operator.  This is a sushi equivalent of std::less()
//=================================================================================================================================
template< class Type >
class SuGreater : public SuBinaryFunction< Type, Type, bool >
{
public:
   /// Default constructor for SuGreater() functor
   SuGreater() {};

   bool operator()( const Type& a, const Type& b ) const
   {
      return a > b;
   }
};


//=================================================================================================================================
/// \ingroup Common
/// \brief This is an adaptor class that converts a binary function template into a unary function template by hard-wiring a
///          specified value for the second argument.  This is a Sushi equivalent to the (somewhat obscure) std::binder2nd
//=================================================================================================================================
template< class Function >
class SuBinderSecond : public SuUnaryFunction< typename Function::first_argument_type, typename Function::result_type >
{
public:

   //=================================================================================================================================
   /// Default constructor for SuBinderSecond
   /// \param rFunction  The functor to call
   /// \param rSecondArg The value to pass as the functor's second argument
   //=================================================================================================================================
   SuBinderSecond( const Function& rFunction, const typename Function::second_argument_type& rSecondArg ) :
      m_rFunction( rFunction ), m_rSecondArg( rSecondArg )
   {
   };

   typename Function::result_type operator() ( const typename Function::first_argument_type& rArg ) const
   {
      return m_rFunction( rArg, m_rSecondArg );
   };

private:

   Function m_rFunction;                                    ///< The binary function to call
   typename Function::second_argument_type m_rSecondArg;    ///< The value to bind to the second argument

};


#else // defined(SU_USE_NATIVE_STL)

#include <functional>

#define SuBinaryFunction  std::binary_function
#define SuUnaryFunction   std::unary_function
#define SuLess            std::less
#define SuGreater         std::greater
#define SuBinderSecond    std::binder2nd


#endif   // SU_USE_NATIVE_STL



#endif // _SU_FUNCTIONAL_H_
