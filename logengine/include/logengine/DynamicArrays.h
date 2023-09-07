#ifndef _DYNAMIC_ARRAYS_
#define _DYNAMIC_ARRAYS_

#ifndef WIN32 // assume it is Linux
#include <string.h>
#endif

#include <exception> 
#include <string>
#include "functions.h"
#include "Compare.h"

#define valuemin(v1,v2) (((v1)<=(v2))?(v1):(v2))
#define valuemax(v1,v2) (((v1)>=(v2))?(v1):(v2))

//#ifdef WIN32
//#pragma warning( disable : 4786 )
//#endif

#define uint unsigned int
#define ulong unsigned long

#define DA_NPOS -1 // return value that usually mean "item is not found", this value returned by functions like IndexOf()

#define DA_EXCEPTION_PREFIX "THArrayException : "
/*!
 * Class for exceptions. Exceptions of this class are generated by 
 * several methods of array classes.
 *
 */
class THArrayException:public std::exception
{
public:
	THArrayException(const char* Message)        { Text = Message; whatText = DA_EXCEPTION_PREFIX + std::string(Message); }
	THArrayException(const std::string& Message) { Text = Message; whatText = DA_EXCEPTION_PREFIX + Message;}
	THArrayException(const THArrayException& ex) { Text = ex.Text; whatText = ex.whatText; }
	virtual ~THArrayException() noexcept {} //throw () { };
	THArrayException& operator=(const THArrayException& rhs) { Text = rhs.Text; whatText = rhs.whatText; return *this; }
	virtual std::string getErrorMessage() const				 { return whatText; }
	const char* what() const noexcept { return whatText.c_str(); }; //throw();
protected:
	THArrayException() {}
private:
	std::string Text;
	std::string whatText; // this is what what() returns as char*
};


//////////////////////////////////////////////////////////////////////
//  AVariant Class Interface
//////////////////////////////////////////////////////////////////////
#ifdef _USE_AVARIANT_

enum AVariantType {vdNull, vdDouble, vdCurrency, vdString, vdInteger, vdDate};

class AVariant {
protected:
	void CopyFrom(const AVariant& v);
public:
	enum AVariantType DataType;
	double AsDouble;
	double AsCurrency;
	std::string AsString;
	int AsInteger;
	int AsDate;

	inline AVariant() {Clear();}
	AVariant(const AVariant &v)			{CopyFrom(v);}
	void operator =(const AVariant &v)	{CopyFrom(v);}
	void operator =(const int &v)		{SetAsInteger(v);}
	void operator =(const double &v)	{SetAsDouble(v);}
	void operator =(const currency &v)	{SetAsCurrency(v);}
	void operator =(const std::string &v){SetAsString(v);}
	inline AVariant(const int v)		{SetAsInteger(v);}
	inline AVariant(const double v)		{SetAsDouble(v);}
	inline AVariant(const currency v)	{SetAsCurrency(v);}
	inline AVariant(std::string& v)		{SetAsString(v);}
	inline AVariant(char *v)			{SetAsString(v);}
	inline void Clear();
	inline void SetAsDouble(double v);
	inline void SetAsCurrency(const currency v);
	inline void SetAsString(const std::string& v);
	inline void SetAsString(char *v);
	inline void SetAsInteger(const int v);
	inline void SetAsDate(int v);
	void operator +=(AVariant& v);
	bool HaveData();
};

#endif


/*! \brief 11111111111111 1223 3444543
 *
 * Class for storing and manipulating raw data types i.e. data which do 
 * not have explicit type or the type can be changed dynamically. All that
 * THArrayRaw need to know is ItemSize of stored data. Very useful to 
 * store fields from database table.
 */

