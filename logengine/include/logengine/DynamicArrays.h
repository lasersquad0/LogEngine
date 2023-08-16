#ifndef _DYNAMIC_ARRAYS_
#define _DYNAMIC_ARRAYS_

#include <stdio.h>
#include <exception> //unix
#include <string>
#include "functions.h"
#include "Compare.h"

#define valuemin(v1,v2) (((v1)<=(v2))?(v1):(v2))
#define valuemax(v1,v2) (((v1)>=(v2))?(v1):(v2))

#ifdef WIN32
#pragma warning( disable : 4786 )
#endif


/*!
 * Class for exceptions. Exceptions of this class are generated by 
 * several methods of array classes.
 *
 */
class THArrayException:public std::exception
{
public:
	THArrayException(const char * Message)		 { Text = Message; };
	THArrayException(const std::string& Message) { Text = Message; };
	THArrayException(const THArrayException& ex) { Text = ex.Text; };
	virtual ~THArrayException() throw ()			 { };
	THArrayException& operator=(const THArrayException& rhs) { Text = rhs.Text; return *this;};
	virtual std::string getErrorMessage() const				 { return "THArrayException : " + Text; };
	const char *what() const throw();
protected:
	THArrayException(){};
private:
	std::string Text;
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
	int		FCount;
	int		FCapacity;
	int		FItemSize;
	void	*FMemory;
	void	Error(const int Value, const int vmin, const int vmax) const;
	void	Grow();
	void	GrowTo(const int Count);
	inline void*CalcAddr(const int num) const;
public:
	THArrayRaw();
	THArrayRaw(int ItemSize);
	virtual ~THArrayRaw()			{ ClearMem(); }
//	void operator=(const THArrayRaw& a);
	void		SetItemSize(const int ItemSize);
	inline int	GetItemSize()const  { return FItemSize; }
	virtual void Clear()			{ FCount = 0; };
	void		ClearMem();
	int			Add(const void *pValue);
	void		AddMany(const void *pValue, const int Count);
	int			Insert(const int num, const void *pValue);
	void		InsertMany(const int num, const void *pValue, const int Count);
	void		Update(const int num, const void *pValue);
	void		UpdateMany(const int num, const void *pValue, const int Count);
	inline	void*GetAddr(const int num) const {Error(num, 0, FCount - 1); return CalcAddr(num);}
	virtual	void Delete(const int num);
	void		Get(const int num, void *pValue) const;
	void*		GetPointer(const int num) const { return GetAddr(num); }
	void		Hold();
	void		MoveData(const int FromPos, const int Count, const int Offset);
	inline	int	Count() const		{ return FCount; }
	inline	int	Capacity() const	{ return FCapacity; }
	inline	void*Memory() const		{ return FMemory; }
	inline	void SetSorted()		{ Sorted = true; }
	void		Zero();
	void		SetCapacity(const int Value);
	void		AddFillValues(const int Value);
	void Swap(const int Index1, const int Index2);
};


/*!
 * The base for all  the classes in the library. THArrayBase is an 
 * abstract class declaring only virtual methods manipulating arrays. It is 
 * not a template class therefore it contains only type independent methods 
 */

class THArrayBase {
public:
	virtual int		Add(const void *) = 0;
	//virtual	void	AddMany(const void* Values) = 0; not implememnted
	virtual void	AddFillValues(const int Num) = 0;
	virtual int		Capacity()const = 0;
	virtual int		Count()const = 0;
	virtual void	Clear() = 0;
	virtual void	ClearMem() = 0;
	virtual	void	DeleteValue(const int Index) = 0;
	virtual void	Insert(const int Index, const void* Value) = 0;
	virtual int		ItemSize() const = 0;
	virtual void	Hold() = 0;
	virtual void	SetCapacity(const int Value) = 0;
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
	char	*GetAddr(const int Index) const;
public:

//	void operator=(const THArrayStringFix& a) {
//		printf("THArrayStringFix =");
//	}
	//THArrayStringFix(){};
	THArrayStringFix(unsigned int Length) {data.SetItemSize(Length);}
	virtual ~THArrayStringFix(){ClearMem();}
	std::string GetValue(const int Index)const;
	void	SetValue(const int Index,const std::string& Value);
	std::string operator[](const int Index)const;
	void	Reverse();
	inline int	Count()const {return data.Count();}
	void	Clear() {data.Clear();}
	void	ClearMem() {data.ClearMem();}
	void	Zero() {data.Zero();}
	void	Hold() {data.Hold();}
	void	DeleteValue(const int Index){data.Delete(Index);};
	void	Insert(const int Index,const void* Value){data.Insert(Index,Value);};
	int		Add(const void *pValue) {return data.Add(pValue);}
	int		AddValue(const std::string& Value){return data.Add(Value.c_str());}
	int		AddChars(const void *pValue,const int len);
	void	AddFillValues(const int Num) {data.AddFillValues(Num);}
};


