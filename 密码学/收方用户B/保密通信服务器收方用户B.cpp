﻿//用户B
#pragma once
#pragma warning( disable : 4996 )
#include "socket初始化.h"//创建TCP套接字
#include <stdio.h>
#include <iostream>
#include<time.h>//时间戳
#include<string>
#include"md5.h"//MD5算法计算hash

//RSA算法

#include"大数定义类.h"
#include"素数检测.h"

using namespace std;
CInitSock initSock;     // 初始化Winsock库
bool flag = false;
int main()
{


    //密钥生成

    srand((unsigned int)time(NULL));//用时间来更换不同的随机种子
    //如果只在random函数中调用，计算速度会变慢很多，因为每次调用rand重新使用新种子
    //改为在程序开始处调用一次即可
    cout << "生成大素数P" << endl;
    bignumber x;

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

    //用PKA(NA,EA)将PKB(NB||EB)加密,PKB为Nb,eB的拼接
    bignumber PKB, eA, nA;//B已知A公钥
    PKB = e;
    eA = n;
    nA = d;

    // 创建socket
    SOCKET sListen = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sListen == INVALID_SOCKET)
    {
        printf("Failed socket() /n");
        return 0;
    }

    // 填充sockaddr_in结构
    sockaddr_in sin;
    sin.sin_family = AF_INET;
    sin.sin_port = htons(4567);
    sin.sin_addr.S_un.S_addr = INADDR_ANY;

    // bind本地地址
    if (::bind(sListen, (LPSOCKADDR)&sin, sizeof(sin)) == SOCKET_ERROR)
    {
        printf("Failed bind() /n");
        return 0;
    }

    // 监听有无连接
    if (::listen(sListen, 2) == SOCKET_ERROR)
    {
        printf("Failed listen() /n");
        return 0;
    }
    cout << "等待客户端连接" << endl;
    // 接受客户的连接请求
    sockaddr_in remoteAddr;
    int nAddrLen = sizeof(remoteAddr);
    SOCKET sClient = 0;
    char sendmessage[] = "12345/r/n";//向A发送数据
    while (sClient == 0)
    {
        // 接受一个新连接
        sClient = ::accept(sListen, (SOCKADDR*)&remoteAddr, &nAddrLen);
        if (sClient == INVALID_SOCKET)
        {
            printf("Failed accept()");
            time_t t;
            time(&t);
            printf("日期和时间%s", ctime(&t));
        }


        printf("接受到一个连接：%s /r/n", inet_ntoa(remoteAddr.sin_addr));
        time_t t;
        time(&t);
        printf("连接建立的时间%s", ctime(&t));
        continue;
    }





    //进行保密通信
    //while (TRUE)
    {

        //N为256个16进制数组成，1个16进制数对应4个bit位，n共1024bit位
        bignumber eB, nB, dB;//由密钥生成后直接填入

        eB = e;
        nB = n;
        dB = d;

        //发送加密PKA的PKB
        cout << "密文C=m^e mod n" << endl;
        bignumber c1 = pow_mod(PKB, eA, nA);
        cout << "加密后的PKB为：" << endl;
        //16进制形式显示
        c1.show();
        string c1str = c1.savestring();

        cout << endl;

        for (int i = 0; i < c1str.length(); i++)
        {
            sendmessage[i] = c1str[i];//转换完的字符串逐个字符进行赋值操作
        }

        //sendmessage[c1str.length()] = 0;
        // 已经完成输入了 ！！！再次get无法获取内容！
        //gets_s(sendmessage);//新版本下需要将gets改为gets_s
        ::send(sClient, sendmessage, strlen(sendmessage), 0);



        // 从客户端接收数据
        char recvbuff[1024];
        string asss;
        int nRecv = ::recv(sClient, recvbuff, 1024, 0);
        recvbuff[nRecv] = 0;

        asss = recvbuff;//接收不含加零的字符内容

        int line = asss.find('|');//从0开始的下标位置
        int at1 = asss.find('@');
        string s3 = asss.substr(at1 + 1, 256);
        //int len = s.length();
        //substr从0开始，（开始位置，读取个数）
        

        
        if (nRecv > 0)
        {
            if (flag)
            {
                printf("接收到的消息为：%s/n", recvbuff);
            }

            cout << "接收到的消息为：" << endl;
            cout << s3 << endl;

            time_t t;
            time(&t);
            printf("消息接受的时间%s", ctime(&t));
        }
        if(flag)
        //用SKB将A发来的消息解密，得到SKA签名的明文及其hash
        //明文|hash


        //解密
        {cout << "m=c^d mod n" << endl;
        //新解密得到的明文记为m1
        bignumber m1 = pow_mod(c2, dB, nB);
        cout << "解密完成" << endl;

        //用已知的PKA验证签名
        bignumber ml2 = pow_mod(m1, eA, nA);
        cout << "PKA验证签名通过" << endl;
        //明文|hash

        asss = string(m12);//将大数强制转换为字符串形式表示

        }
        cout << "解密和验签通过，下面恢复明文和hash" << endl;
        string s1 = asss.substr(0, line);//明文部分,从开头向后数line个，line+1个对应下标line“|”

        string s2 = asss.substr(line + 1, 32);//hash部分,从|位置的下一个开始向后读到结束，32位
        cout << "解密得到的明文为：" << endl;
        cout << s1 << endl;
        if (flag) {//如果消息中有除了数字之外的字符
            int z1[1000];
            for (int i = 0; i < s1.length(); i++)
            {
                if (s1[i] < '0' || s1[i]>'9')
                {
                    s1[i] = z1[i];//将int对应位置ASCII数字赋值给char,将直接转换为对应的字符
                }
            }
        }
        cout << "解密得到的明文hash为" << endl;
        cout << s2 << endl;
        cout << "对方能够正确获取PKB，且B用SKA验签通过并获取了明文和hash，可认为消息来源真实" << endl;
        //计算明文hash
        string mhash = md5(s1);
        cout << "将明文计算hash" << endl;
        cout << mhash << endl;
        //判断hash是否相等s2
        for (int i = 0; i < 32; i++)
        {
            if (mhash[i] != s2[i])
            {
                cout << "hash值不同！收到的明文已经被篡改！" << endl;
                break;
            }
        }
        cout << "如果hash相等，说明明文没有被篡改,可认为消息满足完整性" << endl;
    }
    // 关闭同客户端的连接
    ::closesocket(sClient);

    // 关闭监听套节字
    ::closesocket(sListen);

    system("pause");
    return 0;
}