class THArrayRaw
{
protected:
	bool	Sorted;
	uint	FCount;
	uint	FCapacity;
	uint	FItemSize;
	void	*FMemory;
	void		Error(const uint Value, /*const uint vmin,*/ const int vmax) const;
	void		Grow();
	void		GrowTo(const uint Count);
	void		*CalcAddr(const uint num) const;
public:
	THArrayRaw();
	THArrayRaw(uint ItemSize);
	virtual ~THArrayRaw()			{ ClearMem(); }
//	void operator=(const THArrayRaw& a);
	void		SetItemSize(const uint ItemSize);
	inline uint	GetItemSize()const  { return FItemSize; }
	virtual void Clear()			{ FCount = 0; };
	void		ClearMem();
	uint		Add(const void *pValue);
	void		AddMany(const void *pValue, const uint Count);
	uint		Insert(const uint Index, const void *pValue);
	void		InsertMany(const uint Index, const void *pValue, const uint Count);
	void		Update(const uint Index, const void *pValue);
	void		UpdateMany(const uint Index, const void *pValue, const uint Count);
	inline void*GetAddr(const uint num) const { Error(num, FCount - 1); return CalcAddr(num); }
	virtual	void Delete(const uint Index);
	void		Get(const uint num, void *pValue) const;
	void*		GetPointer(const uint num) const { return GetAddr(num); }
	void		Hold();
	void		MoveData(const int FromPos, const int Count, const int Offset);
	inline uint	Count() const		{ return FCount; }
	inline uint	Capacity() const	{ return FCapacity; }
	inline void*Memory() const		{ return FMemory; }
	inline void SetSorted()  		{ Sorted = true; }
	void		Zero();
	void		SetCapacity(const uint Value);
	void		AddFillValues(const uint Count);
	void		Swap(const uint Index1, const uint Index2);
};


/*!
 * The base for all  the classes in the library. THArrayBase is an 
 * abstract class declaring only virtual methods manipulating arrays. It is 
 * not a template class therefore it contains only type independent methods 
 */

class THArrayBase 
{
public:
	virtual ~THArrayBase() {} // to eliminate warning C5204
	virtual uint	Add(const void*) = 0;
	//virtual	void	AddMany(const void* Values) = 0; not implememnted
	virtual void	AddFillValues(const uint Num) = 0;
	virtual uint	Capacity() const = 0;
	virtual uint	Count() const = 0;
	virtual void	Clear() = 0;
	virtual void	ClearMem() = 0;
	virtual	void	DeleteValue(const uint Index) = 0;
	virtual uint	Insert(const uint Index, const void* Value) = 0;
	virtual uint	ItemSize() const = 0;
	virtual void	Hold() = 0;
	virtual void	SetCapacity(const uint Value) = 0;
	virtual void	Zero() = 0;
//	virtual inline void Swap(const int Index1, const int Index2);
/*	void	Reverse ();
	void	Reverse (int endIndex);*/

};


//////////////////////////////////////////////////////////////////////
//  THArrayStringFix Class Interface
//////////////////////////////////////////////////////////////////////

class THArrayStringFix:public THArrayBase
{
private:
	THArrayRaw data;
protected:
	char*		GetAddr(const uint Index) const;
public:
//	void operator=(const THArrayStringFix& a) {
//		printf("THArrayStringFix =");
//	}
	//THArrayStringFix(){};
	THArrayStringFix(uint Length) { data.SetItemSize(Length); }
	virtual ~THArrayStringFix()   { ClearMem(); }
	std::string GetValue(const uint Index) const;
	void	SetValue(const uint Index, const std::string& Value);
	std::string operator[](const uint Index) const;
	inline uint	Count() const { return data.Count(); }
	void		Clear()       { data.Clear(); }
	void		ClearMem()    { data.ClearMem(); }
	void		Zero()        { data.Zero(); }
	void		Hold()        { data.Hold(); }
	void		DeleteValue(const uint Index)      { data.Delete(Index); };
	uint		Add(const void *pValue)            { return data.Add(pValue); }
	uint		AddValue(const std::string& Value) { return data.Add(Value.c_str()); }
	void		AddFillValues(const uint Num)      { data.AddFillValues(Num); }
	uint		Insert(const uint Index, const void* Value) { return data.Insert(Index, Value); }
	uint		AddChars(const void* pValue, const uint len);
	void		Reverse();
};


//////////////////////////////////////////////////////////////////////
//  THArray Class Interface
//////////////////////////////////////////////////////////////////////

