//用户A
#pragma warning( disable : 4996 )//解决ctime，inet_ntoa的报错
#pragma once
#include "socket初始化.h"//创建TCP套接字 
#include <stdio.h>
#include <iostream>
#include<time.h>//时间戳
#include"md5.h"//md5算法，用于计算hash
//RSA算法

#include"大数定义类.h"
#include"素数检测.h"

using namespace std;
CInitSock initSock;     // 初始化Winsock库
bool flag = false;
//每次每边只能发送一条消息，发送后需要等待对面发送的消息
int main()
{

    //密钥生成

    srand((unsigned int)time(NULL));//用时间来更换不同的随机种子
    //如果只在random函数中调用，计算速度会变慢很多，因为每次调用rand重新使用新种子
    //改为在程序开始处调用一次即可
    cout << "生成大素数P" << endl;
    bignumber x, y, z;//PKB,明文等转换为大数形式进行运算

    //产生大素数
    bignumber p = testnumber();
    p.show();
    cout << endl;
    for (int i = 63; i >= 0; i--)//从低位向高位输出
        cout << p.data[i];
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



    // 创建套节字
    SOCKET s = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (s == INVALID_SOCKET)
    {
        printf(" Failed socket() /n");
        return 0;
    }


    // 填写远程地址信息
    sockaddr_in servAddr;
    servAddr.sin_family = AF_INET;
    servAddr.sin_port = htons(4567);

    servAddr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");//设置本地主机ip

    if (::connect(s, (sockaddr*)&servAddr, sizeof(servAddr)) == -1)
    {
        printf(" Failed connect() /n");
        time_t t;
        time(&t);
        printf("建立连接的时间%s", ctime(&t));//显示对应的年月日和时间秒
        return 0;
    }



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
    bignumber eBl=e, nBl=n;

    //进行保密通信

    char recvbuff[1024];//接收数据的缓冲区

    char sendtext[2048];//发送数据缓冲区

    //while (TRUE)
    {
        //从用户B端接收被PKA加密的B的公钥PKB
        int nRecv = ::recv(s, recvbuff, 1024, 0);
        if (nRecv > 0)
        {
            recvbuff[nRecv] = 0;//修改缓冲区


            printf("接收到PKA加密的PKB：%s/n", recvbuff);
            time_t t;
            time(&t);
            printf("消息发送的日期和时间 %s", ctime(&t));
        }

        if (flag) {


            //用skA（d,n）解密得到PKB(eB|nB)
            //解密
            //cout << "m=c^d mod n" << endl;
            //新解密得到的明文记为m1
            string x1 = recvbuff;//将接收的消息转换为字符串
            x = bignumber(xl);//把字符串转换为大数
            bignumber m1 = pow_mod(xl, d, n);
            cout << "解密完成，获取PKB=(eB|nB)" << endl;
            string PKB = string(ml);//把大数转换为string,拆分得到eB,nB
            int line = PKB.find('|');//从0开始的下标位置

            int len = PKB.length();
            //substr从0开始，（开始位置，读取个数）
            string e1 = PKB.substr(0, line);//eB,从开头向后数line个，line+1个对应下标line“|”

            string n1 = PKB.substr(line + 1, len - 1);//nB,从|位置的下一个开始向后读到结束，32位

            bignumber eB1 = bignumber(el);
            bignumber nB1 = bignumber(nl);//将截取的e1,n1转换为大数形式参与运算
        }

        //对明文进行hash
        cout << "请输入需要加密传输的明文" << endl;
        string m;//输入明文
        cin >> m;
        cout << "明文是：" << m << endl;
        string hashm = md5(m);
        cout << "明文hash为" << hashm << endl;

        //将明文和明文hash拼接，中间用|隔开
        string oneline = "|";
        string ass = m + oneline + hashm;
        cout << "明文和hash拼接后为" << ass << endl;
        if (flag) {//如果输入内容有除了数字外的字符，需转化
        int z1[1000];
        for (int i = 0; i < m.length(); i++)
        {
            if (m[i] < '0' || m[i]>'9')//如果不是数字
            {
                z1[i] = m[i];//将字符常量赋值给整型常量，字符可以转换为对应ascii码的形式
            }
        }}
        //用skA(d，n)签名
        // ass转换为大数y
        //cout << "m=c^d mod n" << endl;
        bignumber mx99 = pow_mod(y, d, n);
        //用获取的PKB对签名内容加密
        //加密
        cout << "用PKB加密明文所得密文C=m^e mod n" << endl;
        z = 123;
        bignumber c = pow_mod(z, eBl, nBl);
        //16进制形式显示
        cout << "实际传输的加密内容为：" << endl;
        c.show();
        string cstr = c.savestring();//长度为256
        
        string oneat = "@";
        string asss = ass + oneat + cstr;
        // 向B发送加密内容

        //cin >> szText;
        for (int i = 0; i < asss.length(); i++)
            sendtext[i] = asss[i];
        sendtext[asss.length()] = 0;
        ::send(s, sendtext, strlen(sendtext), 0);

    }

    // 关闭套节字
    ::closesocket(s);

    system("pause");
    return 0;
}