#include<iostream>
#include<cstring>
#include<sstream>
#include<fstream>
#include<string>
#include<vector>
#include<algorithm>
const double eps=1e-6;
class Matrix
{
public:
    int row,col;
    int rank;
    std::vector<std::vector<double>> matrix;
    void swprol(int i,int j)
    {
        std::swap(matrix[i],matrix[j]);
    }
    void addrol(int i,int j,double k)
    {
        for(int l=0;l<col;l++)
            matrix[j][l]+=k*matrix[i][l];
    }
    void mulrol(int i,double k)
    {
        for(int l=0;l<col;l++)
            matrix[i][l]*=k;
    }

    void simplify()
    {
        int now=0;//主元应当在的行
        for(int j=0;j<col;j++)
        {
            //第一步：寻找主元行
            bool flag=false;
            for(int i=now;i<row;i++)
                if(std::abs(matrix[i][j])>eps)
                {
                    swprol(i,now);
                    flag=true;
                    break;
                }
            if(!flag)continue;
            //第二步：单位化主元
            mulrol(now,1/matrix[now][j]);
            //第三步：主元列消元
            for(int i=0;i<row;i++)
                if(i!=now)
                    addrol(now,i,-matrix[i][j]);
            
            now++;
        }
        rank=now;
    }
    void read(std::string s)
    {
        //((2 4 1 3)(-1 -2 1)(0 0 2 2)(3 6 2 5))
        row=-1;
        for(int i=0;i<s.size();i++)
            if(s[i]=='(')
                row++;
        if(row==0)
        {
            col=0;
            return;
        }
        int p=0;
        while(s[p]!='(')
            p++;
        p++;
        std::vector<std::vector<double>> temp;
        for(int i=0;i<row;i++)
        {
            std::vector<double> temprow;
            int num,cnt=0;
            std::stringstream ss;
            int start=s.find('(',p);
            int end=s.find(')',start);
            p=end+1;
            ss<<s.substr(start+1,end-p-1);
            while(ss>>num)
            {
                temprow.push_back(num);
                cnt++;
            }
            col=std::max(col,cnt);
            temp.push_back(temprow);
        }
        for(int i=0;i<row;i++)
        {
            std::vector<double> temprow=temp[i];
            while(temprow.size()<col)
                temprow.push_back(0);
            matrix.push_back(temprow);
        }
    }
    bool operator==(const Matrix& that)
    {
        // 真正的等价
        // return rank==that.rank;

        //题目所说的“等价”
        int maxrow=std::max(row,that.row);
        int maxcol=std::max(col,that.col);
        for(int i=0;i<maxrow;i++)
            for(int j=0;j<maxcol;j++)
            {
                if(i>=row||j>=col)
                {
                    if(std::abs(that.matrix[i][j])>eps)
                        return false;
                    continue;
                }
                if(i>=that.row||j>=that.col)
                {
                    if(std::abs(matrix[i][j])>eps)
                        return false;
                    continue;
                }
                if(std::abs(matrix[i][j]-that.matrix[i][j])>eps)
                    return false;

            }
        return true;
    }
    void print()
    {
        for(int i=0;i<row;i++)
        {
            for(int j=0;j<col;j++)
                std::cout<<matrix[i][j]<<" ";
            std::cout<<std::endl;
        }
        std::cout<<std::endl;
    }
};
std::string sa,sb;
Matrix A,B;
int main()
{
    getline(std::cin,sa);
    getline(std::cin,sb);
    A.read(sa);
    B.read(sb);
    A.simplify();
    B.simplify();
    std::cout<<((A==B)?"Yes":"No")<<std::endl;
}