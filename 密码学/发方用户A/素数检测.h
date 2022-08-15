#pragma once
#include "大数定义类.h"
#include<iostream>
//#include<NTL/ZZ.h>//C++大数标准算法库
#include<math.h>
#include<stdlib.h>
#include<windows.h>
using namespace std;
//using namespace NTL;

const static int prime_table[] = {
2,3,5,7,11,13,17,19,23,29,31,37,41,43,47,53,59,61,67,71,
73,79,83,89,97,101,103,107,109,113,127,131,137,139,149,151,157,163,167,173,
179,181,191,193,197,199,211,223,227,229,233,239,241,251,257,263,269,271,277,281,
283,293,307,311,313,317,331,337,347,349,353,359,367,373,379,383,389,397,401,409,
419,421,431,433,439,443,449,457,461,463,467,479,487,491,499,503,509,521,523,541,
547,557,563,569,571,577,587,593,599,601,607,613,617,619,631,641,643,647,653,659,
661,673,677,683,691,701,709,719,727,733,739,743,751,757,761,769,773,787,797,809,
811,821,823,827,829,839,853,857,859,863,877,881,883,887,907,911,919,929,937,941,
947,953,967,971,977,983,991,997,1009,1013,1019,1021,1031,1033,1039,1049,1051,1061,1063,1069,
1087,1091,1093,1097,1103,1109,1117,1123,1129,1151,1153,1163,1171,1181,1187,1193,1201,1213,1217,1223,
1229,1231,1237,1249,1259,1277,1279,1283,1289,1291,1297,1301,1303,1307,1319,1321,1327,1361,1367,1373,
1381,1399,1409,1423,1427,1429,1433,1439,1447,1451,1453,1459,1471,1481,1483,1487,1489,1493,1499,1511,
1523,1531,1543,1549,1553,1559,1567,1571,1579,1583,1597,1601,1607,1609,1613,1619,1621,1627,1637,1657,
1663,1667,1669,1693,1697,1699,1709,1721,1723,1733,1741,1747,1753,1759,1777,1783,1787,1789,1801,1811,
1823,1831,1847,1861,1867,1871,1873,1877,1879,1889,1901,1907,1913,1931,1933,1949,1951,1973,1979,1987,
1993,1997,1999

};//2000以内的素数

//产生一个随机大数,大数的LENGTH为SIZE的1/4,是为了防止运算时的溢出
void bignumber::Random()
{
	for (int i = 0; i < 16; i++)//64/4=16
		//RAND()最多0X7FFF的数，需要3次调用
		data[i] = (rand() << 17) + (rand() << 2) + rand() % 4;
}

//产生一个可能是素数的大数,
//此数为奇数,且不能被小于2000的素数整除时，进行rabin检测
void makeprime(bignumber& n)
{
	int i = 0;
	bignumber test;
	const int length = sizeof(prime_table) / sizeof(int);
	while (i != length)
	{
		n.Random();
		while (!n.IsOdd())
			n.Random();

		i = 0;
		for (; i < length; i++)
		{
			test = prime_table[i];
			if ((n % test) == 0)
				break;
		}
	}
}
string ml,yl;
//对大奇数n进行RabinMiller检测
bool RabinMiller(const bignumber& n)//课本P94算法描述
{
	bignumber x, y, z;
	unsigned int p, q;
	x = n - 1;
	p = 0;

	while (!x.IsOdd())//n-1的二进制描述
	{
		p++;
		x >> 1;
	}
	
	bool flag1 = true;
	while (flag1)
	{
		y.Random();//随机生成一个y来作为检测数
	   //y应该小于N的整数
		if (y < n)
			flag1 = false;

	}
	y = pow_mod(y, x, n);

	if ((!(y == 1)) && (!(y == (n - 1))))
	{
		q = 1;
		while ((n <= p - 1) && (!(y == (n - 1))))
		{
			y = (y * y) % n;
			if (y == 1)
				return false;
			q++;
		}
		if (!(y == (n - 1)))
			return false;
	}
	return true;
}

//多次测试
bignumber testnumber()
{
	
	bignumber n;
	int j = 0;
	int m;
	cout << "对生成的数进行几次rabinmiller" << endl;
	cin >> m;
	
	while (j < m)//每个大数10次rabinmiller通过，可基本认为是素数了。
	{
		cout << "随机生成大数：" << endl;
		makeprime(n);
		n.show();
		for (; j < m; j++)
		{
			
			if (!RabinMiller(n))
			{
				cout << "在"<<m<<"轮以内测试失败" << endl;
				break;
			}
			cout << "通过第"<<(j+1)<<"轮RabinMiller测试" << endl;
		}

		/*while (RabinMiller(n))
		{
			i++;
		}
		if()
		{
			i = 0;

			SortPrime(n);
		}*/

	}
	return n;
}

//求两个大数的最大公约数,采用辗转相除法
bignumber gcd(const bignumber& m, const bignumber& n)
{
	if (n == 0)
		return m;
	else
		return gcd(n, m % n);
}

//用扩展欧几里德算法求乘法模逆
bignumber Egcd(const bignumber& a, const bignumber& b, bignumber& x, bignumber& y)
{
	bignumber t, d;
	//如果一个操作数为零则无法进行除法运算
	if (b == 0)
	{
		x = 1;
		y = 0;
		return a;
	}
	d = Egcd(b, a % b, x, y);
	t = x;
	x = y;
	y = t - ((a / b) * y);
	return d;
}
