#include "frac.h"
int gcd(int a, int b)
{
    return b==0?a:gcd(b,a%b);
}
Frac::Frac()
{
    this->num=0;
    this->den=1;
}
Frac::Frac(const Frac &that)
{
    this->num=that.num;
    this->den=that.den;
}
Frac::Frac(int num,int den)
{
    this->num=num;
    this->den=den;
}
Frac Frac::operator=(const Frac &that)
{
    if(this==&that)
        return *this;
    this->num=that.num;
    this->den=that.den;
    return *this;
}
Frac Frac::operator+(const Frac &that)
{
    Frac temp;
    temp.num=this->num*that.den+this->den*that.num;
    temp.den=this->den*that.den;
    temp.simplify();
    return temp;
}
Frac Frac::operator-()
{
    Frac temp(*this);
    temp.num=-temp.num;
    return temp;
}
Frac Frac::operator-(const Frac &that)
{
    Frac temp;
    temp.num=this->num*that.den-this->den*that.num;
    temp.den=this->den*that.den;
    temp.simplify();
    return temp;
}
Frac Frac::operator*(const Frac &that)
{
    Frac temp;
    temp.num=this->num*that.num;
    temp.den=this->den*that.den;
    temp.simplify();
    return temp;
}
Frac Frac::operator/(const Frac &that)
{
    Frac temp;
    temp.num=this->num*that.den;
    temp.den=this->den*that.num;
    temp.simplify();
    return temp;
}
bool Frac::operator==(const Frac &that)
{
    return this->num*that.den==this->den*that.num;
}
bool Frac::operator!=(const Frac &that)
{
    return this->num*that.den!=this->den*that.num;
}
bool Frac::operator<(const Frac &that)
{
    return this->num*that.den<this->den*that.num;
}
bool Frac::operator<=(const Frac &that)
{
    return this->num*that.den<=this->den*that.num;
}
bool Frac::operator>(const Frac &that)
{
    return this->num*that.den>this->den*that.num;
}
bool Frac::operator>=(const Frac &that)
{
    return this->num*that.den>=this->den*that.num;
}
void Frac::setFrac(int num,int den)
{
    this->num=num;
    this->den=den;
}
void Frac::simplify()
{
    int g;
    if(this->num==0)
    {
        this->den=1;
        return;
    }
    if(this->den<0)
    {
        this->den=-this->den;
        this->num=-this->num;
    }
    if(this->num<0)
        g=gcd(-this->num,this->den);
    else
        g=gcd(this->num,this->den);
    this->num/=g;
    this->den/=g;
}