template<class T>
class THArray :public THArrayBase
{
protected:
	bool	Sorted;
	uint 	FCount;
	uint 	FCapacity;
	T*		FMemory;
	void	Error(const uint Value, /*const uint vmin,*/ const int vmax) const;
	uint	GetGrowDelta();
	void	Grow();	/// Grow memory size allocated for elements
public:
	THArray();
	THArray(const THArray<T>& a);
	virtual	   ~THArray() { ClearMem(); }
	inline  T operator [](const uint Index) const { return GetValue(Index); };
	THArray<T>& operator=(const THArray<T>& a); // copy constructor
	inline	 T* Memory() const   { return FMemory; }
	inline uint ItemSize() const { return sizeof(T); }
	inline uint Count() const    { return FCount; };
	inline void SetSorted()      { Sorted = true; }
	void		Clear()          { FCount = 0; }
	uint		Capacity() const { return FCapacity; };
	inline void Hold()           { SetCapacity(FCount); }	
	void		ClearMem();
	void		Zero();
	void		GrowTo(const uint ToCount);
	void		SetCapacity(const uint Value);
	void		SetValue(const uint Index, const T& Value);
	T			GetValue(const uint Index) const;
	uint		InsertValue(const uint Index, const T& Value);
	uint		Insert(const uint Index, const void* Value) { return InsertValue(Index, *(T*)Value); }
	void		DeleteValue(const uint Index);
	inline uint AddValue(const T& Value)  { return InsertValue(FCount, Value); }
	inline uint Add(const void *pValue)   { return AddValue(*(T*)pValue);      }
	inline	 T* GetValuePointer(const uint Index) const;
	inline  int IndexOf(const T& Value, const Compare<T>& cmp) const { return IndexOfFrom(Value, 0, cmp); }
	inline  int IndexOf(const T& Value) const                        { return IndexOfFrom(Value, 0);      }
	int			IndexOfFrom(const T& Value, const uint Start) const;
	int			IndexOfFrom(const T& Value, const uint Start, const Compare<T>& cmp) const;
	void		AddFillValues(const uint Num);
	inline void Push(const T& Value);
	inline T	Pop();
	inline T	PopFront();
	inline void Swap(const uint Index1, const uint Index2);
	void		Reverse();
	void		Reverse(uint endIndex); // Reverse till specified element
#ifdef _USE_STREAMS_
	void		SaveToStream(TStream& stre);
#endif
};


//////////////////////////////////////////////////////////////////////
//  THArrayAuto Class Interface
//////////////////////////////////////////////////////////////////////

/*template<class T>
class THArrayAuto:public THArray<T>
{
protected:
	void	HaveValue(const int Index);
public:
    inline T operator [](const int Index);
    void    SetValue(const int Index,const T& Value);
    T		GetValue(const int Index);
    inline  T *GetValuePointer(const int Index);
};*/

//////////////////////////////////////////////////////////////////////
//  THash Class Interface
//////////////////////////////////////////////////////////////////////

template <class I,class V>
class THash
{
private:
	THArray<I> FAKeys;
	THArray<V> FAValues;
	void replace(uint i1, uint i2);
public:
	THash() {;}
	THash(THash<I,V>& a);
	//~THash();

	//THash<I,V>& operator=(const THash<I,V>& a);
	I		GetKey(uint Index) const { return FAKeys.GetValue(Index); }
	void	Clear()            { FAKeys.Clear(); FAValues.Clear(); }
	void	ClearMem()		   { FAKeys.ClearMem(); FAValues.ClearMem(); };
	inline uint  Count() const { return FAKeys.Count(); }
	THArray<I>* GetKeys()      { return &FAKeys;  }
	THArray<V>* GetValues()    { return &FAValues;}
	bool	IfExists(const I& Key) const;
	bool	IfExists(const I& Key, const Compare<I>& cmp) const;
	void	Delete(const I& Key);
	void	Delete(const I& K, const Compare<I>& cmp);
	void	SetValue(const I& Key, const V& Value);
	V		GetValue(const I& Key) const;
	V		GetValue(const I& Key, const Compare<I>& cmp) const;
	V*		GetValuePointer(const I& Key);
	V*		GetValuePointer(const I& Key, const Compare<I>& cmp);
	void	Sort();
	void	Reverse();

/*	void Minus(THash<I, V> in);	*/
};


