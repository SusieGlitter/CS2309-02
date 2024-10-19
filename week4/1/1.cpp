#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include<sstream>
#include<algorithm>


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

int month2number(string month)
{
    if(month=="Jan")return 1;
    if(month=="Feb")return 2;
    if(month=="Mar")return 3;
    if(month=="Apr")return 4;
    if(month=="May")return 5;
    if(month=="Jun")return 6;
    if(month=="Jul")return 7;
    if(month=="Aug")return 8;
    if(month=="Sep")return 9;
    if(month=="Oct")return 10;
    if(month=="Nov")return 11;
    if(month=="Dec")return 12;
    return -1;
}

class Time
{
public:
    int month;
    int day;
    int hour;
    int minute;
    int second;
    void setTime(string t)
    {
        vector<string> temp;
        stringSplit(t,' ',temp);
        month=month2number(temp[0]);
        day=stoi(temp[1]);
        hour=stoi(temp[2].substr(0,2));
        minute=stoi(temp[2].substr(3,2));
        second=stoi(temp[2].substr(6,2));
        err=false;
        if(month==-1)err=true;
        if(day>31||day<0)err=true;
        if(((month==4)||(month==6)||(month==9)||(month==11))&&day>30)err=true;
        if(month==2&&day>28)err=true;
        if(hour>=24||hour<0)err=true;
        if(minute>=60||minute<0)err=true;
        if(second>=60||second<0)err=true;
    }
    bool err;
    bool operator<(const Time& that)
    {
        if(month<that.month)return true;
        if(month>that.month)return false;
        if(day<that.day)return true;
        if(day>that.day)return false;
        if(hour<that.hour)return true;
        if(hour>that.hour)return false;
        if(minute<that.minute)return true;
        if(minute>that.minute)return false;
        return second<that.second;
    }
    bool operator<=(const Time& that)
    {
        if(month<that.month)return true;
        if(month>that.month)return false;
        if(day<that.day)return true;
        if(day>that.day)return false;
        if(hour<that.hour)return true;
        if(hour>that.hour)return false;
        if(minute<that.minute)return true;
        if(minute>that.minute)return false;
        return second<=that.second;
    }
    bool operator>(const Time& that)
    {
        return !(*this<=that);
    }
    bool operator>=(const Time& that)
    {
        return !(*this<that);
    }
    bool operator==(const Time& that)
    {
        return month==that.month&&day==that.day&&hour==that.hour&&minute==that.minute&&second==that.second;
    }
};


class Log
{
public:
    string data;
    Time times;
    int id;
    void analysis()
    {
        times.setTime(data.substr(0,15));
    }
    bool operator<(const Log& that)
    {
        if(times==that.times)
            return id<that.id;
        return times<that.times;
    }
};

vector<Log>logs;

string strstart,strend;
Time timestart,timeend;

vector<Log>output;

int main()
{
    ifstream file;
    file.open("./Linux_2k.log", ios::in);
    // file.open("/data/bigfiles/Linux_2k.log", ios::in);

    string line;
    while(getline(file,line))
    {
        Log temp;
        temp.data=line;
        temp.analysis();
        temp.id=logs.size();
        logs.push_back(temp);
    }
    file.close();

    getline(cin,strstart);
    getline(cin,strend);
    
    timestart.setTime(strstart);
    timeend.setTime(strend);

    if(timestart.err||timeend.err)
    {
        cout<<"error1"<<endl;
        return 0;
    }


    if(timestart>timeend)
    {
        cout<<"error2"<<endl;
        return 0;
    }

    
    for(int i=0;i<=logs.size();i++)
        if(logs[i].times>=timestart&&logs[i].times<=timeend)
            output.push_back(logs[i]);
    // sort(output.begin(),output.end());//居然不排序？
    for(int i=0;i<output.size();i++)
        if(i>=10)
        {
            cout<<output.size()-10<<' '<<"lines remain..."<<endl;
            break;
        }
        else
            cout<<output[i].data<<endl;

    return 0;
}