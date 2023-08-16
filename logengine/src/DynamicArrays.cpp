#include <stdlib.h>
#include <stdio.h>
#include "DynamicArrays.h"

//using namespace std;

// splits string to array of strings using Delim as delimiter
void StringToArray(const std::string& str, THArray<std::string>& arr, const char Delim /*='\n'*/)
{
	std::string s;
	unsigned int i = 0;
	
	while(i < str.length())
	{
		s = "";
		while(i < str.length())
		{
			if(str[i] == Delim)
			{
				i++;
				break;
			}
			s += str[i++];
		}
		
		if(s.length() > 0)
			arr.AddValue(s);
		
	}
}


//////////////////////////////////////////////////////////////////////
//  THArrayException Class Implementation
//////////////////////////////////////////////////////////////////////

const char* THArrayException::what() const throw()
{
	std::string s = getErrorMessage();
	char * err = (char*)malloc(s.size() + 1); //on extra byte for
	if(err) // don't want to include <string.h>
	{
		for(unsigned int i = 0; i < s.size(); i++)
			err[i] = s[i];
		err[s.size()] = '\0';
	}
		
	return err;
};

//////////////////////////////////////////////////////////////////////
//  THArrayRaw Class Implementation
//////////////////////////////////////////////////////////////////////

THArrayRaw::THArrayRaw()
{
    Sorted    = false;
    FCount    = 0;
    FCapacity = 0;
    FItemSize = 1;
    FMemory   = NULL;
}

THArrayRaw::THArrayRaw(int ItemSize)
{
    Sorted    = false;
    FCount    = 0;
    FCapacity = 0;
    FMemory   = NULL;
	if(ItemSize > 0)
		FItemSize = ItemSize;
	else
	{
		throw THArrayException("Error in THArrayRaw: Cannot put zero (0) item size into constructor!");
	}

}

/*void THArrayRaw::operator=(const THArrayRaw& a) {
	ClearMem();
	FItemSize=a.FItemSize;
	SetCapacity(a.FCount);
	memmove(FMemory,a.FMemory,FCount*FItemSize);
	Sorted=a.Sorted;
}*/

void THArrayRaw::Error(const int Value, const int vmin, const int vmax) const
{
	if((vmin > Value) || (vmax < Value)) 
	{
		char str[512];
#if __STDC_SECURE_LIB__ //_MSC_VER < 1400
        sprintf_s(str, 512, "Error in HArray: Element with index %i not found!", Value);		
#else
        sprintf(str, "Error in HArray: Element with index %i not found!", Value);        
#endif
		throw THArrayException(str);
	}
}

inline void* THArrayRaw::CalcAddr(const int num) const 
{
	return (unsigned char *)FMemory + num * FItemSize;
}

void THArrayRaw::SetItemSize(const int Size)
{
    ClearMem();
    if(Size > 0/*1*/)
		FItemSize = Size;
}

void THArrayRaw::Delete(const int num)
{
	Error(num, 0, FCount - 1);
	if (num < (FCount - 1))
		memmove(GetAddr(num), GetAddr(num + 1), (FCount - num - 1) * FItemSize);
	FCount--;
}

void THArrayRaw::ClearMem()
{
	Clear();
	FCapacity = 0;
	free(FMemory);
	FMemory = NULL;
}

void THArrayRaw::Get(const int num, void *pValue) const
{
	Error(num, 0, FCount - 1);
	if (pValue != NULL) 
		memmove(pValue, CalcAddr(num), FItemSize);
	return;
}

int  THArrayRaw::Add(const void *pValue)
{
    Sorted = false;
	return Insert(FCount, pValue);
}