//////////////////////////////////////////////////////////////////////
//  THash2 Class Interface
//////////////////////////////////////////////////////////////////////

template <class I1, class I2, class V> class THash2
{
protected:
	THash<I1, THash<I2, V> > hash;
	int FCount = 0;
	int InternalGetCount();
public:
	//THash2<I1,I2,V>& operator=(const THash2<I1,I2,V>& a);
	void Clear() { hash.Clear(); FCount = 0; }
	void SetValue(const I1& Key1, const THash<I2, V>& Value);
	void SetValue(const I1& Key1, const I2& Key2, const V& Value);
	V	 GetValue(const I1& Key1, const I2& Key2);
	THash<I2,V> GetValue(const I1& Key1) { return hash.GetValue(Key1); }
	void Delete(const I1& Key1, const I2& Key2);
	void Delete(const I1& Key1);
	bool IfExists(const I1& Key1, const I2& Key2);
	bool IfExists(const I1& Key1) { return hash.IfExists(Key1);	}
	inline int Count()            { return FCount; /*hash.Count();*/ };
	I1	 GetKey(uint Index)       { return hash.GetKey(Index);  };
	V*	 GetValuePointer(const I1& Key1, const I2& Key2) { return hash.GetValuePointer(Key1)->GetValuePointer(Key2); }
	THash<I2,V>* GetValuePointer(const I1& Key1) { return hash.GetValuePointer(Key1); }
	THArray<I1>* GetAIndexes()		    { return hash.GetKeys(); };
	THArray<THash<I2,V> >* GetAValues() { return hash.GetValues(); };

//	void Minus(THash2<I1, I2, V>& in);

};


//////////////////////////////////////////////////////////////////////
//  Typedefs Defines
//////////////////////////////////////////////////////////////////////

typedef THash<std::string, std::string> TStringHash;
typedef THArray<std::string> THArrayString;
typedef THArray<int>* PHArrayInt;
// Splits string to array of strings using Delim as delimiter
void StringToArray(const std::string& str, THArrayString& arr, const char Delim = '\n');

//////////////////////////////////////////////////////////////////////
//  THArray Class Implementation
//////////////////////////////////////////////////////////////////////

template<class T>
THArray<T>::THArray()  
{
	Sorted    = false;
	FCount    = 0;
	FCapacity = 0;
	FMemory   = nullptr;
}

template<class T> 
THArray<T>::THArray(const THArray<T>& a):THArray()
{
	//Sorted    = false;
	//FCount    = 0;
	//FCapacity = 0;
	//FMemory   = NULL;

	SetCapacity(a.FCount);
	for (uint i = 0; i < a.FCount; i++)
		FMemory[i] = a.FMemory[i];

	FCount = a.FCount;
	Sorted = a.Sorted;
}

//template<class T>
//THArray<T>::~THArray() 
//{
//	ClearMem();
//}	

template<class T>
void THArray<T>::Error(const uint Value, /*const uint vmin, */ const int vmax) const
{
	if(/*(vmin > Value) ||*/ ((int)Value) > vmax)
	{
		char str[100];

#ifdef WIN32 //__STDC_SECURE_LIB__ //_MSC_VER < 1400
	    sprintf_s(str, 100, "Error in THArray: Element with index %i not found!", Value);
#else
        sprintf(str, "Error in THArray: Element with index %i not found!", Value);		
#endif

		throw THArrayException(str);
	}
};

template<class T>
uint THArray<T>::GetGrowDelta()
{
	if (FCapacity > 64)
	{
		return FCapacity / 4; // increase by 25% for most cases
	}
	else
	{
		if (FCapacity > 8)
			return 16;
		else
			return 4;
	}
}

/// Grow allocated memory using a special algorithm 
template<class T>
void THArray<T>::Grow() 
{
	uint Delta = GetGrowDelta();
	SetCapacity(FCapacity + Delta);
};

