/*
 * DynamicArrays.cpp
 *
 * Copyright 2023, Dynamic Arrays Project. All rights reserved.
 *
 * See the COPYING file for the terms of usage and distribution.
 */

#include "DynamicArrays.h"


//////////////////////////////////////////////////////////////////////
//  THArrayRaw Class Implementation
//////////////////////////////////////////////////////////////////////

THArrayRaw::THArrayRaw()
{
    Sorted    = false;
    FCount    = 0;
    FCapacity = 0;
    FItemSize = 1;
    FMemory   = nullptr;
}

THArrayRaw::THArrayRaw(uint ItemSize):THArrayRaw()
{
	if (ItemSize > 0)
		FItemSize = ItemSize;
	else
		ThrowZeroItemSize();
}

/*void THArrayRaw::operator=(const THArrayRaw& a) {
	ClearMem();
	FItemSize=a.FItemSize;
	SetCapacity(a.FCount);
	memmove(FMemory,a.FMemory,FCount*FItemSize);
	Sorted=a.Sorted;
}*/

void THArrayRaw::Error(const uint Value, /*const uint vmin,*/ const uint vmax) const
{
	if(/*(vmin > Value) ||*/ (vmax <= Value))
	{
		char str[512];
#ifdef WIN32 //__STDC_SECURE_LIB__ //_MSC_VER < 1400
        sprintf_s(str, 512, "Error in HArray: Element with index %i not found!", Value);		
#else
        sprintf(str, "Error in HArray: Element with index %i not found!", Value);        
#endif
		throw THArrayException(str);
	}
 }

void* THArrayRaw::CalcAddr(const uint num) const 
{
	return (void*)((unsigned char *)FMemory + static_cast<size_t>(num) * FItemSize);
}

void THArrayRaw::SetItemSize(const uint Size)
{
 	if (Size > 0)
		FItemSize = Size;
	else
		ThrowZeroItemSize();
	
	ClearMem();
}

void THArrayRaw::Delete(const uint num)
{
	Error(num, FCount);
	if (num < (FCount - 1)) // do not need to call memmove if we delete last item.
		memmove(GetAddr(num), GetAddr(num + 1), (FCount - (static_cast<size_t>(num) + 1)) * FItemSize);
	FCount--;
}

void THArrayRaw::ClearMem()
{
	Clear();
	FCapacity = 0;
	free(FMemory);
	FMemory = nullptr;
}

void THArrayRaw::Get(const uint num, void *pValue) const
{
	Error(num, FCount);
	if (pValue != nullptr) 
		memmove(pValue, CalcAddr(num), FItemSize);
}

uint THArrayRaw::Add(const void *pValue)
{
    Sorted = false;
	return Insert(FCount, pValue);
}

void THArrayRaw::AddMany(const void *pValue, const uint Count)
{
    Sorted = false;
	if (Count == 0) 
	{ 
		char str[512];
#ifdef WIN32 //__STDC_SECURE_LIB__//_MSC_VER < 1400
		sprintf_s(str, 512, "AddMany(): invalid parameter 'Count'=%i !", Count);
#else
        sprintf(str, "AddMany(): invalid parameter 'Count'=%i !", Count);        
#endif  
		throw THArrayException(str);
	}
    InsertMany(FCount, pValue, Count);
}

uint THArrayRaw::Insert(const uint Index, const void *pValue)
{
	Error(Index, FCount + 1);
	if (FCount >= FCapacity) 
		Grow();
	FCount++;
	memmove(CalcAddr(Index + 1), CalcAddr(Index), (FCount - static_cast<size_t>(Index) - 1) * FItemSize);
	Update(Index, pValue);
    Sorted = false;
	return Index;
}

void THArrayRaw::InsertMany(const uint num, const void *pValue, const uint Count)
{
	Error(num, FCount + 1);
	if ((FCount + Count) > FCapacity) 
		GrowTo(FCount + Count);

	FCount=FCount + Count;
	memmove(CalcAddr(num + Count), CalcAddr(num), (FCount - static_cast<size_t>(num) - Count) * FItemSize);
    Sorted = false;
	UpdateMany(num, pValue, Count);
}

void THArrayRaw::Update(const uint num, const void *pValue)
{
	Error(num, FCount);
	if (pValue != nullptr) 
		memmove(CalcAddr(num), pValue, FItemSize);
	else
		memset(CalcAddr(num), 0, FItemSize);
    Sorted = false;
}

void THArrayRaw::UpdateMany(const uint num, const void *pValue, const uint Count)
{
	Error(num + Count - 1, FCount);
	memmove(GetAddr(num), pValue, FItemSize * static_cast<size_t>(Count));
    Sorted = false;
}

void THArrayRaw::Grow()
{
	uint Delta;

	if(FCapacity > 64)
	{
		Delta = FCapacity/4 ;
	}
	else
	{
		if(FCapacity > 8)
			Delta = 16 ;
		else 
			Delta = 4;
	}

	SetCapacity(FCapacity + Delta);
}

void THArrayRaw::GrowTo(const uint Count)
{
	uint Delta;

	if(Count <= FCapacity)
		return;

	if(FCapacity > 64)
		Delta = FCapacity/4 ;
	else
	{
		if(FCapacity > 8)
			Delta = 16 ;
		else 
			Delta = 4;
	}

	if ((FCapacity + Delta) < Count) 
		Delta = Count - FCapacity;

	SetCapacity(FCapacity + Delta);
}

void THArrayRaw::SetCapacity(const uint Value)
{
	if(Value > 0)
	{
		FMemory = realloc(FMemory, static_cast<size_t>(Value) * FItemSize);
		FCapacity = Value;
	} 
	else  // Value == 0
	{
		free(FMemory);
		FMemory = nullptr;
	}

  if(FCount > FCapacity)
	  FCount = FCapacity;
}

