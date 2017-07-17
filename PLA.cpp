/*************************************************************************
	> File Name: PLA.cpp
	> Author: 
	> Mail: 
	> Created Time: Mon 17 Jul 2017 11:14:44 GMT
 ************************************************************************/

#include<iostream>
using namespace std;
int main()
{
    bool flag = true;
    int x1[10],x2[10],num;
    int type[10];
    int w1 = 0,w2 = 0,b = 0,times = 0;
    cin>>num;
    for(int  i = 0; i < num; i++)
    {
        cin >> x1[i] >> x2[i]>>type[i];
    }
    do
    {
        times++;
        int t = 0;
        for(int i  = 0; i  < num; i++)
        {
            if(type[i]*(w1*x1[i]+w2*x2[i]+b) <= 0)//判断是否被正确分类
            {
                flag = false;//如果没有正确分类,修改标志
                
                w1 += type[i]*x1[i];//更新w
                w2 += type[i]*x2[i];
                b  += type[i];
            }
            else t++;
        }
        if(t == num)
        {
            flag = true;
        }

    }
    while(!flag);
    cout <<"w1:"<< w1 <<" w2:"<<w2<<" b:"<<b<<"times"<<times<<endl;
    return 0;
}
