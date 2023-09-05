/*
 * FileStream.h
 *
 * Copyright 2003, LogEngine Project. All rights reserved.
 *
 * See the COPYING file for the terms of usage and distribution.
 */

#ifndef _stream_
#define _stream_

#ifndef WIN32
#include <string.h>
#endif

#include <string>
#include <exception>

#define IO_EXCEPTION_PREFIX "LogException : "

enum TFileMode {fmRead,fmWrite,fmReadWrite};
enum TSeekMode {smFromBegin,smFromEnd,smFromCurrent};

class IOException : public std::exception
{
public:
	IOException(const char * Message)       { Text = Message; whatText = IO_EXCEPTION_PREFIX + std::string(Message); }
	IOException(const std::string& Message) { Text = Message; whatText = IO_EXCEPTION_PREFIX + Message; }
	IOException(const IOException& ex)      { Text = ex.Text; whatText = ex.whatText; }
	virtual ~IOException() noexcept {};
	IOException& operator=(const IOException& rhs);
	virtual const char* what() const throw(){ return whatText.c_str(); }
	//std::string GetError(void);
private:
	std::string Text;
	std::string whatText;
};


class TStream
{
public:
	virtual int Read(void *Buffer, size_t Size) = 0;
	virtual int Write(const void *Buffer, const size_t Size) = 0;
	virtual long Length() = 0;
	void operator >>(bool& Value); 
	void operator >>(int& Value); 
	void operator <<(int Value); 
	void operator <<(double Value) ;
	void operator <<(const char *Value);
	void operator <<(std::string& Value);
	void operator >>(std::string& Value);
	char ReadChar();
	std::string LoadPString();
};

/*class TMemoryStream:public TStream
{
    void *Data;
    int  len;
public:
    TMemoryStream();
    virtual ~TMemoryStream();
    int Read(void *Buffer,int Size);
    int Write(void *Buffer,int Size);
	inline int Length() {return len;}
};
*/

class TFileStream:public TStream 
{
private:
	std::string FFileName;
	TFileMode FFileMode;
	int hf;  // file handle
public:
	TFileStream(const std::string& FileName, const TFileMode fMode);
	virtual ~TFileStream();
	int Read(void *Buffer, size_t Size);
	int Write(const void *Buffer, const size_t Size);
	int WriteString(const std::string& str);
	int WriteLn(const void *Buffer, const size_t Size);
	int WriteCRLF(void);
	off_t Length();
	void Flush();

/* Moves the current position in the file.
 * When sMode=smFromEnd the current position moves _back_ (to the beginning).
 * Negative values of Offset parameter are allowed only when sMode=smFromCurrent.
 *
 */
	off_t Seek(const off_t Offset, const TSeekMode sMode);

};

#endif
