#ifndef FRAC_H
#define FRAC_H
int gcd(int a,int b);
class Frac
{
public:
    Frac();
    Frac(const Frac &that);
    Frac(int num,int den=1);
    Frac operator=(const Frac &that);
    Frac operator+(const Frac &that);
    Frac operator-();
    Frac operator-(const Frac &that);
    Frac operator*(const Frac &that);
    Frac operator/(const Frac &that);
    bool operator==(const Frac &that);
    bool operator!=(const Frac &that);
    bool operator<(const Frac &that);
    bool operator<=(const Frac &that);
    bool operator>(const Frac &that);
    bool operator>=(const Frac &that);
    int num;
    int den;
private:
    void setFrac(int num,int den);
    void simplify();
};

#endif // FRAC_H
