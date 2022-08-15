#pragma once
//#include"2000以内的素数表.h"
//这里如果单独将素数表放在一个头文件中，、
//在后面在输出时会出现声明和文件不兼容的问题。
#include <iostream>
#include <stdlib.h> //生成随机数rand()
using namespace std;
class bignumber {

	//大数之间的加减乘除模
	friend bignumber operator+ (const bignumber&, const bignumber&);
	friend bignumber operator- (const bignumber&, const bignumber&);
	friend bignumber operator* (const bignumber&, const bignumber&);//n=p*q
	friend bignumber operator% (const bignumber&, const bignumber&);//模幂运算,m^e mod n
	friend bignumber operator/ (const bignumber&, const bignumber&);

	//大数和普通整数
	friend bignumber operator- (const bignumber&, const int&);//n的欧拉函数p-1,q-1用到大整数和小整数
	
	
	friend bool operator< (const bignumber&, const bignumber&);
	friend bool operator> (const bignumber&, const bignumber&);

	friend bool operator<= (const bignumber&, const int&);
	friend bool operator== (const bignumber&, const bignumber&);
	friend bignumber pow_mod(const bignumber&, const bignumber&, const bignumber&);
	friend void makeprime(bignumber& n);

public:
	bignumber();
	bignumber(const int&);
	

	void operator= (const bignumber&);
	void operator= (const int& a) { zero(); data[0] = a; }
	void operator>> (const int&);//将较小的随机数进行左移位，从而组合成大数

	inline int numlength() const;   //返回大数的长度
	
	void zero();  //大数清0
	void Random(); //随机产生一个大数
	void show() const;
	string savestring();
	bool IsOdd() const { return (data[0] & 1); }  //判断大数奇偶性

public:
	unsigned int data[64];
	bool flag;//标志位，看是否是一个在定义范围内的合法大数
};

//默认构造函数,成员数据清0
bignumber::bignumber()
{
	for (int i = 0; i < 64; i++)
		data[i] = 0;
	flag = true;
}
bignumber el, nl;

//用int初始化大数
bignumber::bignumber(const int& input)
{
	for (int i = 0; i < 64; i++)
		data[i] = 0;
	data[0] = input;
	if (input >= 0)
		flag = true;
	else
		flag = false;
}

//大数加法
bignumber operator+ (const bignumber& a, const bignumber& b)
{
	bignumber result;//大数加和结果
	unsigned __int64 sum;
	//up为进位标志
	unsigned int up = 0;
	//取a,b中长度较长的长度
	int length;
	if (a.numlength() > b.numlength())
		length = a.numlength();
	else
		length = b.numlength();

	//同为正，加法
	if (a.flag == b.flag)
	{
		//对位相加，满后进位
		for (int i = 0; i < length; i++)
		{
			sum = (unsigned __int64)a.data[i] + b.data[i] + up;
			result.data[i] = (unsigned int)sum;
			//sum的高位为进位
			up = (sum >> 32);
		}

		result.flag = a.flag;
		return result;
	}

}

//大数的差
bignumber operator- (const bignumber& a, const bignumber& b)
{
	bignumber result;
	unsigned __int64 sum;
	unsigned int up = 0;
	int sub;
	if (a.flag == b.flag)
	{
		bignumber bigger, little;
		if (a < b)
		{
			bigger = b;
			little = a;
			bigger.flag = !bigger.flag;
		}
		else
		{
			bigger = a;
			little = b;
		}

		//每一位进行竖式减
		for (int i = 0; i < 64; i++)
		{
			sub = little.data[i] + up;
			if (bigger.data[i] >= sub)
			{
				result.data[i] = bigger.data[i] - sub;
				up = 0;
			}
			else
			{
				//借位减
				result.data[i] = (unsigned __int64)bigger.data[i] + (1 << 32) - sub;
				up = 1;
			}
		}
		result.flag = bigger.flag;
		return result;
	}

	else
	{
		for (int i = 0; i < 64; i++)
		{
			sum = (unsigned __int64)a.data[i] + b.data[i] + up;
			result.data[i] = (unsigned int)sum;
			up = (sum >> 32);
		}
		result.flag = a.flag;
		return result;
	}
}

