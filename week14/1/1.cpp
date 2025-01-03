#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include<sstream>
#include<algorithm>
#include<map>
#include<queue>
using namespace std;

void stringSplit(string strs,const char split,vector<string>& str)
{
    str.clear();
    istringstream iss(strs);
    string temp;
    while(getline(iss,temp,split))
    {
        if(temp!="")
            str.push_back(temp);
        // cout<<temp<<endl;
    }
}
const int MAXINT=0x3fffffff;
const int MAXN=1145;
int head[MAXN],nxt[MAXN],to[MAXN],we[MAXN],cnt;
void ins(int s,int t,int w)
{
    cnt++;
    nxt[cnt]=head[s];
    head[s]=cnt;
    to[cnt]=t;
    we[cnt]=w;
}
string line1,line2;
vector<string>str1,str2,temp;
map<string,int>m;
string name1,name2;
int w;
const int sour=1,dest=2,aux=3;
string name[MAXN];
int nameCnt=3;
std::queue<int>q;
int dis[MAXN];
std::vector<string> way[MAXN];
int main()
{
    getline(cin,line1);
    getline(cin,line2);
    stringSplit(line1,' ',str1);
    stringSplit(line2,' ',str2);
    m[str1[0]]=sour;
    m[str1[1]]=dest;
    name[sour]=str1[0];
    name[dest]=str1[1];
    ins(dest,aux,0);
    for(int i=0;i<str2.size();i++)
    {
        stringSplit(str2[i],':',temp);
        name1=temp[0];
        name2=temp[1];
        w=stoi(temp[2]);
        if(m.count(name1)==0)
        {
            m[name1]=++nameCnt;
            name[nameCnt]=name1;
        }
        if(m.count(name2)==0)
        {
            m[name2]=++nameCnt;
            name[nameCnt]=name2;
        }
        ins(m[name1],m[name2],w);
    }
    for(int i=1;i<=nameCnt;i++)
        dis[i]=MAXINT;
    dis[sour]=0;
    q.push(sour);
    while(!q.empty())
    {
        int now=q.front();
        q.pop();
        for(int e=head[now];e;e=nxt[e])
        {
            int son=to[e];
            if(dis[son]>dis[now]+we[e])
            {
                dis[son]=dis[now]+we[e];
                q.push(son);
                way[son]=way[now];
                way[son].push_back(name[now]);
                continue;
            }
            if(dis[son]==dis[now]+we[e])
            {
                bool flag=false;
                for(int i=0;i<way[now].size();i++)
                {
                    if(way[now][i]>name[now])
                    {
                        flag=true;
                        break;
                    }
                }
                if(flag)
                {
                    q.push(son);
                    way[son]=way[now];
                    way[son].push_back(name[now]);
                }
            }
        }
    }
    cout<<dis[dest]<<' ';
    for(int i=0;i<way[aux].size()-1;i++)
        cout<<way[aux][i]<<"->";
    cout<<way[aux][way[aux].size()-1]<<endl;
}