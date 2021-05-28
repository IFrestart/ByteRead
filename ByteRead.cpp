#include"ByteRead.h"
#include<stdio.h>
#include<cstring>
#include<sstream>
#include<iomanip>
#include<cmath>
#include <atlstr.h>
int  ByteReader::bytesToInt(byte* bytes, int size)

{

	int a = ((int)bytes[3]) & 0xFF;

	a |= (((int)bytes[2] << 8) & 0xFF00);

	a |= (((int)bytes[1] << 16) & 0xFF0000);

	a |= (((int)bytes[0] << 24) & 0xFF000000);

	return a;

}

void  ByteReader::XORBuf(char* pBuf, int nBufLen)
{
	for (int i = 0; i < nBufLen; ++i)
		pBuf[i] ^= 0x07;
}

std::string  ByteReader::ConvToString(unsigned char* data, int nLen)
{
	/*
		* using a ostringstream to convert the hash in a
		* hex string
		*/
	std::ostringstream os;
	for (int i = 0; i < nLen; ++i)
	{
		/*
			* set the width to 2
			*/
		os.width(2);

		/*
			* fill with 0
			*/
		os.fill('0');

		/*
			* conv to hex
			*/
		os << std::hex << static_cast<unsigned int>(data[i]);
	}

	/*
		* return as std::string
		*/
	return os.str();
}

void ByteReader::Skip(int nSkip)
{
	offset_ += nSkip;
}

int ByteReader::Find(const std::string& strFind, int nfind)
{
	int result = -1;
	unsigned char* bytes = (unsigned char*)strFind.c_str();
	int nFind = strFind.length();
	int nstart = nfind > begin_ ? nfind : begin_;
	if (nstart + nFind > limit_)
	{
		return result;
	}
	for (int i = nstart; i < limit_; i++)
	{
		if (CompareRange(i, nFind, bytes))
		{
			result = i;
			break;
		}
	}
	return result;
}

int ByteReader::FindChar(unsigned char* strFind, int nLen, int nfind)
{
	int result = -1;
	unsigned char* bytes = strFind;
	int nFind = nLen;
	int nstart = nfind > begin_ ? nfind : begin_;
	if (nstart + nFind > limit_)
	{
		return result;
	}
	for (int i = nstart; i < limit_; i++)
	{
		if (CompareRange(i, nFind, bytes))
		{
			result = i;
			break;
		}
	}
	return result;
}

bool ByteReader::CompareRange(int offset, int count, unsigned char value)
{
	for (int i = offset; i < offset + count; i++)
		if (do_buffer_[i] != value)
			return false;

	return true;
}

bool ByteReader::CompareRange(int offset, int count, unsigned char* value)
{
	if (len_ < offset + count)
		return false;

	for (int i = 0; i < count; i++)
	{
		if (do_buffer_[i + offset] != value[i])
		{
			return false;
		}
	}

	return true;
}

std::string ByteReader::GetString(int here, int size)
{
	std::string strRes;
	strRes.append((char*)do_buffer_ + here, size);
	for (int i = 0; i < strRes.size(); i++)
	{
		if (strRes[i] == '\0')
		{
			strRes[i] = ' ';
		}
	}
	return strRes;
}

byte ByteReader::GetByte(int at)
{
	auto here = advance(at, 1);
	return do_buffer_[here];
}

int ByteReader::advance(int at, int dist)
{
	auto here = (at == -1) ? offset_ : begin_ + at;
	if (at == -1)
		offset_ += dist;

	return here;
}

uint32_t ByteReader::GetUInt32()
{
	uint32_t n = 0;
	if (len_ < offset_ + 4)
		return false;
	memcpy(&n, recent_buffer_ + offset_, 4);
	return n;
}

uint32_t ByteReader::GetUInt32Inverted()
{
	uint32_t n = 0;
	if (len_ < offset_ + 4)
		return false;
	unsigned char cInt[4];
	cInt[0] = *(recent_buffer_ + offset_ + 3);
	cInt[1] = *(recent_buffer_ + offset_ + 2);
	cInt[2] = *(recent_buffer_ + offset_ + 1);
	cInt[3] = *(recent_buffer_ + offset_);
	memcpy(&n, cInt, 4);
	return n;
}

uint16_t ByteReader::GetUInt16()
{
	uint32_t n = 0;
	if (len_ < offset_ + 2)
		return false;
	memcpy(&n, recent_buffer_ + offset_, 2);
	return n;
}

uint16_t ByteReader::GetUInt16Inverted()
{
	uint32_t n = 0;
	if (len_ < offset_ + 2)
		return false;
	unsigned char cInt[2];
	cInt[0] = *(recent_buffer_ + offset_ + 1);
	cInt[1] = *(recent_buffer_ + offset_);
	memcpy(&n, cInt, 2);
	return n;
}

std::string ByteReader::Dec2hex(unsigned int i) //将int转成16进制字符串
{
	std::stringstream ioss; //定义字符串流
	std::string s_temp; //存放转化后字符
	//ioss << setiosflags(ios::uppercase) << hex << i; //以十六制(大写)形式输出
	ioss << std::resetiosflags(std::ios::uppercase) << std::hex << i; //以十六制(小写)形式输出//取消大写的设置
	ioss >> s_temp;
	return s_temp;
}

bool ByteReader::UnicodeToChinese(std::string str, std::wstring& cstr)
{
	int i = 0;
	int j = 0;
	int len = 0;

	len = str.length();
	if (len <= 0)
	{
		return false;
	}

	int nValue = 0;
	wchar_t* pWchar;
	wchar_t* szHex;
	char strchar[6] = { '0','x','\0' };

	for (i = 0; i < len; i++)
	{
		if (str[i] == 'u')
		{
			for (j = 2; j < 6; j++)
			{
				i++;
				strchar[j] = str[i];
			}
			USES_CONVERSION;
			szHex = A2W(strchar);
			StrToIntExW(szHex, STIF_SUPPORT_HEX, &nValue);
			pWchar = (wchar_t*)&nValue;

			cstr = cstr + pWchar;
		}
	}
	return true;
}

std::string ByteReader::HexToStr(const unsigned char* pbSrc, int nLen)
{
	if (NULL == pbSrc || nLen <= 0 || 0 != nLen % 2)
		return "";

	std::string res;
	for (int i = 0; i < nLen; i += 2)
	{
		char ch1[3] = { 0 }, ch2[3] = { 0 };
		sprintf(ch1, "%02x", (unsigned char)(*(pbSrc + i + 1)));
		sprintf(ch2, "%02x", (unsigned char)(*(pbSrc + i)));
		res.append("\\u");
		res.append(ch1);
		res.append(ch2);
	}
	return res;
}


std::string ByteReader::GetWholeString()
{
	std::string strRes;
	strRes.append((const char*)recent_buffer_, len_ - (recent_buffer_ - do_buffer_));
	recent_buffer_ = do_buffer_ + len_;
	return strRes;
}