//大数普通整数
bignumber operator- (const bignumber& a, const int& b)
{
	bignumber temp(b);
	bignumber result = a - temp;
	return result;
}


//大数乘整数，用于生成随机大数
bignumber operator* (const bignumber& a, const unsigned int& b)
{
	bignumber result;
	unsigned __int64 sum;
	unsigned int up = 0;
	for (int i = 0; i < 64; i++)
	{
		sum = ((unsigned __int64)a.data[i]) * b + up;
		result.data[i] = (unsigned int)sum;
		up = (sum >> 32);
	}
	result.flag = a.flag;
	return result;
}

//大数相乘p*q
bignumber operator* (const bignumber& a, const bignumber& b)
{
	//结果，中间结果，当前计算结果
	bignumber result, temp1, current;
	//进位的积
	unsigned __int64 sum;
	//存放进位
	unsigned int up;

	for (int i = 0; i < b.numlength(); i++)
	{
		up = 0;
		//B的每一位与A相乘
		for (int j = 0; j < a.numlength() + 1; j++)
		{
			sum = ((unsigned __int64)a.data[j]) * (b.data[i]) + up;
			if ((i + j) < 64)
				current.data[i + j] = (unsigned int)sum;
			up = (sum >> 32);
		}
		result = (current + temp1);
		temp1 = result;
		current.zero();
	}

	//判断积的符号
	if (a.flag == b.flag)
		result.flag = true;
	else
		result.flag = false;

	return result;
}

//大数/大数，试商
bignumber operator/ (const bignumber& a, const bignumber& b)
{
	//尝试可能的商,标记高位低位
	unsigned int testresult;
	unsigned int down, up;
	//div被除数，div2除数,acc商乘除数，acc2和下一个的积
	bignumber DIV, DIV2, acc, acc2;
	int lengtha = a.numlength(), lengthb = b.numlength();

	//如果被除数小于除数,直接返回0
	if (a < b)
	{
		if (a.flag == b.flag)
			DIV2.flag = true;
		else
			DIV2.flag = false;
		return DIV2;
	}

	//把被除数按除数的长度从高位截位
	int i;
	for (i = 0; i < lengthb; i++)
		DIV.data[i] = a.data[lengtha - lengthb + i];

	for (i = lengtha - lengthb; i >= 0; i--)
	{
		//如果被除数小于除数,再往后补位
		if (DIV < b)
		{
			for (int j = lengthb; j > 0; j--)
				DIV.data[j] = DIV.data[j - 1];
			DIV.data[0] = a.data[i - 1];
			continue;
		}

		down = 0;
		up = 0xffffffff;

		//二分查找法查找试商
		while (down < up)
		{
			testresult = (((unsigned __int64)up) + down) / 2;
			acc = (b * testresult);
			acc2 = (b * (testresult + 1));

			if (((acc < DIV) && (acc2 > DIV)) || (acc == DIV))
				break;
			if (acc2 == DIV)
			{
				testresult++;
				acc = acc2;
				break;
			}
			if ((acc < DIV) && (acc2 < DIV))
			{
				down = testresult;
				continue;
			}
			if ((acc > DIV) && (acc2 > DIV))
			{
				up = testresult;
				continue;
			}

		}

		//试商结果保存到商中去
		DIV2.data[i] = testresult;
		//临时被除数变为被除数与试商积的差
		DIV = DIV - acc;

		//临时被除数往后补位
		if ((i - 1) >= 0)
		{
			for (int j = lengthb; j > 0; j--)
				DIV.data[j] = DIV.data[j - 1];
			DIV.data[0] = a.data[i - 1];
		}
	}

	//判断商的符号
	if (a.flag == b.flag)
		DIV2.flag = true;
	else
		DIV2.flag = false;
	return DIV2;
}