//////////////////////////////////////////////////////////////////////
//  THArray Class Interface
//////////////////////////////////////////////////////////////////////

template<class T>
class THArray:public THArrayBase
{
protected:
	bool	Sorted;
	int 	FCount;
	int 	FCapacity;
	T		*FMemory;
	void	Error(const int Value, const int vmin, const int vmax) const;
	/// Grow memory size allocated for elements
	void	Grow();
public:
	THArray();
	THArray(const THArray<T>& a);
	virtual ~THArray();
	inline  T operator [](const int Index) const;
	THArray<T>& operator=(const THArray<T>& a);
	inline T *Memory() const;
	inline int ItemSize() const;
	inline int Count() const;
	inline void SetSorted();
	void	Clear();
	void	ClearMem();
	void	Zero();
	void	GrowTo(const int ToCount);
	void	SetCapacity(const int Value);
	int		Capacity() const;
	inline void Hold();
	void	SetValue(const int Index, const T& Value);
	T   	GetValue(const int Index) const;
	void	InsertValue(const int Index, const T& Value);
	void	Insert(const int Index, const void* Value);
	void	DeleteValue(const int Index);
	inline int AddValue(const T& Value);
	inline int Add(const void *pValue);
	inline  T *GetValuePointer(const int Index) const;
	inline  int IndexOf(const T& Value, const Compare<T>& cmp) const;
	inline  int IndexOf(const T& Value) const;
	int		IndexOfFrom(const T& Value, const int Start) const;
	int		IndexOfFrom(const T& Value, const int Start, const Compare<T>& cmp) const;
	void	AddFillValues(const int Num);
	inline void Push(const T& Value);
	inline T	Pop();
	inline void Swap(const int Index1, const int Index2);
	void	Reverse ();
	void	Reverse (int endIndex); // Reverse till specified element
#ifdef _USE_STREAMS_
	void	SaveToStream(TStream& stre);
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
	THArray<I> FAIndex;
	THArray<V> FAValues;
	void replace(int i1, int i2);
public:
	THash() {;}
	THash(THash<I,V>& a);

	~THash();

	//THash<I,V>& operator=(const THash<I,V>& a);
	I	GetKey(int Index) {return FAIndex.GetValue(Index);}
	
	void Reverse ();
	
	void Clear() {FAIndex.Clear(); FAValues.Clear();}
	void ClearMem() {FAIndex.ClearMem(); FAValues.ClearMem();};
	bool IfExists(const I& Key) const;
	bool IfExists(const I& Key, const Compare<I>& cmp) const;

	void Delete(const I& Key);
	void Delete(const I& K, const Compare<I>& cmp);

	inline int  Count()const	{ return FAIndex.Count();	}
	THArray<I>* GetAIndexes()	{ return &FAIndex;			}
	THArray<V>* GetAValues()	{ return &FAValues; 		}

	void SetValue(const I& Key, const V& Value);
	V	 GetValue(const I& Key) const;
	V	 GetValue(const I& Key, const Compare<I>& cmp) const;
	V*	 GetValuePointer(const I& Key);
	V*	 GetValuePointer(const I& Key, const Compare<I>& cmp);
	void Sort();
/*	void Minus(THash<I, V> in);	*/
};


