#include<iostream>
#include<sstream>
#include<string>
#include<cstring>
#include<cmath>
#include<vector>
#include<algorithm>
#include<map>
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
int n;
string s,s1,s2;
vector<string>ss;
map<string,int>m;
int mapcnt=0;
int head[1145],nxt[1145],eto[1145],ecnt=0;
void ins(int s,int t)
{
    nxt[++ecnt]=head[s];
    head[s]=ecnt;
    eto[ecnt]=t;
}
// 一开始看成有向图了，其实应该用并查集的
int color[1145],dfn[1145],low[1145],cnt=0,stk[1145],instk[1145],stktop=0;
int ans=0;
void tarjan(int now)
{
    dfn[now]=low[now]=++cnt;
    stk[++stktop]=now;
    instk[now]=1;
    for(int e=head[now];e;e=nxt[e])
    {
        if(!dfn[eto[e]])
        {
            tarjan(eto[e]);
            low[now]=min(low[now],low[eto[e]]);
        }
        else if(instk[eto[e]])
            low[now]=min(low[now],dfn[eto[e]]);
    }
    if(dfn[now]==low[now])
    {
        ++ans;
        while(stk[stktop]!=now)
        {
            instk[stk[stktop]]=0;
            color[stk[stktop]]=ans;
            --stktop;
        }
        instk[now]=0;
        color[now]=ans;
        --stktop;
    }
}

int main()
{
    getline(cin,s);
    stringSplit(s,' ',ss);
    for(auto &s:ss)
    {
        if(s.find('-')==string::npos)
        {
            if(m.count(s)==0)m[s]=++mapcnt;
            continue;
        }
        s1=s.substr(0,s.find('-'));
        s2=s.substr(s.find('>')+1);
        if(m.count(s1)==0)m[s1]=++mapcnt;
        if(m.count(s2)==0)m[s2]=++mapcnt;
        ins(m[s1],m[s2]);
        ins(m[s2],m[s1]);
    }
    for(int i=1;i<=mapcnt;i++)
        if(!dfn[i])
            tarjan(i);
    cout<<ans<<" graph"<<((ans>1)?"s":"")<<endl;
}