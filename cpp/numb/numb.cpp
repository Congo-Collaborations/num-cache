#include "numb.hpp"
#include <fstream>
#include <cmath>
#include "sha256.hpp"
#include <vector>
#include <cstdlib>
#include <sstream>
#include <iomanip>
#include <iostream>

string Numb::cache_in;
string Numb::cache_out;
int Numb::precision;
string Numb::strategy;

void split(const string& s, char c, vector<string>& v)
{
   string::size_type i = 0;
   string::size_type j = s.find(c);

   while (j != string::npos) {
      v.push_back(s.substr(i, j-i));
      i = ++j;
      j = s.find(c, j);

      if (j == string::npos)
         v.push_back(s.substr(i, s.length()));
   }
}

Numb::Numb(double _value)
{
    stringstream ss (stringstream::in | stringstream::out);
    ss.setf( std::ios::fixed, std:: ios::floatfield );
    ss.precision(Numb::precision);
    ss << "asign" << std::setprecision(Numb::precision) << _value;
    cout << "hash = " << ss.str() << endl;
    signature = sha256(ss.str());
    cout << "signature = " << signature << endl;
    string queried = Numb::query(signature);
    if(queried.compare("") != 0){
        value = _value;
        stringstream ss (stringstream::in | stringstream::out);
        ss.setf( std::ios::fixed, std:: ios::floatfield );
        ss.precision(Numb::precision);
        ss << "asign|" << value;
        Numb::cache(signature, ss.str());
    }else{
        value = _value;
        stringstream ss (stringstream::in | stringstream::out);
        ss.setf( std::ios::fixed, std:: ios::floatfield );
        ss.precision(Numb::precision);
        ss << "asign|" << value;
        Numb::cache(signature, ss.str());
    }
}

Numb::Numb(double _value, string _signature)
{
    value = _value;
    signature = _signature;
    stringstream ss (stringstream::in | stringstream::out);
    ss.setf( std::ios::fixed, std:: ios::floatfield );
    ss.precision(Numb::precision);
    ss << "asign|" << value;
    Numb::cache(signature, ss.str());
}

void Numb::setup(string cache_out, string cache_in, int precision, string strategy)
{
    Numb::cache_in = cache_in;
    Numb::cache_out = cache_out;
    Numb::strategy = strategy;
    Numb::precision = precision;
    cout.setf( std::ios::fixed, std:: ios::floatfield );
    cout.precision(Numb::precision);
}

void Numb::cache(string signature, string content)
{
    
    const int dir_err = system(string("mkdir -p "+Numb::cache_out).c_str());
    if (-1 == dir_err)
    {
        cout << "ERROR -- Unable to create the cache directory." << endl;
    }
    vector<string> v;
    split(content, '|', v);
    ofstream cache_file(Numb::cache_out+"/"+signature+".cache");
    cache_file.setf( std::ios::fixed, std:: ios::floatfield );
    cache_file.precision(Numb::precision);
    if (cache_file.is_open())
    {
        cache_file << content << endl;
        cache_file.close();
        cout << "DEBUG -- " << v[0] << " operation was successfuly cached." << endl;
    }
    else cout << "ERROR -- Unable to open the output cache entry " << signature << " file." << endl;
}

string Numb::query(string signature)
{
    string content = "";
    if(Numb::cache_in.compare("") == 0){
        return content;
    }else{
        ifstream cache_file (Numb::cache_in+"/"+signature+".cache");
        cache_file.setf( std::ios::fixed, std:: ios::floatfield );
        cache_file.precision(Numb::precision);
        if(Numb::cache_in.compare("") == 0){
            return content;
        }
        if (cache_file.is_open())
        {
            string line;
            while ( getline (cache_file,line) )
            {
                content += line + "|";
            }
            cache_file.close();
        }
        else {
            cout << "ERROR -- Unable to open the input cache entry " << signature << " file." << endl;
        }
       return content;
    }
}

