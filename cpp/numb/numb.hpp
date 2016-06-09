#include <iostream>
#include <string>

using namespace std;

class Numb {
  public:
    double value;
    string signature;
    static string cache_in;
    static string cache_out;
    static string strategy;
    static int precision;
    Numb(double);
    Numb(double, string);
    static void setup(string, string, int, string);
    static void cache(string, string);
    static string query(string);
    static bool check(string, double);
    static Numb doublon(string, Numb, Numb, double);
    static Numb singleton(string, Numb, double);
    static void setup(string, Numb, double);

    friend Numb operator+ (const Numb &lhs, const Numb &rhs);
    friend Numb operator-(const Numb &lhs, const Numb &rhs);
    friend Numb operator*(const Numb &lhs, const Numb &rhs);
    friend Numb operator/(const Numb &lhs, const Numb &rhs);

    friend ostream &operator<<( ostream &output, const Numb &N );

    static Numb exp(Numb numb);
    static Numb ceil(const Numb &numb);
    static Numb fabs(const Numb &numb);
    static Numb floor(const Numb &numb);
    static Numb trunc(const Numb &numb);
    static Numb expm1(const Numb &numb);
    static Numb log(const Numb &numb);
    static Numb log1p(const Numb &numb);
    static Numb log10(const Numb &numb);
    static Numb pow(const Numb &numb);
    static Numb sqrt(const Numb &numb);
    static Numb acos(const Numb &numb);
    static Numb asin(const Numb &numb);
    static Numb atan(const Numb &numb);
    static Numb cos(const Numb &numb);
    static Numb sin(const Numb &numb);
    static Numb tan(const Numb &numb);
    static Numb acosh(const Numb &numb);
    static Numb asinh(const Numb &numb);
    static Numb atanh(const Numb &numb);
    static Numb cosh(const Numb &numb);
    static Numb sinh(const Numb &numb);
    static Numb tanh(const Numb &numb);
    static Numb erf(const Numb &numb);

};