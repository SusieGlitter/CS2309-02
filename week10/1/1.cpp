#include<iostream>
#include<sstream>
#include<string>
#include<cmath>
#include<vector>
#include<algorithm>
using namespace std;
string s1,s2;
string treeSort(string now)
{
    int p=1,stk=0,s=1;
    string ret="1";
    vector<string> son;
    for(int i=1;i<now.size()-1;i++)
    {
        if(now[i]=='1')stk++;
        else stk--;
        if(stk==0)
        {
            son.push_back(treeSort(now.substr(s,i-s+1)));
            s=i+1;
        }
    }
    sort(son.begin(),son.end());
    for(auto &i:son)ret+=i;
    ret+="0";
    return ret;
}
int main()
{
    cin>>s1>>s2;
    s1=treeSort(s1);
    s2=treeSort(s2);
    // cout<<s1<<endl<<s2<<endl;
    if(s1==s2) cout<<"isomorphic";
    else cout<<"non-isomorphic";
}