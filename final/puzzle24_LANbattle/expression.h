#ifndef EXPRESSION_H
#define EXPRESSION_H

#include <QWidget>
#include <stack>
#include <QRegularExpression>
#include "frac.h"

enum ExpressionError{
    noError,// 没有错误
    operatorError,// 表达式形式错误
    dividedByZeroError,// 除以零错误
    numberTooMuchError,// 使用超过四个数字
    numberTooLessError,// 使用少于四个数字
    numberOutOfRangeError,// 使用数字不是规定数字
};
QString errorText(ExpressionError err);

class Expression
{
public:
    Expression();
    Expression(QString s);
    Expression(const Expression &that);
    void setExpression(const QString s);
    void filter();
    void calculate();
    Frac res;
    ExpressionError err;
    int usedNums[4];
    int usedNumCnt;
private:
    QString s;
    std::stack<Frac>numStack;
    std::stack<QChar>opStack;
    void stackTopCalcAux();
};

#endif // EXPRESSION_H