//大数求模,类比除法
bignumber operator% (const bignumber& a, const bignumber& b)
{
	unsigned int testresult, down, up;
	bignumber DIV, DIV2, ACC, ACC2;
	int lengtha = a.numlength(), lengthb = b.numlength();

	//如果被除数小于除数,返回被除数为模
	if (a < b)
	{
		DIV = a;
		DIV.flag = a.flag;
		return DIV;
	}

	//进行除法运算
	int i;
	for (i = 0; i < lengthb; i++)
		DIV.data[i] = a.data[lengtha - lengthb + i];

	for (i = lengtha - lengthb; i >= 0; i--)
	{
		if (DIV < b)
		{
			for (int j = lengthb; j > 0; j--)
				DIV.data[j] = DIV.data[j - 1];
			DIV.data[0] = a.data[i - 1];
			continue;
		}

		down = 0;
		up = 0xffffffff;

		while (down <= up)
		{
			testresult = (((unsigned __int64)up) + down) / 2;
			ACC = (b * testresult);
			ACC2 = (b * (testresult + 1));

			if (((ACC < DIV) && (ACC2 > DIV)) || (ACC == DIV))
				break;
			if (ACC2 == DIV)
			{
				testresult++;
				ACC = ACC2;
				break;
			}
			if ((ACC < DIV) && (ACC2 < DIV))
			{
				down = testresult;
				continue;
			}
			if ((ACC > DIV) && (ACC2 > DIV))
			{
				up = testresult;
				continue;
			}
		}

		DIV2.data[i] = testresult;
		DIV = DIV - ACC;
		if ((i - 1) >= 0)
		{
			for (int j = lengthb; j > 0; j--)
				DIV.data[j] = DIV.data[j - 1];
			DIV.data[0] = a.data[i - 1];
		}
	}

	DIV.flag = a.flag;
	return DIV;
}

//大数赋值
void bignumber::operator= (const bignumber& input)
{
	for (int i = 0; i < 64; i++)
		data[i] = input.data[i];
	flag = input.flag;
}

//重载小于
bool operator< (const bignumber& a, const bignumber& b)
{
	for (int i = 63; i > 0; i--)
	{
		if (a.data[i] < b.data[i])
			return true;
		if (a.data[i] > b.data[i])
			return false;
	}
	return a.data[0] < b.data[0];
}

//重载大于
bool operator> (const bignumber& a, const bignumber& b)
{
	for (int i = 63; i >= 0; i--)
	{
		if (a.data[i] > b.data[i])
			return true;
		if (a.data[i] < b.data[i])
			return false;
	}
	return false;
}

//大数相等
bool operator== (const bignumber& a, const bignumber& b)
{
	for (int i = 0; i < 64; i++)
		if (a.data[i] != b.data[i])
			return false;
	return true;
}


void hex_to_bin(char* hex, int* bin) {
	for (int i = 0, k = 0; i < 16; i++) {
		int val = 0;
		if (hex[i] >= '0' && hex[i] <= '9') {
			val = hex[i] - '0';
		}
		else if (hex[i] >= 'A' && hex[i] <= 'F') {
			val = hex[i] - 'A' + 10;
		}
		else if (hex[i] >= 'a' && hex[i] <= 'f') {
			val = hex[i] - 'a' + 10;
		}
		for (k = 4 * i + 3; k >= 4 * i; k--) {
			bin[k] = val % 2;
			val /= 2;
		}
	}
}

void bin_to_hex(int* bin, char* hex) {
	for (int i = 0, k = 0; i < 16; i++) {
		int val = 0;
		for (k = 4 * i; k <= 4 * i + 3; k++) {
			val *= 2;
			val += bin[k];
		}
		if (val < 10) {
			hex[i] = val + '0';
		}
		else if (val >= 10 && val <= 15) {
			hex[i] = val - 10 + 'A';
		}
	}
	hex[16] = '\0';
}