bool Numb::check(string cache, double current)
{
    stringstream ss (stringstream::in | stringstream::out);
    ss.setf( std::ios::fixed, std:: ios::floatfield );
    ss.precision(Numb::precision);
    cout.setf(ios::fixed, ios::floatfield);
    vector<string> v;
    split(cache, '|', v);
    cout << "V1: " << v[1] << endl;
    cout << "Current: " << current << endl;
    ss << current;
    if (v[1] != ss.str())
    {
        cout << "WARNING -- previously cached operation evaluation changed from [" << stod(v[1]) << "] to [" << current << "]." << endl;
        cout << "WARNING -- cached: " << cache << endl;
        return false;
    }else{
        cout << "DEBUG -- operator gave an identical evaluation that match the cache." << endl;
        return true;
    }
}

Numb Numb::doublon(string oper, Numb numb1, Numb numb2, double result)
{
    stringstream ss (stringstream::in | stringstream::out);
    ss.setf( std::ios::fixed, std:: ios::floatfield );
    ss.precision(Numb::precision);
    ss << oper << numb1.signature << numb2.signature;
    cout << "hash = " << ss.str() << endl;
    string signature = sha256(ss.str());
    cout << "signature = " << signature << endl;
    stringstream _ss (stringstream::in | stringstream::out);
    _ss.setf( std::ios::fixed, std:: ios::floatfield );
    _ss.precision(Numb::precision);
    _ss << oper << "|" << result << "|" << numb1.value << "|" << numb2.value;
    Numb::cache(signature, _ss.str());
    string queried = Numb::query(signature);
    if(queried.compare("") != 0){
        if(Numb::strategy.compare("load-cache") == 0){
            vector<string> v;
            split(queried, '|', v);
            cout << "INFO -- loading [" << stod(v[1]) << "] from cache entry [" << signature << "]." << endl;
            return Numb(stod(v[1]), signature);
        }else{
            if (Numb::check(queried, result)){
                return Numb(result, signature);
            }else{
                if(Numb::strategy.compare("ignore-cache") == 0){
                    return Numb(result, signature);
                }else if(Numb::strategy.compare("use-cache") == 0){
                    vector<string> v;
                    split(queried, '|', v);
                    cout << "INFO -- using [" << stod(v[1]) << "] from cache entry [" << signature << "]." << endl;
                    return Numb(stod(v[1]), signature);
                }else{
                    cout << "ERROR -- unknown cache strategy provided. Only accepts ['ignore-cache', 'use-cache']." << endl;
                    Numb(result, signature);
                }
            }
        }
    }else{
        return Numb(result, signature);
    }
}

Numb operator+ (const Numb &lhs, const Numb &rhs)
{
    double result = lhs.value + rhs.value;
    return Numb::doublon("add", lhs, rhs, result);
}

Numb operator- (const Numb &lhs, const Numb &rhs)
{
    double result = lhs.value - rhs.value;
    return Numb::doublon("sub", lhs, rhs, result);
}

Numb operator* (const Numb &lhs, const Numb &rhs)
{
    double result = lhs.value * rhs.value;
    return Numb::doublon("mul", lhs, rhs, result);
}

Numb operator/ (const Numb &lhs, const Numb &rhs)
{
    double result = lhs.value / rhs.value;
    return Numb::doublon("div", lhs, rhs, result);
}

ostream &operator<< ( ostream &output, const Numb &N )
{ 
    output << N.value;
    return output;            
}

Numb Numb::singleton(string oper, Numb numb, double result)
{
    stringstream ss (stringstream::in | stringstream::out);
    ss.setf( std::ios::fixed, std:: ios::floatfield );
    ss.precision(Numb::precision);
    ss << oper << numb.signature;
    cout << "hash = " << ss.str() << endl;
    string signature = sha256(ss.str());
    cout << "signature = " << signature << endl;
    stringstream _ss (stringstream::in | stringstream::out);
    _ss.setf( std::ios::fixed, std:: ios::floatfield );
    _ss.precision(Numb::precision);
    _ss << oper << "|" << result << "|" << numb.value;
    Numb::cache(signature, _ss.str());
    string queried = Numb::query(signature);
    if(queried.compare("") != 0){
        if(Numb::strategy.compare("load-cache") == 0){
            vector<string> v;
            split(queried, '|', v);
            cout << "INFO -- loading [" << stod(v[1]) << "] from cache entry [" << signature << "]." << endl;
            return Numb(stod(v[1]), signature);
        }else{
            if (Numb::check(queried, result)){
                return Numb(result, signature);
            }else{
                if(Numb::strategy.compare("ignore-cache") == 0){
                    return Numb(result, signature);
                }else if(Numb::strategy.compare("use-cache") == 0){
                    vector<string> v;
                    split(queried, '|', v);
                    cout << "INFO -- using [" << stod(v[1]) << "] from cache entry [" << signature << "]." << endl;
                    return Numb(stod(v[1]), signature);
                }else{
                    cout << "ERROR -- unknown cache strategy provided. Only accepts ['ignore-cache', 'use-cache']." << endl;
                    return Numb(result, signature);
                }
            }
        }
    }else{
        return Numb(result, signature);
    }
}