void THArrayRaw::AddFillValues(const uint Count)
{
    if ((FCount + Count) > FCapacity)
		GrowTo(FCount + Count);
	memset(CalcAddr(FCount), 0, static_cast<size_t>(Count) * FItemSize);
    FCount = FCount + Count;
    Sorted = false;
}

void THArrayRaw::Zero()
{
    if (FCount > 0)
        memset(FMemory, 0, static_cast<size_t>(FCount) * FItemSize);
}

void THArrayRaw::Hold()
{
    SetCapacity(FCount);
}

void THArrayRaw::Swap(const uint Index1, const uint Index2)
{
	Error(Index1, FCount);
	Error(Index2, FCount);

	if(Index1 == Index2)
		return;
	
	void* temp = malloc(FItemSize);
	if(temp)
	{
		memcpy(temp, GetAddr(Index1), FItemSize);
		memcpy(GetAddr(Index1), GetAddr(Index2), FItemSize);
		memcpy(GetAddr(Index2), temp, FItemSize);
	}

	free(temp);
}

//////////////////////////////////////////////////////////////////////
//  THArrayStringFix Class Implementation
//////////////////////////////////////////////////////////////////////

void THArrayStringFix::Reverse()
{
	std::string temp;
	if (data.Count() <= 1)
		return;
	for (uint i = 0; i < data.Count()/2; i++)
	{
		temp = GetValue(i);
		data.Update(i, (void*)GetValue(data.Count() - 1 - i).data());
		data.Update(data.Count() - 1 - i, (void*)temp.data());
	}
}

char* THArrayStringFix::GetAddr(const uint Index) const
{
	return (char *)(data.GetAddr(Index));
}

std::string THArrayStringFix::operator [](const uint Index) const
{
	return GetValue(Index);
}

void THArrayStringFix::SetValue(const uint Index, const std::string& Value)
{
	data.Update(Index, Value.c_str());
}

std::string THArrayStringFix::GetValue(const uint Index) const 
{
	std::string s(GetAddr(Index), data.GetItemSize());
	return s;
}

uint THArrayStringFix::AddChars(const void *pValue, const uint len) 
{
	uint i;
	char* b;

	b = (char*)malloc(data.GetItemSize());
	memset(b, 0, data.GetItemSize());

	i = valuemin(len, data.GetItemSize());

#ifdef WIN32 //__STDC_SECURE_LIB__ //_MSC_VER < 1400  // less than VS2005
    strncpy_s(b, i, (char*)pValue, i);
#else
    strncpy(b, (char *)pValue, i);    
#endif

	i = data.Add(b);
	free(b);
	return i;
}

//////////////////////////////////////////////////////////////////////
//  AVariant Class Implementation
//////////////////////////////////////////////////////////////////////
#ifdef _USE_AVARIANT_

void AVariant::CopyFrom(const AVariant& v) 
{
	DataType  = v.DataType;
	AsDouble  = v.AsDouble;
	AsCurrency= v.AsCurrency;
	AsString  = v.AsString;
	AsInteger = v.AsInteger;
	AsDate    = v.AsDate;
}

inline void AVariant::Clear() 
{
	DataType   = vdNull;
	AsCurrency = AsDouble = AsDate = AsInteger = 0;
}

inline void AVariant::SetAsDouble(double v) 
{
	DataType = vdDouble;
	AsDouble = v;
}

inline void AVariant::SetAsCurrency(const currency v) 
{
	DataType   = vdCurrency;
	AsCurrency = v;
}

inline void AVariant::SetAsString(const std::string& v) 
{
	DataType = vdString;
	AsString = v;
}

inline void AVariant::SetAsString(char *v) 
{
	DataType = vdString;
	AsString = v;
}

inline void AVariant::SetAsInteger(const int v) 
{
	DataType  = vdInteger;
	AsInteger = v;
}

inline void AVariant::SetAsDate(int v) 
{
	DataType = vdDate;
	AsDate   = v;
}

void AVariant::operator +=(AVariant& v) 
{
	if(DataType == vdNull) 
	{
		CopyFrom(v);
		return;
	}

	if(v.DataType == vdNull)
		return;

	switch (DataType) 
	{
	case vdInteger:  
		AsInteger += v.AsInteger; 
		break;
	case vdDouble:   
		AsDouble += v.AsDouble; 
		break;
	case vdCurrency: 
		AsCurrency += v.AsCurrency; 
		break;
	case vdString:
		AsString += v.AsString;
		break;
	case vdDate:
		AsDate += v.AsDate;
		break;
	}
}

bool AVariant::HaveData() 
{
	switch (DataType) 
	{
	case vdDouble: 
		return AsDouble != 0;
	case vdCurrency: 
		return AsCurrency != 0;
	case vdString: 
		return AsString.length() != 0;
	case vdInteger: 
		return AsInteger != 0;
	case vdDate: 
		return AsDate != 0;
	}

	return false;
}

#endif //_USE_AVARIANT_


// splits string to array of strings using Delim as delimiter
void StringToArray(const std::string& str, THArrayString& arr, const char Delim /*= '\n'*/)
{
	std::string s;
	uint i = 0;

	while (i < str.length())
	{
		s = "";
		while (i < str.length())
		{
			if (str[i] == Delim)
			{
				i++;
				break;
			}
			s += str[i++];
		}

		if (s.length() > 0)
			arr.AddValue(s);
	}
}

//
std::string toString(const THArrayString& array)
{
	std::string res;
	res.reserve(static_cast<size_t>(100)*array.Count()); // to reduce number of memory re-allocations we assume that each string in array has 100 characters
	for (uint i = 0; i < array.Count(); i++)
	{
		res.append(array[i]);
	}

	return res;
}

