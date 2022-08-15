#include <iostream>
#include <string>
#include <cstring>
#include <stdlib.h>
using namespace std;

//常数表T
const unsigned T[] =
{
	0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee,
	0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501,
	0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be,
	0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821,
	0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa,
	0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8,
	0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed,
	0xa9e3e905, 0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a,
	0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c,
	0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70,
	0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x04881d05,
	0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665,
	0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039,
	0x655b59c3, 0x8f0ccc92, 0xffeff47d, 0x85845dd1,
	0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1,
	0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391
};

//压缩函数每步左循环移位位数
const unsigned Lmove[] =
{
	7, 12, 17, 22,  7, 12, 17, 22,  7, 12, 17, 22,  7, 12, 17, 22,
	5,  9, 14, 20,  5,  9, 14, 20,  5,  9, 14, 20,  5,  9, 14, 20,
	4, 11, 16, 23,  4, 11, 16, 23,  4, 11, 16, 23,  4, 11, 16, 23,
	6, 10, 15, 21,  6, 10, 15, 21,  6, 10, 15, 21,  6, 10, 15, 21
};

//x循环左移n位
#define shift(x,n) (( x << n ) | ( x >> ( 32 - n )))

//用实际值表示ABCD4个初始化的链接变量
unsigned int A = 0x67452301;
unsigned int B = 0xefcdab89;
unsigned int C = 0x98badcfe;
unsigned int D = 0x10325476;
//存储时，A是01234567的形式

//FGHI4个基本逻辑函数
#define F(b,c,d) (( b & c ) | (( ~b ) & ( d )))
#define G(b,c,d) (( b & d ) | ( c & ( ~d )))
#define H(b,c,d) ( b ^ c ^ d )
#define I(b,c,d) ( c ^ ( b | ( ~d )))



string inttohex(int n)//整数转换为十六进制字符串
{
	/*unsigned hex; string temp;
	for (int i = 0; i < 4; i++)
	{
		temp = "";
		hex = (n >> (i * 8)) & 0xff;
	}
	char s[1000000] = {0};
	sprintf_s(s, "%x", hex);
	return s;*/
	const char str16[] = "0123456789abcdef";
	unsigned hex;

	string temp;
	string s_hex = "";

	for (int i = 0; i < 4; i++)
	{
		temp = "";
		hex = (n >> (i * 8)) & 0xff;

		for (int j = 0; j < 2; j++)
		{
			temp.insert(0, 1, str16[hex % 16]);
			hex /= 16;
		}

		s_hex += temp;
	}
	return s_hex;
	
}
string md5(string m)//对明文m进行hash
{
	int mlength = m.length();//明文长度
	int mgroup = ((mlength + 8) / 64) + 1;//明文分组个数

	unsigned int* mbit = new unsigned int[mgroup * 16];
	memset(mbit, 0, sizeof(unsigned int) * mgroup * 16);//内存初始化

	for (int i = 0; i < mlength; i++)
	{
		mbit[i / 4] |= m[i] << ((i % 4) * 8);
	}
	mbit[mlength >> 2] |= 0x80 << ((mlength % 4) * 8);

	mbit[mgroup * 16 - 2] = mlength * 8;

	//分组处理消息
	unsigned int a, b, c, d;
	for (int i = 0; i < mgroup; i++)
	{
		a = A;
		b = B;
		c = C;
		d = D;
		unsigned int x1;
		int x2;

		for (int j = 0; j < 64; j++)//4个16步迭代操作
		{
			if (j < 16)
			{
				x1 = F(b, c, d);//逻辑函数依次为FGHI
				x2 = j;//对字进行置换 
			}
			else if (j >= 16 && j < 32)
			{
				x1 = G(b, c, d);
				x2 = (1 + 5 * j) % 16;
			}
			else if (j >= 32 && j < 48)
			{
				x1 = H(b, c, d);
				x2 = (5 + 3 * j) % 16;
			}
			else if (j >= 48 && j < 64)
			{
				x1 = I(b, c, d);
				x2 = (7 * j) % 16;
			}
			//压缩函数的迭代
			unsigned temp = d;
			d = c;
			c = b;
			b = b + shift(a + x1 + mbit[i * 16 + x2] + T[j], Lmove[j]);
			a = temp;
		}
		A = a + A;
		B = b + B;
		C = c + C;
		D = d + D;
	}
	return inttohex(A) + inttohex(B) + inttohex(C) + inttohex(D);//将hash用16进制形式输出
}