Numb Numb::exp(Numb numb){
    double result = std::exp(numb.value);
    return Numb::singleton("exp", numb, result);
}
Numb Numb::ceil(const Numb &numb){
    double result = std::ceil(numb.value);
    return Numb::singleton("ceil", numb, result);
}
Numb Numb::fabs(const Numb &numb){
    double result = std::fabs(numb.value);
    return Numb::singleton("fabs", numb, result);
}

Numb Numb::floor(const Numb &numb){
    double result = std::floor(numb.value);
    return Numb::singleton("floor", numb, result);
}

Numb Numb::trunc(const Numb &numb){
    double result = std::trunc(numb.value);
    return Numb::singleton("trunc", numb, result);
}
Numb Numb::expm1(const Numb &numb){
    double result = std::expm1(numb.value);
    return Numb::singleton("expm1", numb, result);
}
Numb Numb::log(const Numb &numb){
    double result = std::log(numb.value);
    return Numb::singleton("log", numb, result);
}
Numb Numb::log1p(const Numb &numb){
    double result = std::log1p(numb.value);
    return Numb::singleton("log1p", numb, result);
}
Numb Numb::log10(const Numb &numb){
    double result = std::log10(numb.value);
    return Numb::singleton("log10", numb, result);
}

Numb Numb::sqrt(const Numb &numb){
    double result = std::sqrt(numb.value);
    return Numb::singleton("sqrt", numb, result);
}
Numb Numb::acos(const Numb &numb){
    double result = std::acos(numb.value);
    return Numb::singleton("acos", numb, result);
}
Numb Numb::asin(const Numb &numb){
    double result = std::asin(numb.value);
    return Numb::singleton("asin", numb, result);
}
Numb Numb::atan(const Numb &numb){
    double result = std::atan(numb.value);
    return Numb::singleton("atan", numb, result);
}

Numb Numb::cos(const Numb &numb){
    double result = std::cos(numb.value);
    return Numb::singleton("cos", numb, result);
}
Numb Numb::sin(const Numb &numb){
    double result = std::sin(numb.value);
    return Numb::singleton("sin", numb, result);
}
Numb Numb::tan(const Numb &numb){
    double result = std::tan(numb.value);
    return Numb::singleton("tan", numb, result);
}

Numb Numb::acosh(const Numb &numb){
    double result = std::acosh(numb.value);
    return Numb::singleton("acosh", numb, result);
}
Numb Numb::asinh(const Numb &numb){
    double result = std::asinh(numb.value);
    return Numb::singleton("asinh", numb, result);
}
Numb Numb::atanh(const Numb &numb){
    double result = std::atanh(numb.value);
    return Numb::singleton("atanh", numb, result);
}
Numb Numb::cosh(const Numb &numb){
    double result = std::cosh(numb.value);
    return Numb::singleton("cosh", numb, result);
}
Numb Numb::sinh(const Numb &numb){
    double result = std::sinh(numb.value);
    return Numb::singleton("sinh", numb, result);
}
Numb Numb::tanh(const Numb &numb){
    double result = std::tanh(numb.value);
    return Numb::singleton("tanh", numb, result);
}
Numb Numb::erf(const Numb &numb){
    double result = std::erf(numb.value);
    return Numb::singleton("erf", numb, result);
}