/*
template <class I1, class I2, class V> class T2Hash
{
protected:
	THash<I1, void*> hash;
public:
	T2Hash(){;}
	void Clear()
	{
		for (int i=0; i<hash.Count(); i++)
			((THash<I2,V>*)hash.GetValue(hash.GetKey(i)))->Clear();
	}
	void SetValue(const I1& Key1, const I2& Key2, const V& Value)
	{
		THash<I2,V> *InsH;
		if (!hash.IfExists(Key1)) InsH = new THash<I2,V>;
		else InsH = (THash<I2,V>*)hash.GetValue(Key1);
		InsH->SetValue(Key2, Value);
		hash.SetValue(Key1, InsH);
	}
	V GetValue(const I1& Key1, const I2& Key2)
	{
		THash<I2,V>* TempH = (THash<I2,V>*)hash.GetValue(Key1);
		return TempH->GetValue(Key2);
	}
	void Delete(const I1& Key1, const I2& Key2)
	{
		THash<I2,V>* TempH = (THash<I2,V>*)hash.GetValue(Key1);
		TempH->Delete(Key2);
		if (TempH->Count() <= 0) {hash.Delete(Key1); delete TempH;}
	}
	void DeleteInternalHash(const I1& Key1)
	{
		THash<I2,V>* TempH = (THash<I2,V>*)hash.GetValue(Key1);
		TempH->Clear();
		hash.Delete(Key1);
		delete TempH;
	}
	bool IfExist(const I1& Key1, const I2& Key2)
	{
		if (!hash.IfExists(Key1)) return false;
		THash<I2,V>* TempH = (THash<I2,V>*)hash.GetValue(Key1);
		return TempH->IfExists(Key2);
	}
	bool IfExistFirstKey(const I1& Key1)
	{
		if (!hash.IfExists(Key1)) return false;
		return true;
	}
	int Count(){return hash.Count();};
	THash<I2, V>* GetInternalHash(const I1& Key1){return (THash<I2,V>*)hash.GetValue(Key1);};
	I1 GetKey(int Index){return hash.GetKey(Index);};
	void Minus(T2Hash<I1, I2, V> &in)
	{
		int Key;
		for (int i=0; i<hash.Count(); i++)
		{
			Key = hash.GetKey(i);
			if (in.hash.IfExists(Key)) 
				((THash<I2,V>*)hash.GetValue(Key))->Minus(*(THash<I2,V>*)in.hash.GetValue(Key));
		}
		for (i=0; i<in.hash.Count(); i++)
		{
			Key = in.hash.GetKey(i);
			if (hash.IfExists(Key)) continue;
			else
			{
				THash<I2,V> *temp = (THash<I2,V>*)in.hash.GetValue(Key);
				for (int dd=0; dd<temp->Count(); dd++)
					temp->SetValue(temp->GetKey(dd), -temp->GetValue(temp->GetKey(dd)));
				hash.SetValue(Key, (void*)temp);
			}
		}
	}
};
*/

//////////////////////////////////////////////////////////////////////
//  THash2 Class Interface
//////////////////////////////////////////////////////////////////////

template <class I1, class I2, class V> class THash2
{
protected:
	THash<I1, THash<I2, V> > hash;
public:
	//THash2<I1,I2,V>& operator=(const THash2<I1,I2,V>& a);
	void Clear();
	void SetValue(const I1& Key1, const THash<I2, V>& Value);
	void SetValue(const I1& Key1, const I2& Key2, const V& Value);
	V			GetValue(const I1& Key1, const I2& Key2);
	THash<I2, V>GetValue(const I1& Key1);
	void Delete(const I1& Key1, const I2& Key2);
	void Delete(const I1& Key1);
	bool IfExists(const I1& Key1, const I2& Key2);
	bool IfExists(const I1& Key1);
	inline int Count()	 { return hash.Count();		};
	I1 GetKey(int Index) { return hash.GetKey(Index);};
	V*			 GetValuePointer(const I1& Key1, const I2& Key2);
	THash<I2, V>*GetValuePointer(const I1& Key1);
	THArray<I1>* GetAIndexes()			{ return hash.GetAIndexes();};
	THArray<THash<I2, V> >* GetAValues(){ return hash.GetAValues();	};

//	void Minus(THash2<I1, I2, V>& in);

};