//将大数以16进制显示到屏幕上
void bignumber::show() const
{
	unsigned int temp, result;
	unsigned int temp2 = 0xf0000000;

	for (int i = numlength() - 1; i >= 0; i--)
	{
		temp = data[i];
		//大数转换成16进制输出
		for (int j = 0; j < 8; j++)
		{
			result = temp & temp2;
			result = (result >> 28);
			temp = (temp << 4);//4个二进制对应一个16进制数
			if (result >= 0 && result <= 9)
				cout << result;
			else
			{
				switch (result)
				{
				case 10:
					cout << "A";
					break;
				case 11:
					cout << "B";
					break;
				case 12:
					cout << "C";
					break;
				case 13:
					cout << "D";
					break;
				case 14:
					cout << "E";
					break;
				case 15:
					cout << "F";
					break;
				}
			}
		}
		cout << " ";//每8个16进制数分为一组
	}
	cout << endl;
}

//大数置0
void bignumber::zero()
{
	for (int i = 0; i < 64; i++)
		data[i] = 0;
}

//返回大数长度
inline int bignumber::numlength() const
{
	int length = 64;
	for (int i = 63; i >= 0; i--)
	{
		//从第一个不为零的最高位开始计算
		if (data[i] == 0)
			length--;
		else
			break;
	}
	return length;
}

//右移位
void bignumber::operator>> (const int& a)
{
	unsigned int bit;
	data[0] = (data[0] >> a);
	for (int i = 1; i < numlength(); i++)
	{
		bit = data[i] & 1;
		bit = bit << (32 - a);;
		data[i - 1] = data[i - 1] | bit;
		data[i] = (data[i] >> a);
	}
}

//大数和普通整数的大小
bool operator<= (const bignumber& a, const int& b)
{
	for (int i = 1; i < a.numlength(); i++)
	{
		if (a.data[i] != 0)
			return false;
	}
	if (a.data[0] <= b)
		return true;
	else
		return false;
}

bignumber xl;

//计算n的p次幂模m
bignumber pow_mod(const bignumber& n, const bignumber& p, const bignumber& m)
{
	bignumber temp = p;
	bignumber NmodM = n % m;
	bignumber result(1);

	//检测指数p的二进制形式的每一位
	while (!(temp <= 1))
	{
		//如果该位为1，则表示该位需要参与模运算
		if (temp.IsOdd())
		{
			result = (result * NmodM) % m;
		}
		NmodM = (NmodM * NmodM) % m;
		temp >> 1;
	}
	return (NmodM * result) % m;
}

string bignumber::savestring() //把大数16进制展示出来
{
	unsigned int temp, result = 0;
	unsigned int temp2 = 0xf0000000;
	string bigdatanumber1 = "";
	/*char empty[100] = ""; */
	string temp3;
	//char bigdatanumber2[100] = "";

	for (int i = numlength() - 1; i >= 0; i--)
	{
		temp = data[i];
		//大数的每一位数字转换成16进制输出
		for (int j = 0; j < 8; j++)
		{
			result = temp & temp2;
			result = (result >> 28);
			temp = (temp << 4);
			if (result >= 0 && result <= 9)
			{ //cout << result;
				char bigdatanumber2[100] = "";
				_itoa_s(result, bigdatanumber2, 30, 10);//将每一位result分别转换为临时字符串里，然后拼接
				bigdatanumber1 = bigdatanumber1 + bigdatanumber2;
				/*bigdatanumber2 = empty;*/
			}
			else
			{
				switch (result)
				{
				case 10:
					//cout << "A";
					temp3 = "A";
					bigdatanumber1 = bigdatanumber1 + temp3;
					temp3 = "";
					break;
				case 11:
					//cout << "B";
					temp3 = "B";
					bigdatanumber1 = bigdatanumber1 + temp3;
					temp3 = "";
					break;
				case 12:
					//cout << "C";
					temp3 = "C";
					bigdatanumber1 = bigdatanumber1 + temp3;
					temp3 = "";
					break;
				case 13:
					//cout << "D";
					temp3 = "D";
					bigdatanumber1 = bigdatanumber1 + temp3;
					temp3 = "";
					break;
				case 14:
					//cout << "E";
					temp3 = "E";
					bigdatanumber1 = bigdatanumber1 + temp3;
					temp3 = "";
					break;
				case 15:
					//cout << "F";
					temp3 = "F";
					bigdatanumber1 = bigdatanumber1 + temp3;
					temp3 = "";
					break;
				}
			}
		}
	}
	return bigdatanumber1;
	cout << endl;
}


