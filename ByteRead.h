#pragma once
#include <unordered_map>
#include<iostream>

class ByteReader
{
public:
	ByteReader(unsigned char* p, int nLen)
		: do_buffer_(p), len_(nLen), recent_buffer_(p) {
		begin_ = 0;
		count_ = nLen;
		limit_ = nLen;
		offset_ = 0;
	}
	unsigned char GetByte(int at = -1);
	int advance(int at, int dist);
	uint32_t GetUInt32();
	uint32_t GetUInt32Inverted();
	uint16_t GetUInt16();
	uint16_t GetUInt16Inverted();
	std::string Dec2hex(unsigned int i);
	bool UnicodeToChinese(std::string str, std::wstring& cstr);
	std::string HexToStr(const unsigned char* pbSrc, int nLen);
	std::string GetString(int here, int size);
	std::string GetWholeString();
	std::string GetUTF8(int size, int at = -1);
	int bytesToInt(unsigned char* bytes, int size);
	void XORBuf(char* pBuf, int nBufLen);
	std::string ConvToString(unsigned char* data, int nLen);
	void Skip(int nSkip);
	int Find(const std::string& find, int nfind = 0);
	int FindChar(unsigned char* find, int nLen, int nfind = 0);

	bool CompareRange(int offset, int count, unsigned char value);
	bool CompareRange(int offset, int count, unsigned char* value);

	unsigned char* recent_buffer_;
	unsigned char* do_buffer_;
	int len_;
	int begin_, count_, limit_, offset_;

};