//////////////////////////////////////////////////////////////////////
//  Typedefs Defines
//////////////////////////////////////////////////////////////////////

typedef THash<std::string, std::string> TStringHash;
typedef THArray<int>* PHArrayInt;
// Splits string to array of strings using Delim as delimiter
void StringToArray(const std::string& str, THArray<std::string>& arr, const char Delim='\n');


//////////////////////////////////////////////////////////////////////
//  THArray Class Implementation
//////////////////////////////////////////////////////////////////////

template<class T>
THArray<T>::THArray()  
{
	Sorted    = false;
	FCount    = 0;
	FCapacity = 0;
	FMemory   = NULL;
}

template<class T> 
THArray<T>::THArray(const THArray<T>& a) 
{
	Sorted    = false;
	FCount    = 0;
	FCapacity = 0;
	FMemory   = NULL;

	SetCapacity(a.FCount);
	for (int i = 0; i < a.FCount; i++)
		FMemory[i] = a.FMemory[i];

	FCount = a.FCount;
	Sorted = a.Sorted;
}

template<class T>
THArray<T>::~THArray() 
{
	ClearMem();
}	

template<class T>
void THArray<T>::Error(const int Value, const int vmin, const int vmax) const 
{
	if((vmin > Value) || (vmax < Value)) 
	{
		char str[512];

#if __STDC_SECURE_LIB__ //_MSC_VER < 1400
	    sprintf_s(str, 512, "Error in THArray: Element with index %i not found!", Value);
#else
        sprintf(str, "Error in THArray: Element with index %i not found!", Value);		
#endif

		throw THArrayException(str);
	}
};

/// Grow allocated memory using a special algorithm 
template<class T>
void THArray<T>::Grow() 
{
	int Delta;
	if(FCapacity > 64)
		Delta = FCapacity / 4 ;
	else
	{
		if(FCapacity > 8) 
			Delta = 16;
		else
			Delta = 4;
	}

	SetCapacity(FCapacity + Delta);
};

template<class T>
void THArray<T>::GrowTo(const int ToCount) 
{
	int Delta;
	if(ToCount <= FCapacity)
		return;
	if(FCapacity > 64)
		Delta = FCapacity/4 ;
	else
	{
		if(FCapacity > 8)
			Delta = 16;
		else
			Delta = 4;
	}

	if((FCapacity + Delta) < ToCount)
		Delta = ToCount - FCapacity;

	SetCapacity(FCapacity + Delta);
};

template<class T>
void THArray<T>::SetCapacity(const int Value) 
{
	T *newMemory;

	if (Value > 0) 
	{
		newMemory = new T[Value];
		for (int i = 0; i < valuemin(Value, FCount); i++)
			newMemory[i] = FMemory[i];
	} 
	else 
		newMemory = NULL;

	delete [] FMemory;
	FMemory = newMemory;
	FCapacity = Value;
	if (FCapacity < FCount) 
		FCount = FCapacity;
};

template<class T>
inline int THArray<T>::ItemSize() const
{
	return sizeof(T);
}
	
template<class T>
void THArray<T>::Reverse ()
{
//	T temp;
	if(FCount < 2) 
		return;

	Sorted = false;
	for(int i = 0; i < FCount/2; i++)
		{
			Swap(i, FCount - 1 - i);
		/*temp = FMemory[i];
		FMemory[i] = FMemory[FCount-1-i];
		FMemory[FCount-1-i] = temp;*/
		}
}

// Reverse array till specified element
// Elements with index more than endIndex will not be reversed
// After Reverse element with index endIndex will have index 0,
// and element with index 0 will have index endIndex
template<class T>
void THArray<T>::Reverse (int endIndex)
{
	if (FCount < 2) 
		return;
	if(endIndex <= 0)
		return;

	if (endIndex >= FCount)
		endIndex = FCount - 1;

	//T temp;
	
	Sorted = false;
	for(int i = 0; i < (endIndex + 1)/2; i++)
		{
			Swap(i, endIndex - i);
			/*
		temp = FMemory[i];
		FMemory[i] = FMemory[endIndex-i];
		FMemory[endIndex-i] = temp;
		*/
		}
}

