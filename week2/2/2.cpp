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

class File
{
public:
    string filename;
    int filesize;
    vector<File*> subFile;
};

string codes;
vector<string> code;

File root;

void insfile(string filepath,int filesize=-1)
{
    File* now=&root;
    while(true)
    {
        bool found=false;
        for(int i=0;i<now->subFile.size();i++)
        {
            int len=now->subFile[i]->filename.length();
            File *son=now->subFile[i];
            if(filepath[len]=='\\'&&filepath.substr(0,len)==son->filename)
            {
                found=true;
                now=son;
                break;
            }
        }
        if(!found)
            break;
    }

    int nextdash=now->filename.length()+1;
    while(nextdash<filepath.length()&&filepath[nextdash]!='\\')
        nextdash++;

    //路径结束了
    if(nextdash==filepath.length())
    {
        File* newfile=new File;
        newfile->filename=filepath;
        newfile->filesize=filesize;
        now->subFile.push_back(newfile); 
        return;
    }

    //创建部分路径
    File* newfile=new File;
    newfile->filename=filepath.substr(0,nextdash);
    newfile->filesize=-1;
    now->subFile.push_back(newfile); 
    insfile(filepath,filesize);
}

File *findFile(File *now,string path)
{
    if(now->filename==path)
        return now;
    for(int i=0;i<now->subFile.size();i++)
    {
        File *ret=findFile(now->subFile[i],path);
        if(ret!=nullptr)
            return ret;
    }
    return nullptr;
}

string labelFile(string sourceName,int &id,vector<string> &filename)
{
    
    string subname1;
    string subname2;
    string subname3;
    string copyname;

    for(int i=sourceName.length()-1;i>=0;i--)
    {
        if(sourceName[i]=='.')
        {
            subname1=sourceName.substr(0,i)+"_";
            subname3=sourceName.substr(i,sourceName.length()-1);
            break;
        }
        if(i==0||sourceName[i]=='\\')
        {
            subname1=sourceName+"_";
            break;
        }
    }
    while(true)
    {
        copyname=subname1+to_string(id)+subname3;
        bool unique=true;
        for(int i=0;i<filename.size();i++)
        {
            if(filename[i]==copyname)
            {
                unique=false;
                id++;
                break;
            }
        }
        if(unique)
        {
            id++;
            return copyname;
        }
    }
}

void sameNameFile(File *s,int sindex,File *t,int tindex)
{
    vector<string> filename;
    int id=1;
    for(int i=0;i<s->subFile.size();i++)
        filename.push_back(s->subFile[i]->filename.substr(s->filename.length()));
    for(int i=0;i<t->subFile.size();i++)
        filename.push_back(t->subFile[i]->filename.substr(t->filename.length()));

    string renamed2=labelFile(t->subFile[tindex]->filename.substr(t->filename.length()),id,filename);
    string renamed1=labelFile(s->subFile[sindex]->filename.substr(s->filename.length()),id,filename);

    s->subFile[sindex]->filename=s->filename+renamed1;
    t->subFile[tindex]->filename=t->filename+renamed2;
}

int maxsize=0;
string maxfile;

int dfs(File* now)
{
    int ret=now->filesize;
    if(ret>maxsize||ret==maxsize&&now->filename<maxfile)
    {
        maxsize=ret;
        maxfile=now->filename;
    }
    if(ret<0)ret=0;
    for(int i=0;i<now->subFile.size();i++)
    {
        ret+=dfs(now->subFile[i]);
    }
    return ret;
}

void movFile(string spath,string tpath,bool flag=true)
{
    // 判断目标目录是否为子目录
    if(tpath.substr(0,spath.length())==spath)
    {
        cout<<"error"<<endl;
        return;
    }

    // 判断两个目录是否存在
    File *s=findFile(&root,spath);
    File *t=findFile(&root,tpath);

    int lastdash=spath.length()-1;
    while(lastdash>=0&&spath[lastdash]!='\\')
        lastdash--;
    File *sfat=findFile(&root,spath.substr(0,lastdash));

    if(s==nullptr||t==nullptr)
    {
        cout<<"error"<<endl;
        return;
    }

    // 判断目标目录是否存在同名文件 移动文件
    for(int i=0;i<s->subFile.size();i++)
    {
        if(s->subFile[i]->filesize==-1)continue;
        for(int j=0;j<t->subFile.size();j++)
        {
            if(t->subFile[j]->filesize==-1)continue;
            if(s->subFile[i]->filename.substr(s->filename.length())==t->subFile[j]->filename.substr(t->filename.length()))
                sameNameFile(s,i,t,j);
        }
    }
    for(int i=0;i<s->subFile.size();i++)
        if(s->subFile[i]->filesize!=-1)
        {
            s->subFile[i]->filename=tpath+s->subFile[i]->filename.substr(spath.length());
            t->subFile.push_back(s->subFile[i]);
        }

    //  判断目标目录是否存在同名文件夹 递归移动文件夹
    for(int i=0;i<s->subFile.size();i++)
    {
        if(s->subFile[i]->filesize!=-1)continue;
        bool unique=true;
        for(int j=0;j<t->subFile.size();j++)
        {
            if(t->subFile[j]->filesize!=-1)continue;
            if(s->subFile[i]->filename.substr(s->filename.length())==t->subFile[j]->filename.substr(t->filename.length()))
            {
                unique=false;
                movFile(s->subFile[i]->filename,t->subFile[j]->filename,false);
                break;
            }
        }
        if(unique)
        {
            File *temp=new File;
            temp->filename=tpath+s->subFile[i]->filename.substr(spath.length());
            temp->filesize=-1;
            t->subFile.push_back(temp);
            movFile(s->subFile[i]->filename,temp->filename,false);
        }
    }

    // 删除源目录
    if(flag)
    {
        for(int i=0;i<sfat->subFile.size();i++)
        {
            if(sfat->subFile[i]->filename==spath)
            {
                sfat->subFile.erase(sfat->subFile.begin()+i);
                break;
            }
        }

        cout<<dfs(t)<<endl<<maxfile<<endl;
    }

}


int main()
{

    while(true)
    {
        getline(cin,codes);
        stringSplit(codes,' ',code);
        if(code.size()==1)
            insfile(code[0]);
        else if(code[1][0]>='0'&&code[1][0]<='9')
            insfile(code[0],stoi(code[1]));
        else
        {
            movFile(code[0],code[1]);
            break;
        }
    }
    // cout<<dfs(&root)<<endl<<maxfile<<endl;
    return 0;
}