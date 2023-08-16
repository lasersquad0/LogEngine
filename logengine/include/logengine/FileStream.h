/*
 * FileStream.h
 *
 * Copyright 2003, LogEngine Project. All rights reserved.
 *
 * See the COPYING file for the terms of usage and distribution.
 */

#ifndef _stream_
#define _stream_

#include <string>
#include <exception>

enum TFileMode {fmRead,fmWrite,fmReadWrite};
enum TSeekMode {smFromBegin,smFromEnd,smFromCurrent};

class IOException : public std::exception
{
public:
	IOException(const char * Message);
	IOException(const std::string& Message);
	IOException(const IOException& ex);
	virtual ~IOException() throw();
	IOException& operator=(const IOException& rhs);
	virtual const char *what() const throw();
	//std::string GetError(void);
private:
	std::string Text;
};


class TStream
{
public:
	virtual int Read(void *Buffer, int Size) = 0;
	virtual int Write(const void *Buffer, const int Size) = 0;
	virtual int Length() = 0;
	void operator >>(bool& Value); 
	void operator >>(int& Value); 
	void operator <<(int Value); 
	void operator <<(double Value) ;
	void operator <<(char *Value);
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
	int Read(void *Buffer, int Size);
	int Write(const void *Buffer, const int Size);
	int WriteString(const std::string& str);
	int WriteLn(const void *Buffer, const int Size);
	int WriteCRLF(void);
	int Length();
	void Flush();

/* Moves the current position in the file.
 * When sMode=smFromEnd the current position moves _back_ (to the beginning).
 * Negative values of Offset parameter are allowed only when sMode=smFromCurrent.
 *
 */
	int Seek(const int Offset, const TSeekMode sMode);

};

#endif