template<class T> 
THArray<T>& THArray<T>::operator=(const THArray<T>& a) 
{
	ClearMem();
	SetCapacity(a.FCount);
	
	for (int i = 0; i < a.FCount; i++)
		FMemory[i] = a.FMemory[i];
	
	FCount = a.FCount;
	Sorted = a.Sorted;

	return *this;
}


template<class T> 
T THArray<T>::operator [](const int Index)const 
{
	return GetValue(Index);
}

template<class T>
inline int THArray<T>::Count()const 
{
	return FCount;
}

template<class T>
inline void THArray<T>::SetSorted() 
{
	Sorted = true;
}

template<class T>
inline T* THArray<T>::Memory()const 
{
	return FMemory;
}

template<class T>
void THArray<T>::Clear() 
{
	FCount = 0;
}

template<class T>
void THArray<T>::Zero() 
{
	for (int i = 0; i < FCount; i++)
		FMemory[i] = T();
}

template<class T>
int THArray<T>::Capacity() const
{
	return FCapacity;
}

template<class T>
inline void THArray<T>::Hold() 
{
	SetCapacity(FCount);
}

template<class T>
void THArray<T>::ClearMem() 
{
	delete [] FMemory;
	FMemory   = NULL;
	FCount    = 0;
	FCapacity = 0;
}

template<class T>
void THArray<T>::SetValue(const int Index, const T& Value) 
{
	Sorted = false;
	Error(Index, 0, FCount - 1);
	FMemory[Index] = Value;
}

template<class T>
T THArray<T>::GetValue(const int Index) const 
{
	Error(Index, 0, FCount - 1);
	return FMemory[Index];
}

template<class T>
void THArray<T>::InsertValue(const int Index,const T& Value) 
{
	Error(Index, 0, FCount);
	Sorted = false;
	if (FCount >= FCapacity)
		Grow();
	for(int i = FCount; i>Index; i--)
		FMemory[i] = FMemory[i-1];
	FMemory[Index] = Value;
	FCount++;
}

template<class T>
void THArray<T>::Insert(const int Index, const void* Value) 
{
	InsertValue(Index, *(T*)Value);
}

template<class T>
void THArray<T>::DeleteValue(const int Index) 
{
	Error(Index, 0, FCount - 1);
	for(int i = Index; i < FCount - 1; i++)
		FMemory[i] = FMemory[i + 1];
	FCount--;
}

template<class T>
inline int THArray<T>::AddValue(const T& Value) 
{
	InsertValue(FCount,Value);
	return FCount - 1;
}

template<class T>
inline int THArray<T>::Add(const void *pValue) 
{
	return AddValue(*(T *)pValue);
}

template<class T>
inline  T* THArray<T>::GetValuePointer(const int Index) const
{
	Error(Index, 0, FCount - 1);
	return FMemory + Index;
}

template<class T>
inline int THArray<T>::IndexOf(const T& Value, const Compare<T>& cmp) const 
{
	return IndexOfFrom(Value, 0, cmp);
}

template<class T>
int THArray<T>::IndexOfFrom(const T& Value, const int Start, const Compare<T>& cmp) const 
{
	for(int i = Start; i < FCount; i++)
	{
		T v = GetValue(i);
		if(cmp.eq(v, (const T&)Value))
			return i;
	}
	return -1;
}

template<class T>
inline int THArray<T>::IndexOf(const T& Value) const 
{
	return IndexOfFrom(Value, 0);
}

template<class T>
int THArray<T>::IndexOfFrom(const T& Value,const int Start) const 
{
	for (int i = Start; i < FCount; i++)
	if (GetValue(i) == Value)
		return i;
	return -1;
}