template<class T>
void THArray<T>::GrowTo(const uint ToCount) 
{
	if(ToCount <= FCapacity)
		return;
	
	uint Delta = GetGrowDelta();

	if( (FCapacity + Delta) < ToCount )
		Delta = ToCount - FCapacity;

	SetCapacity(FCapacity + Delta);
};

template<class T>
void THArray<T>::SetCapacity(const uint Value) 
{
	T *newMemory;

	if (Value > 0) 
	{
		newMemory = new T[Value];
		for (uint i = 0; i < valuemin(Value, FCount); i++)
			newMemory[i] = FMemory[i];
	}
	else 
		newMemory = nullptr;

	delete [] FMemory;
	FMemory = newMemory;
	FCapacity = Value;
	if (FCapacity < FCount) 
		FCount = FCapacity;
};

//template<class T>
//inline uint THArray<T>::ItemSize() const
//{
//	return sizeof(T);
//}
//	
template<class T>
void THArray<T>::Reverse()
{
	if(FCount < 2) 
		return;

	Sorted = false;
	for(uint i = 0; i < FCount/2; i++)
	{
		Swap(i, FCount - 1 - i);
	}
}

// Reverse array till specified element
// Elements with index more than endIndex will not be reversed
// After Reverse element with index endIndex will have index 0,
// and element with index 0 will have index endIndex
template<class T>
void THArray<T>::Reverse(uint endIndex)
{
	if (FCount < 2) 
		return;
	if(endIndex == 0)
		return;

	if (endIndex >= FCount)
		endIndex = FCount - 1;

	Sorted = false;
	for(uint i = 0; i < (endIndex + 1)/2; i++)
	{
		Swap(i, endIndex - i);
	}
}

template<class T> 
THArray<T>& THArray<T>::operator=(const THArray<T>& a) 
{
	ClearMem();
	SetCapacity(a.FCount);
	
	//memcpy(FMemory, a.FMemory, sizeof(T) * a.FCount);
	for (uint i = 0; i < a.FCount; i++)
		FMemory[i] = a.FMemory[i];
	
	FCount = a.FCount;
	Sorted = a.Sorted;

	return *this;
}

template<class T>
void THArray<T>::Zero() 
{
	for (uint i = 0; i < FCount; i++)
		FMemory[i] = T();
}

template<class T>
void THArray<T>::ClearMem() 
{
	delete [] FMemory;
	FMemory   = nullptr;
	FCount    = 0;
	FCapacity = 0;
}

template<class T>
void THArray<T>::SetValue(const uint Index, const T& Value) 
{
	Sorted = false;
	Error(Index, (int)FCount - 1);
	FMemory[Index] = Value;
}

template<class T>
T THArray<T>::GetValue(const uint Index) const 
{
	Error(Index, (int)FCount - 1);
	return FMemory[Index];
}

template<class T>
uint THArray<T>::InsertValue(const uint Index, const T& Value) 
{
	Error(Index, (int)FCount);
	Sorted = false;
	if (FCount >= FCapacity)
		Grow();
	for(uint i = FCount; i > Index; i--)
		FMemory[i] = FMemory[i - 1];
	FMemory[Index] = Value;
	FCount++;

	return Index;
}

template<class T>
void THArray<T>::DeleteValue(const uint Index) 
{
	Error(Index, (int)FCount - 1);
	for(uint i = Index; i < FCount - 1; i++)
		FMemory[i] = FMemory[i + 1];
	FCount--;
}

template<class T>
inline T* THArray<T>::GetValuePointer(const uint Index) const
{
	Error(Index, (int)FCount - 1);
	return FMemory + Index;
}

template<class T>
int THArray<T>::IndexOfFrom(const T& Value, const uint Start, const Compare<T>& cmp) const 
{
	for(uint i = Start; i < FCount; i++)
	{
		T v = GetValue(i);
		if(cmp.eq(v, (const T&)Value))
			return i;
	}
	return DA_NPOS;
}

template<class T>
int THArray<T>::IndexOfFrom(const T& Value, const uint Start) const 
{
	for (uint i = Start; i < FCount; i++)
	if (GetValue(i) == Value)
		return (int)i;
	return DA_NPOS;
}

