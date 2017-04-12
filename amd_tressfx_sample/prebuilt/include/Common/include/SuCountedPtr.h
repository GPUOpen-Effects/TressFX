//
// Copyright (c) 2017 Advanced Micro Devices, Inc. All rights reserved.
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

#ifndef _SU_COUNTEDPTR_H
#define _SU_COUNTEDPTR_H

// This include is a workaround for the latest version of tolua++, which chokes if you put the code found in the include directly
// into this file.
#include "ToluaTemplateHack.h"

// This warning is:  "deletion of pointer to incomplete type, no destructor called"
// In order to use counted pointers properly, you need to have the full class definition for whatever
// object you're pointing to, a forward declaration will not do, because then you can't call a destructor
// This warning is just to make sure we dont get memory leaks.  Hopefully we will work out a better solution in the future.
#ifdef WIN32
#pragma warning(error: 4150)
#endif

/// \brief This is an interface that is implemented by "owners" of shared objects in order to receive
/// notification that only a single reference to the object exists.  In this case, the owner of the object
/// may wish to delete it by releasing the final reference.
template <typename Type>
class SuSharedObjectOwner
{
public:

	/// This method is called when the reference count of a shared object reaches one, due to counted pointer destruction 
	virtual void NotifyLastReference(Type* pSharedObject) = 0;

};


/// \class SuCountedPtr
/// \brief Simple reference counted pointer.
/// \ingroup Common

template <typename Type>
// tolua_begin
class SuCountedPtr
{
	TOLUA_TEMPLATE_BIND(Type, SuLightController, SuGPUStructuredBuffer, SuGPUUnorderedAccessView, SuGPUSamplingResourceView, SuGPUDepthStencilResourceView, SuGPURenderableResourceView, SuGPUTexture2DArray, SuGPUTextureCubeArray, SuGPUTexture3D, SuMemoryBuffer, SuAnimationCurve, SuImageData2D, SuGPUVertexBuffer, SuGPUIndexBuffer, SuConstantBuffer, SuGPUTexture, SuGPUResource, SuEffect, SuCommandList, SuCommandListArray, SuCommandQueue, SuFence, SuIndirectCommandSignature)

	// this TEMPLATE_BIND macro tells tolua about all the template instantiations that I intend to use
	// ToLua needs to generate seperate bindings for each individual template instantiation.
	// in order to expose counted pointers to classes in the script, you need to add them to this list
	// if the counted pointer isn't exposed correctly, memory leaks will result

	// NOTE:  Because tolua++ has a stupid parser, you need to put all of the TEMPLATE_BIND arguments on one huge line.  You cannot
	//  seperate them out onto multiple lines for readability

public:
	typedef Type element_type;

	/// Explicit Constructor - allocate a new counter
	explicit SuCountedPtr(Type* p = NULL, SuSharedObjectOwner<Type>* pOwner = NULL) : m_pItsCounter(0)
	{
		if (p)
			m_pItsCounter = new Counter(p, 1, pOwner);
	}

	/// \brief Method which constructs a NULL counted pointer.  
	/// This is a workaround for creating NULL counted pointers in the script.  Tolua doesn't do it right if you try and pass 0 or nil
	static SuCountedPtr<Type> Null()
	{
		return SuCountedPtr<Type>(0);
	};

	/// Destuctor - Release with reference counting
	~SuCountedPtr()
	{
		Release();
	}

	/// Copy Constructor
	SuCountedPtr(const SuCountedPtr<Type>& r)
	{
		Acquire(r.m_pItsCounter);
	}


	/// operator= 
	SuCountedPtr& operator=(const SuCountedPtr& r)
	{
		if (this != &r)
		{
			Release();
			Acquire(r.m_pItsCounter);
		}
		return *this;
	}
// tolua_end
	/// operator*
	Type& operator*()  const { return *m_pItsCounter->ptr; }

	/// operator->
	Type* operator->() const { return m_pItsCounter->ptr; }

	//// implicit conversion to "bool"
	operator bool() const { return (get() != 0); }

// tolua_begin

	/// If the counter has been allocated, return the pointed to object, otherwise NULL
	Type* get() const { return m_pItsCounter ? m_pItsCounter->ptr : 0; }

	/// Return if the object has only been referenced once
	bool unique() const { return (m_pItsCounter ? m_pItsCounter->count == 1 : true); }

	/// Explicit assignment method, for performing assignment operations in lua code
	void set(const SuCountedPtr<Type>& rhs)
	{
		*this = rhs;
	};

// tolua_end

	/// Method for casting from one counted pointer type to another
	template< typename CastType > SuCountedPtr<CastType> cast() const
	{
		SuCountedPtr<CastType> ptr;

		// this doesn't work, because different template instantiations cannot access each others privates
		//ptr.Acquire( (SuCountedPtr<CastType>::Counter*)this->m_pItsCounter );

		// EVIL, SINFUL HACK:  cast the pointer to the CastType into a pointer of this type, and call acquire on that
		SuCountedPtr<Type>* pHack = (SuCountedPtr<Type>*) &ptr;
		pHack->Acquire(this->m_pItsCounter);

		return ptr;
	};


	/// Comparison operator, which lets this be used in a map
	bool operator<(const SuCountedPtr& r) const
	{
		return get() < r.get();
	};


private:

	/// Internal counter
	struct Counter
	{
		Counter(Type* p = 0, unsigned c = 1, SuSharedObjectOwner<Type>* pObjOwner = NULL) : ptr(p), count(c), pOwner(pObjOwner) {}

		Type*       ptr;
		unsigned    count;
		SuSharedObjectOwner<Type>* pOwner; ///< Object to notify when ref count reaches 1

	} *m_pItsCounter;


	/// Acquire the counter from another source
	void Acquire(Counter* c)
	{
		// increment the count
		m_pItsCounter = c;
		if (c)
			++c->count;
	}

	/// Release the counter and the pointed to object when reference count hits 0
	void Release()
	{
		// decrement the count, delete if it is 0
		if (m_pItsCounter)
		{
			if (--m_pItsCounter->count == 0)
			{
				delete m_pItsCounter->ptr;
				delete m_pItsCounter;
			}
			else if (m_pItsCounter->count == 1)
			{
				// when the reference count reaches 1, we optionally send a messages to the 'owner' of the shared object
				// the intended use of this is to allow an object that creates things that are shared between other things 
				// to receive notifications when nobody is using its shared things, so that it can clean them up
				// (for example, the sushi effect manager)
				if (m_pItsCounter->pOwner)
				{
					m_pItsCounter->pOwner->NotifyLastReference(m_pItsCounter->ptr);
				}
			}
			m_pItsCounter = 0;
		}
	}
// tolua_begin
};
// tolua_end


/// operator== for comparison with raw pointer
template<class T, class U> inline bool operator==(SuCountedPtr<T> const & a, const U* b)
{
	return a.get() == b;
}

/// operator==
template<class T, class U> inline bool operator== (SuCountedPtr<T> const & a, SuCountedPtr<U> const & b)
{
	return a.get() == b.get();
}

/// operator!=
template<class T, class U> inline bool operator!= (SuCountedPtr<T> const & a, SuCountedPtr<U> const & b)
{
	return a.get() != b.get();
}



#endif // _SU_COUNTEDPTR_H