// !!!!! what is happend if Num < 0 
template<class T>
void THArray<T>::AddFillValues(const int Num) 
{
	if((FCount + Num) > FCapacity) 
		GrowTo(FCount + Num);
	for (int i = FCount; i < FCount + Num; i++)
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
	Error(FCount - 1, 0, FCount - 1);
	FCount--;
	return FMemory[FCount];
}

template<class T>
inline void THArray<T>::Swap(const int Index1, const int Index2)
{
	Error(Index1, 0, FCount - 1);
	Error(Index2, 0, FCount - 1);

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
	FAIndex = a.FAIndex;
	FAValues = a.FAValues;
}

template <class I, class V>
THash<I,V>::~THash() 
{
//	FAIndex = FAIndex;
//	FAValues = FAValues;
}

/*template <class I, class V>
THash<I,V>& THash<I,V>::operator=(const THash<I,V>& a) 
{
	FAIndex = a.FAIndex;
	FAValues = a.FAValues;

	return *this;
}
*/

template <class I, class V>
void THash<I,V>::replace(int i1, int i2) 
{
	V temp;
	I itemp;
	itemp = FAIndex[i1];
	temp = FAValues[i1];
	FAIndex.SetValue(i1, FAIndex[i2]);
	FAValues.SetValue(i1, FAValues[i2]);
	FAIndex.SetValue(i2, itemp);
	FAValues.SetValue(i2, temp);
}

template <class I, class V>
void THash<I,V>::Reverse ()
{
	if(FAIndex.Count() <= 1)
		return;
	for(int i = 0; i < FAIndex.Count()/2; i++)
	{
		replace(i, FAIndex.Count() - 1 - i);
	}
}
	
template <class I, class V>
bool THash<I,V>::IfExists(const I& Key) const
{
	if (FAIndex.IndexOf(Key) != -1) 
		return true; 
	else 
		return false;
}

template <class I, class V>
bool THash<I,V>::IfExists(const I& Key, const Compare<I>& cmp) const
{
	if (FAIndex.IndexOf(Key, cmp) != -1) 
		return true; 
	else 
		return false;
}
    
template <class I, class V>
void THash<I,V>::Delete(const I& Key) 
{
	int n;
	n = FAIndex.IndexOf(Key);
	if (n >= 0)
	{
		FAIndex.DeleteValue(n);
		FAValues.DeleteValue(n);
	}
}

template <class I, class V>
void THash<I,V>::Delete(const I& K, const Compare<I>& cmp)
{
	int n;
	n = FAIndex.IndexOf(Key, cmp);
	if(n >= 0) 
	{
		FAIndex.DeleteValue(n);
		FAValues.DeleteValue(n);
	}
}

template <class I, class V>
void THash<I,V>::SetValue(const I& Key, const V& Value)
{
	int n;
	n = FAIndex.IndexOf(Key);
	if(n >= 0)
		FAValues.SetValue(n, Value);
	else 
	{
		FAIndex.AddValue(Key);
		FAValues.AddValue(Value);
	}
}

template <class I, class V>
V THash<I,V>::GetValue(const I& Key) const
{
	int n;
	n = FAIndex.IndexOf(Key);
	if(n == -1)
		throw THArrayException("THash<I,V>::GetValue(Key) : Key not found !");
		//return V();
	return FAValues[n];
}

template <class I, class V>
V THash<I,V>::GetValue(const I& Key, const Compare<I>& cmp) const
{
	int n;
	n = FAIndex.IndexOf(Key, cmp);
	if(n == -1)
		throw THArrayException("THash<I,V>::GetValue(Key, cmp) : Key not found !");
		//return V();
	return FAValues[n];
}
	
template <class I, class V>
V* THash<I,V>::GetValuePointer(const I& Key) 
{
	int n;
    n = FAIndex.IndexOf(Key);
    if (n == -1) 
		return NULL;
    return FAValues.GetValuePointer(n);
}

template <class I,class V>
V* THash<I,V>::GetValuePointer(const I& Key, const Compare<I>& cmp) 
{
	int n;
	n = FAIndex.IndexOf(Key, cmp);
	if (n == -1)
		return NULL;
	return FAValues.GetValuePointer(n);
}