template<class T>
void THArray<T>::AddFillValues(const uint Num) 
{
	if((FCount + Num) > FCapacity) 
		GrowTo(FCount + Num);
	for (uint i = FCount; i < FCount + Num; i++)
		FMemory[i] = T();
    FCount = FCount + Num;
	Sorted = false;
}

template<class T>
inline void THArray<T>::Push(const T& Value) 
{
	AddValue(Value);
}

template<class T>
T	THArray<T>::Pop() 
{
	Error(0/*FCount - 1 */ , (int)FCount - 1); // just check that array is not empty
	FCount--;
	return FMemory[FCount];
}

template<class T>
T	THArray<T>::PopFront()
{
	Error(0, (int)FCount - 1); // just check that array is not empty
	T tmp = FMemory[0];
	DeleteValue(0);
	return tmp;
}

template<class T>
inline void THArray<T>::Swap(const uint Index1, const uint Index2)
{
	Error(Index1, (int)FCount - 1);
	Error(Index2, (int)FCount - 1);

	if(Index1 == Index2)
		return;

	T temp = FMemory[Index1];
	FMemory[Index1] = FMemory[Index2];
	FMemory[Index2] = temp;
}

#ifdef _USE_STREAMS_
template<class T>
void THArray<T>::SaveToStream(TStream& stre) 
{
	stre << FCount;
	stre.Write(FMemory, FCount*sizeof(T));
}
#endif //_USE_STREAMS_

//////////////////////////////////////////////////////////////////////
//  THArrayAuto Class Implementation
//////////////////////////////////////////////////////////////////////

/*template<class T>
void THArrayAuto<T>::HaveValue(const int Index) 
{
	if (Index>=FCount) AddFillValues(Index-FCount+1);
}

template<class T>
inline T THArrayAuto<T>::operator [](const int Index)
{
	return GetValue(Index);
}

template<class T>
void THArrayAuto<T>::SetValue(const int Index,const T& Value) 
{
	HaveValue(Index);
	SetValue(Index,Value);
}

template<class T>
T THArrayAuto<T>::GetValue(const int Index) 
{
	if (Index>=FCount)
		return T();
	else
		return THArray<T>::GetValue(Index);
}

template<class T>
inline  T *THArrayAuto<T>::GetValuePointer(const int Index) 
{
	HaveValue(Index);
	return THArray<T>::GetValuePointer(Index);
}
*/

//////////////////////////////////////////////////////////////////////
//  THash Class Implementation
//////////////////////////////////////////////////////////////////////


template <class I, class V>
THash<I,V>::THash(THash<I,V>& a) 
{
	FAKeys = a.FAKeys;
	FAValues = a.FAValues;
}

/*template <class I, class V>
THash<I,V>& THash<I,V>::operator=(const THash<I,V>& a) 
{
	FAKeys = a.FAKeys;
	FAValues = a.FAValues;

	return *this;
}
*/

template <class I, class V>
void THash<I,V>::replace(uint i1, uint i2) 
{
	V vtemp = FAValues[i1];
	I itemp = FAKeys[i1];
	
	FAKeys.SetValue(i1, FAKeys[i2]);
	FAValues.SetValue(i1, FAValues[i2]);
	FAKeys.SetValue(i2, itemp);
	FAValues.SetValue(i2, vtemp);
}

template <class I, class V>
void THash<I,V>::Reverse()
{
	if(FAKeys.Count() < 2)
		return;

	for(uint i = 0; i < FAKeys.Count()/2; i++)
	{
		replace(i, FAKeys.Count() - 1 - i);
	}
}
	
template <class I, class V>
bool THash<I,V>::IfExists(const I& Key) const
{
	return (FAKeys.IndexOf(Key) != DA_NPOS);
	//if (FAKeys.IndexOf(Key) != DA_NPOS)
	//	return true; 
	//else 
	//	return false;
}

template <class I, class V>
bool THash<I,V>::IfExists(const I& Key, const Compare<I>& cmp) const
{
	return (FAKeys.IndexOf(Key, cmp) != DA_NPOS);
	/*if (FAKeys.IndexOf(Key, cmp) != -1) 
		return true; 
	else 
		return false;*/
}
    