void THArrayRaw::AddMany(const void *pValue, const int Count)
{
    Sorted = false;
	if (Count <= 0) 
	{ 
		char str[512];
#if __STDC_SECURE_LIB__//_MSC_VER < 1400
		sprintf_s(str, 512, "AddMany(): invalid parameter 'Count'=%i !", Count);
#else
        sprintf(str, "AddMany(): invalid parameter 'Count'=%i !", Count);        
#endif  
		throw THArrayException(str);
	}
    InsertMany(FCount, pValue, Count);
}

int	 THArrayRaw::Insert(const int num, const void *pValue)
{
	Error(num, 0, FCount);
	if (FCount >= FCapacity) 
		Grow();
	FCount++;
	memmove(CalcAddr(num + 1), CalcAddr(num), (FCount - num - 1) * FItemSize);
	Update(num, pValue);
    Sorted = false;
	return num;
}

void THArrayRaw::InsertMany(const int num,const void *pValue,const int Count)
{
	Error(num, 0, FCount);
	if ((FCount + Count) > FCapacity) 
		GrowTo(FCount + Count);

	FCount=FCount + Count;
	memmove(CalcAddr(num + Count),CalcAddr(num),(FCount-num-Count)*FItemSize);
    Sorted = false;
	UpdateMany(num, pValue, Count);
}

void THArrayRaw::Update(const int num, const void *pValue)
{
	Error(num, 0, FCount - 1);
	if (pValue != NULL) 
		memmove(CalcAddr(num), pValue, FItemSize);
	else
		memset(CalcAddr(num), 0, FItemSize);
    Sorted = false;
}

void THArrayRaw::UpdateMany(const int num, const void *pValue, const int Count)
{
	Error(num + Count, 0, FCount);
	memmove(GetAddr(num), pValue, FItemSize * Count);
    Sorted = false;
}

void THArrayRaw::Grow()
{
	int Delta;

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

void THArrayRaw::GrowTo(const int Count)
{
	int Delta;

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

void THArrayRaw::SetCapacity(const int Value)
{
	if(Value > 0)
	{
		FMemory = realloc(FMemory, Value * FItemSize);
		FCapacity = Value;
	} 
	else 
	{
		free(FMemory);
		FMemory = NULL;
	}

  if(FCount > FCapacity)
	  FCount = FCapacity;
}

void THArrayRaw::AddFillValues(const int Count)
{
    if ((FCount + Count) > FCapacity)
		GrowTo(FCount + Count);
    memset(CalcAddr(FCount), 0, Count * FItemSize);
    FCount = FCount + Count;
    Sorted = false;
}

void THArrayRaw::Zero()
{
    if (FCount > 0)
        memset(FMemory, 0, FCount * FItemSize);
}

void THArrayRaw::Hold()
{
    SetCapacity(FCount);
}

void THArrayRaw::Swap(const int Index1, const int Index2)
{
	Error(Index1, 0, FCount - 1);
	Error(Index2, 0, FCount - 1);

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
	for (int i = 0; i < data.Count()/2; i++)
	{
		temp = GetValue(i);
		data.Update(i, (void*)GetValue(data.Count() - 1 - i).data());
		data.Update(data.Count() - 1 - i, (void*)temp.data());
	}
}

char* THArrayStringFix::GetAddr(const int Index) const
{
	return (char *)(data.GetAddr(Index));
}

std::string THArrayStringFix::operator [](const int Index) const
{
	return GetValue(Index);
}

void THArrayStringFix::SetValue(const int Index, const std::string& Value)
{
	data.Update(Index, Value.c_str());
}

std::string THArrayStringFix::GetValue(const int Index) const 
{
	std::string s(GetAddr(Index), data.GetItemSize());
	return s;
}

int THArrayStringFix::AddChars(const void *pValue, const int len) 
{
	int i;
	char *b;

	b = (char *)malloc(data.GetItemSize());
	memset(b, 0, data.GetItemSize());

	i = valuemin(len, data.GetItemSize());

#if __STDC_SECURE_LIB__ //_MSC_VER < 1400  // less than VS2005
    strncpy_s(b, i, (char *)pValue, i);
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

