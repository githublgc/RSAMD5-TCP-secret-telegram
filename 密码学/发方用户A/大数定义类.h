#pragma once
//#include"2000���ڵ�������.h"
//����������������������һ��ͷ�ļ��У���
//�ں��������ʱ������������ļ������ݵ����⡣
#include <iostream>
#include <stdlib.h> //���������rand()
using namespace std;
class bignumber {

	//����֮��ļӼ��˳�ģ
	friend bignumber operator+ (const bignumber&, const bignumber&);
	friend bignumber operator- (const bignumber&, const bignumber&);
	friend bignumber operator* (const bignumber&, const bignumber&);//n=p*q
	friend bignumber operator% (const bignumber&, const bignumber&);//ģ������,m^e mod n
	friend bignumber operator/ (const bignumber&, const bignumber&);

	//��������ͨ����
	friend bignumber operator- (const bignumber&, const int&);//n��ŷ������p-1,q-1�õ���������С����
	
	
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
	void operator>> (const int&);//����С���������������λ���Ӷ���ϳɴ���

	inline int numlength() const;   //���ش����ĳ���
	
	void zero();  //������0
	void Random(); //�������һ������
	void show() const;
	string savestring();
	bool IsOdd() const { return (data[0] & 1); }  //�жϴ�����ż��

public:
	unsigned int data[64];
	bool flag;//��־λ�����Ƿ���һ���ڶ��巶Χ�ڵĺϷ�����
};

//Ĭ�Ϲ��캯��,��Ա������0
bignumber::bignumber()
{
	for (int i = 0; i < 64; i++)
		data[i] = 0;
	flag = true;
}
bignumber el, nl;

//��int��ʼ������
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

//�����ӷ�
bignumber operator+ (const bignumber& a, const bignumber& b)
{
	bignumber result;//�����Ӻͽ��
	unsigned __int64 sum;
	//upΪ��λ��־
	unsigned int up = 0;
	//ȡa,b�г��Ƚϳ��ĳ���
	int length;
	if (a.numlength() > b.numlength())
		length = a.numlength();
	else
		length = b.numlength();

	//ͬΪ�����ӷ�
	if (a.flag == b.flag)
	{
		//��λ��ӣ������λ
		for (int i = 0; i < length; i++)
		{
			sum = (unsigned __int64)a.data[i] + b.data[i] + up;
			result.data[i] = (unsigned int)sum;
			//sum�ĸ�λΪ��λ
			up = (sum >> 32);
		}

		result.flag = a.flag;
		return result;
	}

}

//�����Ĳ�
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

		//ÿһλ������ʽ��
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
				//��λ��
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

//������ͨ����
bignumber operator- (const bignumber& a, const int& b)
{
	bignumber temp(b);
	bignumber result = a - temp;
	return result;
}


//���������������������������
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

