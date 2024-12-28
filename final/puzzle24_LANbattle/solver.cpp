#include "solver.h"

QString solve(int num0,int num1,int num2,int num3)
{
    int nums[4]={num0,num1,num2,num3};
    static QString formatStrings[11]={
        "%1%2%3%4%5%6%7",
        "(%1%2%3)%4%5%6%7",
        "%1%2(%3%4%5)%6%7",
        "%1%2%3%4(%5%6%7)",
        "(%1%2%3%4%5)%6%7",
        "%1%2(%3%4%5%6%7)",
        "(%1%2%3)%4(%5%6%7)",
        "((%1%2%3)%4%5)%6%7",
        "(%1%2(%3%4%5))%6%7",
        "%1%2((%3%4%5)%6%7)",
        "%1%2(%3%4(%5%6%7))",
    };
    static QChar ops[4]={'+','-','*','/'};
    for(int i=0;i<11;i++)
    {
        for(int j=0;j<64;j++)
        {
            int seq[4]={0,1,2,3};
            do
            {
                // qDebug()<<QDateTime::currentDateTime().toMSecsSinceEpoch()<<"a";
                QString expString=formatStrings[i].arg(nums[seq[0]]).arg(ops[j/16]).arg(nums[seq[1]]).arg(ops[j/4%4]).arg(nums[seq[2]]).arg(ops[j%4]).arg(nums[seq[3]]);
                Expression exp(expString);

                exp.calculate();

                Frac res=exp.res;
                ExpressionError err=exp.err;
                if(err==noError&&res==Frac(24))
                    return expString;
                // qDebug()<<QDateTime::currentDateTime().toMSecsSinceEpoch()<<"b";
            }
            while(std::next_permutation(seq,seq+4));
            // qDebug()<<QDateTime::currentDateTime().toMSecsSinceEpoch()<<"roundround"<<j;
        }

        // qDebug()<<QDateTime::currentDateTime().toMSecsSinceEpoch()<<"round"<<i;
    }
    return "No solution";
}

Solver::Solver() {}

QString Solver::getAns(int num0,int num1,int num2,int num3)
{
    int nums[4]={num0,num1,num2,num3};
    std::sort(nums,nums+4);
    return ans[nums[0]][nums[1]][nums[2]][nums[3]];
}

void Solver::solveAll()
{
    ableCnt=0;
    for(int i=1;i<=13;i++)
        for(int j=i;j<=13;j++)
            for(int k=j;k<=13;k++)
                for(int l=k;l<=13;l++)
                    if((ans[i][j][k][l]=solve(i,j,k,l))!="No solution")
                        // ableCnt++,qDebug()<<ableCnt<<' '<<i<<' '<<j<<' '<<k<<' '<<l<<' '<<ans[i][j][k][l];
                        ableCnt++;
}
