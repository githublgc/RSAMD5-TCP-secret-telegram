//#include"2000以内的素数表.h"
#include"大数定义类.h"
#include"素数检测.h"


using namespace std;

int main()
{
	srand((unsigned int)time(NULL));//用时间来更换不同的随机种子
	//如果只在random函数中调用，计算速度会变慢很多，因为每次调用rand重新使用新种子
	//改为在程序开始处调用一次即可
	cout << "生成大素数P" << endl;

	//产生大素数
	bignumber p = testnumber();
	p.show();
	cout << endl;

	cout << "生成大素数Q" << endl;

	//产生大素数
	bignumber q = testnumber();

	//16进制形式显示
	q.show();
	cout << endl;

	cout << "n = p * q" << endl;
	bignumber n = p * q;

	cout << "n= " << endl;
	//16进制形式显示
	n.show();

	cout << endl;
	cout << "任意生成互素的e和乘法逆元d " << endl;

	//n的欧拉函数n1
	bignumber n1 = (p - 1) * (q - 1);

	//e为公开钥
	bignumber e;

	//d为秘密钥，即e模n1的乘法逆元
	bignumber d;

	//存储n1模e的乘法逆元
	bignumber temp1;
	bignumber temp2;

	while (1)
	{
		//随机生成互素的e
		e.Random();
		while (!(gcd(e, n1) == 1))
		{
			e.Random();//不满足就重新生成
		}

		//用扩展欧几里德算法求乘法逆元
		temp2 = Egcd(e, n1, d, temp1);

		//e*d模t结果为1时，d为e的乘法逆元
		temp2 = (e * d) % n1;
		if (temp2 == 1)
			break;

		//否则重新生成e
	}


	cout << "公钥e= " << endl;
	//16进制形式显示
	e.show();
	
	cout << endl;

	cout << "私钥d= " << endl;
	//16进制形式显示
	d.show();
	cout << endl;

	system("pause");
	return 0;
}