template <class I, class V>
void THash<I,V>::Sort() 
{
	bool updated;
	do
	{
		updated = false;
		for (int i = 0; i < FAIndex.Count() - 1; i++)
			if(FAIndex[i] > FAIndex[i + 1]) 
			{
				replace(i, i + 1);
				updated = true;
			}
	}
	while(updated);
	FAIndex.SetSorted();
}

/*template <class I,class V>
void THash<I,V>::Minus(THash<I, V> in)
{
	I Key;
	for (int i=0; i<FAIndex.Count(); i++)
	{
		Key = FAIndex.GetValue(i);
		if (in.FAIndex.IndexOf(Key) != -1) FAValues.SetValue(i, FAValues.GetValue(i) - in.FAValues.GetValue(in.FAIndex.IndexOf(Key)));
	}
	for (i=0; i<in.FAIndex.Count(); i++)
	{
		Key = in.FAIndex.GetValue(i);
		if (FAIndex.IndexOf(Key) != -1) continue;
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
void THash2<I1,I2,V>::Clear()
{
	hash.Clear();
	//for (int i = 0; i < hash.Count(); i++)
	//	hash.GetValue(hash.GetKey(i)).Clear();
}

template <class I1, class I2, class V>
void THash2<I1,I2,V>::SetValue(const I1& Key1, const THash<I2, V>& Value)
{
	hash.SetValue(Key1, Value);
}

template <class I1, class I2, class V>
void THash2<I1,I2,V>::SetValue(const I1& Key1, const I2& Key2, const V& Value)
{
/*	THash<I2, V> InsH;
	if(hash.IfExists(Key1))
		InsH = hash.GetValue(Key1);
	InsH.SetValue(Key2, Value);
	hash.SetValue(Key1, InsH);*/

	
	THash<I2, V>* InsH;
	InsH = hash.GetValuePointer(Key1);
	if(InsH)
	{
		InsH->SetValue(Key2, Value);
	}
	else
	{
		THash<I2, V> h;
		h.SetValue(Key2, Value);
		hash.SetValue(Key1, h);
	}
}

template <class I1, class I2, class V>
V THash2<I1,I2,V>::GetValue(const I1& Key1, const I2& Key2)
{
	THash<I2,V>* h = hash.GetValuePointer(Key1);
	if(h == NULL)
		throw THArrayException("THash2::GetValue(Key1, Key2) : Key1 not found !");
	return h->GetValue(Key2);
}

template <class I1, class I2, class V>
THash<I2, V> THash2<I1,I2,V>::GetValue(const I1& Key1)
{
	return hash.GetValue(Key1);
}

template <class I1, class I2, class V>
void THash2<I1,I2,V>::Delete(const I1& Key1, const I2& Key2)
{
	THash<I2,V>* TempH = hash.GetValuePointer(Key1);
	if(TempH)
		TempH->Delete(Key2);
	/*if(TempH.Count() == 0) 
		hash.Delete(Key1);
	else 
		hash.SetValue(Key1, TempH);*/
}

template <class I1, class I2, class V>
void THash2<I1,I2,V>::Delete(const I1& Key1)
{
	hash.Delete(Key1);
}

template <class I1, class I2, class V>
bool THash2<I1,I2,V>::IfExists(const I1& Key1, const I2& Key2)
{
	if(!hash.IfExists(Key1))
		return false;
	return hash.GetValuePointer(Key1)->IfExists(Key2);
}

template <class I1, class I2, class V>
bool THash2<I1,I2,V>::IfExists(const I1& Key1)
{
	return hash.IfExists(Key1); 
}

template <class I1, class I2, class V>
THash<I2, V>* THash2<I1,I2,V>::GetValuePointer(const I1& Key1)
{
	return hash.GetValuePointer(Key1);
}

template <class I1, class I2, class V>
V* THash2<I1,I2,V>::GetValuePointer(const I1& Key1, const I2& Key2)
{
	//return (THash<I2,V>*)
	return hash.GetValuePointer(Key1)->GetValuePointer(Key2);
}

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
