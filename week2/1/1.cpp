#include<iostream>
#include<cstring>
#include<sstream>
#include<fstream>
#include<string>
#include<vector>
#include<algorithm>
using namespace std;

void stringSplit(string strs,const char split,vector<string>& str)
{
    istringstream iss(strs);
    string temp;
    while(getline(iss,temp,split))
    {
        str.push_back(temp);
        // cout<<temp<<endl;
    }
}
string filenames;
vector<string> filename;
string copycodes;
vector<string> copycode;
string copyname;
string subname1;
string subname2;
string subname3;

int id=1;
int times;

int main()
{
    getline(cin,filenames);
    stringSplit(filenames,' ',filename);
    getline(cin,copycodes);
    stringSplit(copycodes,' ',copycode);

    if(copycode.size()==2)
        times=1;
    else
        times=stoi(copycode[2]);

    bool exist=false;
    for(int j=0;j<filename.size();j++)
        if(copycode[1]==filename[j])
        {
            exist=true;
            break;
        }
    if(!exist||copycode[0]!="copy"||times<=0)
    {
        cout<<"error";
        return 0;
    }
    
    // cout<<times;

    for(int i=copycode[1].length()-1;i>=0;i--)
    {
        if(copycode[1][i]=='.')
        {
            subname1=copycode[1].substr(0,i)+"_";
            subname3=copycode[1].substr(i,copycode[1].length()-1);
            break;
        }
        if(i==0)
        {
            subname1=copycode[1]+"_";
        }
    }
    
    for(int i=0;i<times;i++)
    {
        while(true)
        {
            copyname=subname1+to_string(id)+subname3;
            bool unique=true;
            for(int j=0;j<filename.size();j++)
                if(copyname==filename[j])
                {
                    unique=false;
                    break;
                }
            if(unique)
                break;
            id++;
        }
        cout<<copyname<<' ';
        id++;
    }

    return 0;
}