template <class I, class V>
void THash<I,V>::Delete(const I& Key) 
{
	int n;
	n = FAKeys.IndexOf(Key);
	if (n != DA_NPOS)
	{
		FAKeys.DeleteValue(n);
		FAValues.DeleteValue(n);
	}
}

template <class I, class V>
void THash<I,V>::Delete(const I& Key, const Compare<I>& cmp)
{
	int n;
	n = FAKeys.IndexOf(Key, cmp);
	if(n != DA_NPOS) 
	{
		FAKeys.DeleteValue(n);
		FAValues.DeleteValue(n);
	}
}

template <class I, class V>
void THash<I,V>::SetValue(const I& Key, const V& Value)
{
	int n;
	n = FAKeys.IndexOf(Key);
	if(n != DA_NPOS)
		FAValues.SetValue(n, Value);
	else 
	{
		FAKeys.AddValue(Key);
		FAValues.AddValue(Value);
	}
}

template <class I, class V>
V THash<I,V>::GetValue(const I& Key) const
{
	int n;
	n = FAKeys.IndexOf(Key);
	if(n == DA_NPOS)
		throw THArrayException("THash<I,V>::GetValue(Key) : Key not found !");
		//return V();
	return FAValues[n];
}

template <class I, class V>
V THash<I,V>::GetValue(const I& Key, const Compare<I>& cmp) const
{
	int n;
	n = FAKeys.IndexOf(Key, cmp);
	if(n == DA_NPOS)
		throw THArrayException("THash<I,V>::GetValue(Key, cmp) : Key not found !");
		//return V();
	return FAValues[n];
}
	
template <class I, class V>
V* THash<I,V>::GetValuePointer(const I& Key) 
{
	int n;
    n = FAKeys.IndexOf(Key);
    if (n == DA_NPOS) 
		return nullptr;
    return FAValues.GetValuePointer(n);
}

template <class I,class V>
V* THash<I,V>::GetValuePointer(const I& Key, const Compare<I>& cmp) 
{
	int n;
	n = FAKeys.IndexOf(Key, cmp);
	if (n == DA_NPOS)
		return nullptr;
	return FAValues.GetValuePointer(n);
}

template <class I, class V>
void THash<I,V>::Sort() 
{
	bool updated;
	do
	{
		updated = false;
		for (uint i = 1; i < FAKeys.Count(); i++)
			if(FAKeys[i - 1] > FAKeys[i]) 
			{
				replace(i - 1, i);
				updated = true;
			}
	}
	while(updated);
	FAKeys.SetSorted();
}

/*template <class I,class V>
void THash<I,V>::Minus(THash<I, V> in)
{
	I Key;
	for (int i=0; i<FAKeys.Count(); i++)
	{
		Key = FAKeys.GetValue(i);
		if (in.FAKeys.IndexOf(Key) != -1) FAValues.SetValue(i, FAValues.GetValue(i) - in.FAValues.GetValue(in.FAKeys.IndexOf(Key)));
	}
	for (i=0; i<in.FAKeys.Count(); i++)
	{
		Key = in.FAKeys.GetValue(i);
		if (FAKeys.IndexOf(Key) != -1) continue;
		else SetValue(Key, -in.FAValues.GetValue(i));
	}
}*/


//////////////////////////////////////////////////////////////////////
//  THash2 Class Implementation
//////////////////////////////////////////////////////////////////////

/*template <class I1, class I2, class V>
THash2<I1,I2,V>& THash2<I1,I2,V>::operator=(const THash2<I1,I2,V>& a)
{
	hash = a.hash;
	return *this;
}
*/

template <class I1, class I2, class V>
int THash2<I1, I2, V>::InternalGetCount()
{
	int result = 0;
	THArray<I1>* ind = GetAIndexes();
	for (int i = 0; i < ind->Count(); i++)
	{
		I1 a = ind->GetValue(i);
		THash<I2, V> val = hash.GetValue(a);
		result += val.Count();
	}

	return result;
}

//template <class I1, class I2, class V>
//void THash2<I1,I2,V>::Clear()
//{
//	hash.Clear();
//	FCount = 0;
//	//for (int i = 0; i < hash.Count(); i++)
//	//	hash.GetValue(hash.GetKey(i)).Clear();
//}

