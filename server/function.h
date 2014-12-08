#ifndef FUNCTION_H
#define FUNCTION_H

#include <QString>

#define INF 100e100
#include <fstream>
#include <iostream>
#include <cmath>
#include <algorithm>
#include <vector>
#include <string>
#include <cstring>
#include <stack>
#include <cstdlib>

using namespace std;

QString toRPN(const QString&);

inline void relax(const char & c, stack<double> & var, bool & flag){
    if (c == 'e' || c == 's' || c == 'c' || c == 'm')
    switch (c)
    {
        case 'e':
            var.top() = exp(var.top());
        break;

        case 's':
            var.top() = sin(var.top());
        break;

        case 'c':
            var.top() = cos(var.top());
        break;

        case 'm':
            var.top() = -var.top();
        break;
        }
    else
    {
        double tmp2 = var.top(); var.pop();
        double tmp1 = var.top(); var.pop();

        switch (c)
        {
        case '+':
            var.push(tmp1 + tmp2);
        break;
        case '-':
            var.push(tmp1 - tmp2);
        break;
        case '*':
            var.push(tmp1 * tmp2);
        break;
        case '/':
            if (tmp2 == 0) flag = true;
            else
            var.push(tmp1 / tmp2);
        break;
        case '^':
            var.push( pow(tmp1, tmp2) );
        break;
        }
        }
}

inline double calc(const string& str, const double& coord){
    stack <double> var;
    int length = str.size();
    int i = 0;

    while (i < length){
        if ((str[i] <= '0' || str[i] >= '9') && str[i] != 'x' && str[i] != '('){
            bool  flag = false;
            relax(str[i], var, flag);
            if (flag == true) return INF;
            ++i;

        }
            else if (str[i] == 'x'){
                var.push(coord);
                ++i;
            }
            else {
                ++i;
                string tmp;
                while(str[i] != ')'){
                tmp += str[i];
                ++i;
            }
                double d_tmp = atof(tmp.c_str());
                var.push(d_tmp);
                ++i;
            }
        }
    return var.top();
}

#endif // FUNCTION_H