//�������p*q
bignumber operator* (const bignumber& a, const bignumber& b)
{
	//������м�������ǰ������
	bignumber result, temp1, current;
	//��λ�Ļ�
	unsigned __int64 sum;
	//��Ž�λ
	unsigned int up;

	for (int i = 0; i < b.numlength(); i++)
	{
		up = 0;
		//B��ÿһλ��A���
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

	//�жϻ��ķ���
	if (a.flag == b.flag)
		result.flag = true;
	else
		result.flag = false;

	return result;
}

//����/����������
bignumber operator/ (const bignumber& a, const bignumber& b)
{
	//���Կ��ܵ���,��Ǹ�λ��λ
	unsigned int testresult;
	unsigned int down, up;
	//div��������div2����,acc�̳˳�����acc2����һ���Ļ�
	bignumber DIV, DIV2, acc, acc2;
	int lengtha = a.numlength(), lengthb = b.numlength();

	//���������С�ڳ���,ֱ�ӷ���0
	if (a < b)
	{
		if (a.flag == b.flag)
			DIV2.flag = true;
		else
			DIV2.flag = false;
		return DIV2;
	}

	//�ѱ������������ĳ��ȴӸ�λ��λ
	int i;
	for (i = 0; i < lengthb; i++)
		DIV.data[i] = a.data[lengtha - lengthb + i];

	for (i = lengtha - lengthb; i >= 0; i--)
	{
		//���������С�ڳ���,������λ
		if (DIV < b)
		{
			for (int j = lengthb; j > 0; j--)
				DIV.data[j] = DIV.data[j - 1];
			DIV.data[0] = a.data[i - 1];
			continue;
		}

		down = 0;
		up = 0xffffffff;

		//���ֲ��ҷ���������
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

		//���̽�����浽����ȥ
		DIV2.data[i] = testresult;
		//��ʱ��������Ϊ�����������̻��Ĳ�
		DIV = DIV - acc;

		//��ʱ����������λ
		if ((i - 1) >= 0)
		{
			for (int j = lengthb; j > 0; j--)
				DIV.data[j] = DIV.data[j - 1];
			DIV.data[0] = a.data[i - 1];
		}
	}

	//�ж��̵ķ���
	if (a.flag == b.flag)
		DIV2.flag = true;
	else
		DIV2.flag = false;
	return DIV2;
}

//������ģ,��ȳ���
bignumber operator% (const bignumber& a, const bignumber& b)
{
	unsigned int testresult, down, up;
	bignumber DIV, DIV2, ACC, ACC2;
	int lengtha = a.numlength(), lengthb = b.numlength();

	//���������С�ڳ���,���ر�����Ϊģ
	if (a < b)
	{
		DIV = a;
		DIV.flag = a.flag;
		return DIV;
	}

	//���г�������
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

//������ֵ
void bignumber::operator= (const bignumber& input)
{
	for (int i = 0; i < 64; i++)
		data[i] = input.data[i];
	flag = input.flag;
}

//����С��
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

//���ش���
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

//�������
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



//��������16������ʾ����Ļ��
void bignumber::show() const
{
	unsigned int temp, result;
	unsigned int temp2 = 0xf0000000;

	for (int i = numlength() - 1; i >= 0; i--)
	{
		temp = data[i];
		//����ת����16�������
		for (int j = 0; j < 8; j++)
		{
			result = temp & temp2;
			result = (result >> 28);
			temp = (temp << 4);//4�������ƶ�Ӧһ��16������
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
		cout << " ";//ÿ8��16��������Ϊһ��
	}
	cout << endl;
}

//������0
void bignumber::zero()
{
	for (int i = 0; i < 64; i++)
		data[i] = 0;
}

//���ش�������
inline int bignumber::numlength() const
{
	int length = 64;
	for (int i = 63; i >= 0; i--)
	{
		//�ӵ�һ����Ϊ������λ��ʼ����
		if (data[i] == 0)
			length--;
		else
			break;
	}
	return length;
}

//����λ
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

//��������ͨ�����Ĵ�С
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

//����n��p����ģm
bignumber pow_mod(const bignumber& n, const bignumber& p, const bignumber& m)
{
	bignumber temp = p;
	bignumber NmodM = n % m;
	bignumber result(1);

	//���ָ��p�Ķ�������ʽ��ÿһλ
	while (!(temp <= 1))
	{
		//�����λΪ1�����ʾ��λ��Ҫ����ģ����
		if (temp.IsOdd())
		{
			result = (result * NmodM) % m;
		}
		NmodM = (NmodM * NmodM) % m;
		temp >> 1;
	}
	return (NmodM * result) % m;
}

string bignumber::savestring() //�Ѵ���16����չʾ����
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
		//������ÿһλ����ת����16�������
		for (int j = 0; j < 8; j++)
		{
			result = temp & temp2;
			result = (result >> 28);
			temp = (temp << 4);
			if (result >= 0 && result <= 9)
			{ //cout << result;
				char bigdatanumber2[100] = "";
				_itoa_s(result, bigdatanumber2, 30, 10);//��ÿһλresult�ֱ�ת��Ϊ��ʱ�ַ����Ȼ��ƴ��
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


