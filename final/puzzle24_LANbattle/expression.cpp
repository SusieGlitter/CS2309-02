#include "expression.h"

int opPri(QChar op)
{
    switch (op.unicode()) {
    case '+':
        return 1;
        break;
    case '-':
        return 1;
        break;
    case '*':
        return 2;
        break;
    case '/':
        return 2;
        break;
    default:
        break;
    }
    return 0;
}

QString errorText(ExpressionError err)
{
    switch (err) {
    case noError:
        return "noError";
        break;
    case operatorError:
        return "operatorError";
        break;
    case dividedByZeroError:
        return "dividedByZeroError";
        break;
    case numberTooMuchError:
        return "numberTooMuchError";
        break;
    case numberTooLessError:
        return "numberTooLessError";
        break;
    case numberOutOfRangeError:
        return "numberOutOfRangeError";
        break;
    default:
        break;
    }
    return "undefinedError";
}

Expression::Expression()
{
    this->s="";
}
Expression::Expression(QString s)
{
    this->s=s;
    filter();
}
Expression::Expression(const Expression &that)
{
    this->s=that.s;
    filter();
}
void Expression::setExpression(const QString s)
{
    this->s=s;
    filter();
}
void Expression::filter()
{
    s.remove(QRegularExpression("[^AJQKajqk0-9\\(\\)\\+\\-\\*\\/]"));
    s.replace(QRegularExpression("[Aa]"),"1");
    s.replace(QRegularExpression("[Jj]"),"11");
    s.replace(QRegularExpression("[Qq]"),"12");
    s.replace(QRegularExpression("[Kk]"),"13");
}
void Expression::calculate()
{
    bool lastIsNum=false;
    bool nowIsNum=false;

    for(auto c:s)
    {
        nowIsNum=(c!='('&&c!=')'&&c!='+'&&c!='-'&&c!='*'&&c!='/');
        if(nowIsNum)
        {
            if(lastIsNum==true)
            {
                Frac temp=numStack.top();
                numStack.pop();
                temp=temp*Frac(10)+Frac(c.digitValue());
                numStack.push(temp);
            }
            else
            {
                numStack.push(Frac(c.digitValue()));
            }
        }

        lastIsNum=nowIsNum;
    }
    usedNumCnt=0;
    while(numStack.empty()==false)
    {
        if(usedNumCnt<4)
            usedNums[usedNumCnt]=numStack.top().num;
        numStack.pop();
        usedNumCnt++;
    }
    lastIsNum=false;
    nowIsNum=false;
    err=noError;
    for(auto c:"("+s+")")
    {
        nowIsNum=(c!='('&&c!=')'&&c!='+'&&c!='-'&&c!='*'&&c!='/');
        if(nowIsNum)
        {
            if(lastIsNum==true)
            {
                Frac temp=numStack.top();
                numStack.pop();
                temp=temp*Frac(10)+Frac(c.digitValue());
                numStack.push(temp);
            }
            else
            {
                numStack.push(c.digitValue());
            }

        }
        else
        {
            if(lastIsNum==false&&(c=='+'||c=='-')&&(opStack.empty()||opStack.top()=='('))
                numStack.push(Frac(0));
            if(c=='(')
                opStack.push(c);
            else if(c=='+'||c=='-'||c=='*'||c=='/')
            {
                while(opStack.empty()==false&&opStack.top()!='('&&opPri(c)<=opPri(opStack.top()))
                    stackTopCalcAux();
                opStack.push(c);
            }
            else if(c==')')
            {
                while(opStack.top()!='(')
                    stackTopCalcAux();
                opStack.pop();
                nowIsNum=true;
            }

        }

        lastIsNum=nowIsNum;
    }
    if(numStack.empty())
        err=operatorError;
    res=(err==noError?numStack.top():Frac(0,0));
}
void Expression::stackTopCalcAux()
{
    Frac num1,num2,num3;
    QChar op;
    if(numStack.size()<2)
    {
        while(numStack.empty()==false)numStack.pop();
        while(opStack.empty()==false)opStack.pop();
        err=operatorError;
        return;
    }
    num2=numStack.top();
    numStack.pop();
    num1=numStack.top();
    numStack.pop();
    op=opStack.top();
    opStack.pop();
    switch (op.unicode()) {
    case '+':
        num3=num1+num2;
        break;
    case '-':
        num3=num1-num2;
        break;
    case '*':
        num3=num1*num2;
        break;
    case '/':
        if(num2==Frac(0))
        {
            err=dividedByZeroError;
            num3=Frac(0);
        }
        else
        {
            num3=num1/num2;
        }
        break;
    default:
        err=operatorError;
        break;
    }
    numStack.push(num3);
}
