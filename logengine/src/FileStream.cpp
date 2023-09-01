/*
 * FileStream.cpp
 *
 * Copyright 2003, LogEngine Project. All rights reserved.
 *
 * See the COPYING file for the terms of usage and distribution.
 */

#include <sys/stat.h>
#include <fcntl.h>

#ifdef __unix__
#include <sys/io.h>
#include <unistd.h>
#else
#include <io.h>
#endif


#include "FileStream.h"

//////////////////////////////////////////////////////////////////////
//  IOException Class
//////////////////////////////////////////////////////////////////////


//IOException::IOException(const char * Message)
//{
//	Text = Message; 
//}
//
//IOException::IOException(const std::string& Message)
//{
//	Text = Message;
//}
//
//IOException::IOException(const IOException& ex)
//{
//	Text = ex.Text;
//}
//
//IOException::~IOException() throw()
//{
//	// nothing done
//}

const char* IOException::what() const throw()
{
	std::string s ="LogException : "+Text;
	char * err = (char*)malloc(s.size() + 1); //on extra byte for terminator
	if(err) // don't want to include <string.h>
	{
		for(unsigned int i = 0; i < s.size(); i++)
			err[i] = s[i];
		err[s.size()] = '\0';
	}
	return err;
}


IOException& IOException::operator=(const IOException& ex)
{
	Text = ex.Text;
	return *this;
}


//////////////////////////////////////////////////////////////////////
//  TStream Class
//////////////////////////////////////////////////////////////////////

void TStream::operator >>(bool& Value) 
{
	if (Read(&Value,sizeof(Value)) != sizeof(Value))
		throw IOException("End of stream !");
}

void TStream::operator >>(int& Value) 
{
	if (Read(&Value,sizeof(Value)) != sizeof(Value))
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
void TStream::operator <<(std::string& Value) 
{
	Write((void*)Value.data(),Value.length());
	Write((void*)"\r\n", 2u);
}
char TStream::ReadChar() 
{
	char c;
	Read(&c,sizeof(c));
	return c;
}
void TStream::operator >>(std::string& Value) 
{
	char c; 
	Value.clear(); // 16.08.23 change
	while ((c=ReadChar()) != '\n') Value+=c;
	ReadChar();
}
std::string TStream::LoadPString() 
{
	std::string res;
	int i;
	*this >> i;
	res.resize(i);
	Read((void*)res.data(),res.length());
	return res;
}

//////////////////////////////////////////////////////////////////////
//  TMemoryStream Class
//////////////////////////////////////////////////////////////////////

/*TMemoryStream::TMemoryStream()
{
    Data=NULL;
    len=0;
}

TMemoryStream::~TMemoryStream()
{
    free(Data);
}

int TMemoryStream::Read(void *Buffer,int Size)
{
    if (Size<=0) return 0;
    if (Size>len) Size=len;
    memmove(Buffer,Data,Size);
    memmove(Data,(byte *)Data+Size,len-Size);
    len-=Size;
    return Size;
}

int TMemoryStream::Write(void *Buffer,int Size)
{
    if (Size<=0) return 0;
    Data=realloc(Data,len+Size);
    memmove((byte *)Data+len,Buffer,Size);
    len+=Size;
    return Size;
}
*/

//////////////////////////////////////////////////////////////////////
//  TFileStream Class
//////////////////////////////////////////////////////////////////////

TFileStream::TFileStream(const std::string& FileName, const TFileMode fMode)
{
	FFileName = FileName;
	FFileMode = fMode;
		

	//_fmode=O_BINARY;
#ifdef WIN32
	switch (fMode)
	{ 
	case fmRead:     hf = _open(FFileName.c_str(), O_RDONLY | O_BINARY);break; 
	case fmWrite:    hf = _open(FFileName.c_str(), O_WRONLY | O_CREAT|O_BINARY, S_IREAD | S_IWRITE);break;
	case fmReadWrite:hf = _open(FFileName.c_str(), O_RDWR | O_CREAT | O_BINARY, S_IREAD | S_IWRITE);break;
	}
#else
	switch (fMode)
	{ 
	case fmRead:     hf = open(FFileName.c_str(), O_RDONLY);break;
	case fmWrite:    hf = open(FFileName.c_str(), O_WRONLY | O_CREAT,S_IREAD | S_IWRITE);break;
	case fmReadWrite:hf = open(FFileName.c_str(), O_RDWR | O_CREAT,S_IREAD | S_IWRITE);break;
	}
	
#endif

	
	if(hf == -1)
	{
		std::string s = "Can't open file '" + FileName + "'!";
		throw IOException(s.c_str());
	}
	
}

TFileStream::~TFileStream()
{
	close(hf);
	hf = 0;
}

int TFileStream::Read(void *Buffer, size_t Size)
{
	if(FFileMode == fmWrite)
		throw IOException("File opened in write-only mode. Can't read!");
	
	int c = read(hf, Buffer, Size);
	
	if(c == -1)
	{
		std::string s = "Cannot read from file '" + FFileName + "'! May be file closed?";
		throw IOException(s.c_str());
	}

	return c;
}

int TFileStream::WriteCRLF(void)
{
	const char *temp = "\n";
	return Write(temp, strlen(temp));
}

int TFileStream::Write(const void *Buffer, const size_t Size)
{
	if(FFileMode == fmRead)
		throw IOException("File opened in read-only mode. Can't write!");

	int c = write(hf, Buffer, Size);
	
	if(c == -1)
	{
		std::string s = "Cannot write to file '"+FFileName+"'! May be disk full?";	
		throw IOException(s.c_str());
	}

	return c;
}

int TFileStream::WriteLn(const void *Buffer, const size_t Size)
{
	int c = Write(Buffer, Size);
	return WriteCRLF() + c;
}

int TFileStream::WriteString(const std::string& str)
{
	return Write((void*)str.data(), str.length()/*strlen(str.data())*/);
}

int TFileStream::Seek(int Offset, TSeekMode sMode)
{
	switch (sMode)
	{
	case smFromBegin:   return lseek(hf, Offset, SEEK_SET);
	case smFromEnd:     return lseek(hf, Offset, SEEK_END);
	case smFromCurrent: return lseek(hf, Offset, SEEK_CUR);
	}

	throw IOException("Invalid seek mode.");
}

int TFileStream::Length() 
{
	struct stat st;
	/*int g=Seek(0,smFromCurrent);
	
	Seek(0,smFromEnd);
	int r=Seek(0,smFromCurrent);*/
	
	fstat(hf, &st);
	return st.st_size;

	//Seek(g,smFromBegin);

	//return r;
};

void TFileStream::Flush()
{
#ifdef __unix__
	fsync(hf);
#else
	_commit(hf);
#endif /* __unix__ */
}
