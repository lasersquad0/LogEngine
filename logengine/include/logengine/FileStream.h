/*
 * FileStream.h
 *
 * Copyright 2003, LogEngine Project. All rights reserved.
 *
 * See the COPYING file for the terms of usage and distribution.
 */

#ifndef FILESTREAM_H
#define FILESTREAM_H

//#ifndef WIN32 // required for Linux
//#include <string.h>
//#endif

#include <string>
#include <exception>
#include "functions.h"

#define IO_EXCEPTION_PREFIX "LogException : "

enum TFileMode { fmRead, fmWrite, fmReadWrite };
enum TSeekMode { smFromBegin, smFromEnd, smFromCurrent };

class IOException : public std::exception
{
public:
	IOException(const char * Message)       { Text = Message; whatText = IO_EXCEPTION_PREFIX + std::string(Message); }
	IOException(const std::string& Message) { Text = Message; whatText = IO_EXCEPTION_PREFIX + Message; }
	IOException(const IOException& ex)      { Text = ex.Text; whatText = ex.whatText; }
	~IOException() noexcept override {}
	IOException& operator=(const IOException& rhs);
	const char* what() const noexcept override { return whatText.c_str(); }

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
	virtual size_t Length() = 0;
	virtual off_t Seek(const off_t Offset, const TSeekMode sMode) = 0;
	virtual char ReadChar();
	virtual std::string LoadPString(); 
	
	void operator >>(bool& Value);
	void operator >>(int& Value); 
	void operator <<(int Value); 
	void operator <<(double Value) ;
	void operator <<(const char *Value);
	void operator <<(std::string& Value);
	void operator >>(std::string& Value);
};

class TMemoryStream : public TStream
{
private:
    uint8_t *FMemory;
    size_t FCount;
	size_t FRPos;
	size_t FWPos;
public:
    TMemoryStream();
    virtual ~TMemoryStream();
    int Read(void *Buffer, size_t Size) override;
    int Write(const void *Buffer, const size_t Size) override;
	inline size_t Length() override { return FCount; }
};


class TFileStream:public TStream 
{
private:
	std::string FFileName;
	TFileMode FFileMode;
	int hf;  // file handle
public:
	TFileStream(const std::string& FileName, const TFileMode fMode = fmWrite);
	virtual ~TFileStream();
	int Read(void *Buffer, size_t Size) override;
	int Write(const void *Buffer, const size_t Size) override;
	int Write(const std::string& str);
	int WriteLn(const void *Buffer, const size_t Size);
	int WriteCRLF(void);
	size_t Length() override;
	void Flush();

/* Moves the current position in the file.
 * When sMode=smFromEnd the current position moves _back_ (to the beginning).
 * Negative values of Offset parameter are allowed only when sMode=smFromCurrent.
 */
	off_t Seek(const off_t Offset, const TSeekMode sMode) override;

};


#endif //FILESTREAM_H
