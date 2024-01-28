/*
 * FileStream.cpp
 *
 * Copyright 2003, LogEngine Project. All rights reserved.
 *
 * See the COPYING file for the terms of usage and distribution.
 */

#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>

#ifdef WIN32
#include <io.h>
#else
#include <sys/io.h>
#include <unistd.h>
#endif

#include "FileStream.h"

using namespace std;

//////////////////////////////////////////////////////////////////////
//  TStream Class
//////////////////////////////////////////////////////////////////////

void TStream::operator >>(bool& Value) 
{
	if (Read(&Value, sizeof(Value)) != sizeof(Value))
		throw IOException("End of stream !");
}

void TStream::operator >>(int& Value) 
{
	if (Read(&Value, sizeof(Value)) != sizeof(Value))
		throw IOException("End of stream !");
}
void TStream::operator <<(int Value) 
{
	Write(&Value,sizeof(Value));
}
void TStream::operator <<(double Value) 
{
	Write(&Value,sizeof(Value));
}

void TStream::operator <<(const char *Value) 
{
	Write(Value, strlen(Value));
	Write((void*)"\r\n", 2u);
}
void TStream::operator <<(string& Value) 
{
	Write((void*)Value.data(), Value.length());
	Write((void*)"\r\n", 2u);
}
char TStream::ReadChar() 
{
	char c;
	Read(&c, sizeof(c));
	return c;
}
void TStream::operator >>(string& Value) 
{
	char c; 
	Value.clear(); // 16.08.23 change
	while ((c=ReadChar()) != '\n') Value+=c;
	ReadChar();
}
string TStream::LoadPString() 
{
	string res;
	int i;
	*this >> i;
	res.resize(i);
	Read((void*)res.data(), res.length());
	return res;
}

//////////////////////////////////////////////////////////////////////
//  TMemoryStream Class
//////////////////////////////////////////////////////////////////////

TMemoryStream::TMemoryStream()
{
    FMemory = nullptr;
    FCount = 0;
	FRPos = 0;
	FWPos = 0;
}

TMemoryStream::~TMemoryStream()
{
	delete [] FMemory;
}

int TMemoryStream::Read(void *Buffer, size_t Size)
{
    if (Size == 0) return 0;

    if (FRPos + Size > FCount) Size = FCount - FRPos;
    memmove(Buffer, FMemory, Size);
    memmove(FMemory, (char *)FMemory + Size, FCount - Size);
    FRPos += Size;
    return (int)Size;
}

int TMemoryStream::Write(const void *Buffer, const size_t Size)
{
    if (Size == 0) return 0;

	if (FWPos + Size > FCount) 
		FMemory = (uint8_t*)realloc(FMemory, FCount + Size - FWPos);
    memmove((char *)FMemory + FWPos, Buffer, Size);
    FWPos += Size;
    return (int)Size;
}


//////////////////////////////////////////////////////////////////////
//  TFileStream Class
//////////////////////////////////////////////////////////////////////

#ifdef WIN32
#define mylseek _lseek
#define myread _read
#define mywrite _write
#define myclose _close
#else
#define mylseek lseek
#define myread read
#define mywrite write
#define myclose close
#endif

TFileStream::TFileStream(const string& FileName, const TFileMode fMode)
{
	FFileName = FileName;
	FFileMode = fMode;
	hf = 0;
	
#if defined(WIN32) && !defined(__BORLANDC__)
	errno_t res = 0;
	switch (fMode)
	{ 
	case fmRead:     res = _sopen_s(&hf, FFileName.c_str(), O_RDONLY | O_BINARY,           _SH_DENYNO, _S_IREAD | _S_IWRITE); break;
	case fmWrite:    res = _sopen_s(&hf, FFileName.c_str(), O_WRONLY | O_CREAT | O_BINARY, _SH_DENYWR, _S_IREAD | _S_IWRITE); break;
	case fmReadWrite:res = _sopen_s(&hf, FFileName.c_str(), O_RDWR   | O_CREAT | O_BINARY, _SH_DENYWR, _S_IREAD | _S_IWRITE); break;
	}
#else
	switch (fMode)
	{ 
	case fmRead:     hf = open(FFileName.c_str(), O_RDONLY);break;
	case fmWrite:    hf = open(FFileName.c_str(), O_WRONLY | O_CREAT, S_IREAD | S_IWRITE); break;
	case fmReadWrite:hf = open(FFileName.c_str(), O_RDWR   | O_CREAT, S_IREAD | S_IWRITE); break;
	}

	int res = errno;
#endif

	if(hf == -1)
	{
		string s;
		if(res == EINVAL) 
			s = "Wrong file name '" + FileName + "'!";
		else if (res == EACCES) 
			s = "Can't get access to file '" + FileName + "'!";
		else 
			s = "Can't open file '" + FileName + "'!";
		
		throw IOException(s.c_str());
	}
}

TFileStream::~TFileStream()
{
	myclose(hf);
	hf = 0;
}

int TFileStream::Read(void *Buffer, size_t Size)
{
	if(FFileMode == fmWrite)
		throw IOException("File opened in write-only mode. Can't read!");
	
	int c = myread(hf, Buffer, (uint)Size);
	
	if(c == -1)
	{
		string s = "Cannot read from file '" + FFileName + "'! May be file closed?";
		throw IOException(s.c_str());
	}

	return c;
}

int TFileStream::WriteCRLF(void)
{
	const char *temp = "\n";
	return Write(temp, strlen(temp));
}

int TFileStream::Write(const string& str)
{
	return Write(str.data(), str.length());
}

int TFileStream::Write(const void *Buffer, const size_t Size)
{
	if(FFileMode == fmRead)
		throw IOException("File opened in read-only mode. Can't write!");

	int c = mywrite(hf, Buffer, (uint)Size);
	
	if(c == -1)
	{
		string s = "Cannot write to file '" + FFileName + "'! May be disk full?";	
		throw IOException(s.c_str());
	}

	return c;
}

int TFileStream::WriteLn(const string& str)
{
	int c = Write(str);
	return WriteCRLF() + c;
}

int TFileStream::WriteLn(const void *Buffer, const size_t Size)
{
	int c = Write(Buffer, Size);
	return WriteCRLF() + c; // TODO why do we sum up two return values???
}

//int TFileStream::WriteString(const string& str)
//{
//	return Write((void*)str.data(), str.length()/*strlen(str.data())*/);
//}


off_t TFileStream::Seek(off_t Offset, TSeekMode sMode)
{
	switch (sMode)
	{
	case smFromBegin:   return mylseek(hf, Offset, SEEK_SET);
	case smFromEnd:     return mylseek(hf, Offset, SEEK_END);
	case smFromCurrent: return mylseek(hf, Offset, SEEK_CUR);
	}

	throw IOException("Invalid Seek() mode.");
}

size_t TFileStream::Length() 
{
	struct stat st;
	fstat(hf, &st);
	return st.st_size;
};

void TFileStream::Flush()
{
#ifdef WIN32
	_commit(hf);
#else
	fsync(hf); 
#endif
}

