#include "function.h"

using namespace std;

int priority(const char & c){
    if (c == '(' || c == ')') return 0;
    else if (c == '+' || c == '-') return 1;
    else if (c == '*' || c == '/') return 2;
    else if (c == '^') return 3;
    else return 4;
}

bool may_unary(const char & c){
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '^' || c == '(';
}

bool is_fync(const string & str, const int & i){
    return str[i] == '+' || str[i] == '-' || str[i] == '*' || str[i] == '/' || str[i] == '^'
                    || (str[i] == 's' && i + 3 < str.size() && str[i + 1] == 'i' && str[i + 2] == 'n' && str[i + 3] == '(')
                    || (str[i] == 'c' && i + 3 < str.size() && str[i + 1] == 'o' && str[i + 2] == 's' && str[i + 3] == '(')
                    || (str[i] == 'e' && i + 3 < str.size() && str[i + 1] == 'x' && str[i + 2] == 'p' && str[i + 3] == '(');
}

bool is_binary(const char & c){
    return c == '+' ||c == '-' || c == '*' || c == '/' || c == '^';
}


QString toRPN(const QString& qstr){
    string str = qstr.toUtf8().constData();

    QString sorry = "IDI NAHUI";
    string ans;
    stack <char> op;
    int count_var = 0;
    int length = str.size();
    if (length == 0) return sorry;

    int i = 0;

    while (i < length){
        if (str[i] == '('){
                if (i + 1 == length || str[i + 1] == ')') return sorry;
                op.push(str[i]);
                ++i;
            }
            else if (str[i] == ')'){
                while (!op.empty() && op.top() != '('){
                    int kill_var = is_binary(op.top());
                    if (kill_var == 1 && count_var < 2) return sorry;
                    else if (count_var < 1) return sorry;

                    ans += op.top();
                    op.pop();

                    count_var -= kill_var;
                }
                if (op.empty()) return sorry;
                op.pop();
                ++i;
            }
            else if (str[i] >= '0' && str[i] <= '9'){
                bool flag_p = false;
                string tmp = "";
                while(((str[i] == '.') || (str[i] >= '0' && str[i] <= '9')) && (i < length)){
                    if (str[i] == '.'){
                        if (flag_p) return sorry;
                    else flag_p = true;
                    }

                    tmp += str[i];
                    ++i;
                }
                ans += '(' + tmp + ')';
                count_var++;
            }
            else if (str[i] == 'x'){
                ans += str[i];
                ++i;
                count_var++;
            }
            else if (is_fync(str, i)){
                if (str[i] == '-' && ( i == 0 || may_unary(str[i - 1]))){
                    str[i] = 'm';
                }
                if (priority(str[i]) != 4){
                while (!op.empty() && priority(op.top()) >= priority(str[i])){
                    int kill_var = is_binary(op.top());
                    if (kill_var == 1 && count_var < 2) return sorry;
                    else if (count_var < 1) return sorry;

                    ans += op.top();
                    op.pop();
                    count_var -= kill_var;
                    }
                }
                op.push(str[i]);
                if (str[i] == 's' || str[i] == 'e' || str[i] == 'c'){
                    i += 3;
                }
                else i++;
            }
            else  return sorry;

    }


    if (ans.size() == 0) return sorry;

    while (!op.empty()){
        if (op.top() == '(') return sorry;

        int kill_var = is_binary(op.top());
                    if (kill_var == 1 && count_var < 2) return sorry;
                    else if (count_var < 1) return sorry;

        ans += op.top();
        op.pop();
        count_var -= kill_var;

    }
    if (count_var != 1) return sorry;
    return QString(ans.c_str());
}

