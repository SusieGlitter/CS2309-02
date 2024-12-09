#ifndef SOLVER_H
#define SOLVER_H

#include <QWidget>
#include <algorithm>
#include <QDebug>

#include "frac.h"
#include "expression.h"

QString solve(int i,int j,int k,int l);

class Solver
{
public:
    Solver();
    QString getAns(int num0,int num1,int num2,int num3);
    void solveAll();
    int ableCnt;
private:
    QString ans[14][14][14][14];
};

#endif // SOLVER_H