template <class I1, class I2, class V>
void THash2<I1,I2,V>::SetValue(const I1& Key1, const THash<I2, V>& Value)
{
	hash.SetValue(Key1, Value);
	FCount = InternalGetCount();
}

template <class I1, class I2, class V>
void THash2<I1,I2,V>::SetValue(const I1& Key1, const I2& Key2, const V& Value)
{
/*	THash<I2, V> InsH;
	if(hash.IfExists(Key1))
		InsH = hash.GetValue(Key1);
	InsH.SetValue(Key2, Value);
	hash.SetValue(Key1, InsH);*/

	
	THash<I2, V>* InsH = hash.GetValuePointer(Key1);

	if(InsH)
	{
		int before = InsH->Count();
		InsH->SetValue(Key2, Value);
		int after = InsH->Count();
		FCount += after - before;
	}
	else
	{
		THash<I2, V> h;
		h.SetValue(Key2, Value);
		hash.SetValue(Key1, h);
		FCount++;
	}
}

template <class I1, class I2, class V>
V THash2<I1,I2,V>::GetValue(const I1& Key1, const I2& Key2)
{
	THash<I2,V>* h = hash.GetValuePointer(Key1);
	if(h == nullptr)
		throw THArrayException("THash2::GetValue(Key1, Key2) : Key1 not found !");
	return h->GetValue(Key2);
}

//template <class I1, class I2, class V>
//THash<I2, V> THash2<I1,I2,V>::GetValue(const I1& Key1)
//{
//	return hash.GetValue(Key1);
//}

template <class I1, class I2, class V>
void THash2<I1,I2,V>::Delete(const I1& Key1, const I2& Key2)
{
	THash<I2,V>* TempH = hash.GetValuePointer(Key1);
	if (TempH)
	{
		TempH->Delete(Key2);
		FCount--;
	}
	/*if(TempH.Count() == 0) 
		hash.Delete(Key1);
	else 
		hash.SetValue(Key1, TempH);*/
}

template <class I1, class I2, class V>
void THash2<I1,I2,V>::Delete(const I1& Key1)
{
	THash<I2,V>* h = hash.GetValuePointer(Key1);
	int c = 0;
	if(h != nullptr) 
		c = h->Count();

	hash.Delete(Key1);
	FCount -= c; //InternalGetCount();
}

template <class I1, class I2, class V>
bool THash2<I1,I2,V>::IfExists(const I1& Key1, const I2& Key2)
{
	if(!hash.IfExists(Key1))
		return false;
	return hash.GetValuePointer(Key1)->IfExists(Key2);
}

//template <class I1, class I2, class V>
//bool THash2<I1,I2,V>::IfExists(const I1& Key1)
//{
//	return hash.IfExists(Key1); 
//}

//template <class I1, class I2, class V>
//THash<I2, V>* THash2<I1,I2,V>::GetValuePointer(const I1& Key1)
//{
//	return hash.GetValuePointer(Key1);
//}

//template <class I1, class I2, class V>
//V* THash2<I1,I2,V>::GetValuePointer(const I1& Key1, const I2& Key2)
//{
//	//return (THash<I2,V>*)
//	return hash.GetValuePointer(Key1)->GetValuePointer(Key2);
//}

/*
template <class I1, class I2, class V>
void THash2<I1,I2,V>::Minus(THash2<I1, I2, V>& in)
{
	I2 Key;
	for (int i=0; i<hash.Count(); i++)
	{
		Key = hash.GetKey(i);
		if (in.IfExists(Key)) 
			hash.GetValue(Key).Minus(in.GetValue(Key));
	}
	for (i=0; i<in.Count(); i++)
	{
		Key = in.GetKey(i);
		if (hash.IfExists(Key)) 
			continue;
		else
		{
			THash<I2,V> temp = in.GetValue(Key);
			for (int dd=0; dd<temp.Count(); dd++)
				temp.SetValue(temp.GetKey(dd), -temp.GetValue(temp.GetKey(dd)));
			hash.SetValue(Key, temp);
		}
	}
}
*/



#endif
