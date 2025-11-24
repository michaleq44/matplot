#pragma once

#include <iostream>
#include <vector>
#include <map>
#include <sstream>
#include <stack>
#include <cmath>

using f64 = long double;

namespace math {
    enum class TokenType {
        OP,
        NUM,
        VAR,
        PRTH
    };

    enum class OperatorType {
        ADD = '+', SUB = '-', MUL = '*', DIV = '/', EXP = '^'
    };

    enum class ParenthesisType {
        OPEN = '(', CLOSED = ')'
    };

    struct Token {
        TokenType type;
        union {
            OperatorType op;
            f64 num;
            char var;
            ParenthesisType paren;
        } token;
    };

    std::string to_string(const Token &t);

    class Equation {
    public:
        Equation(std::initializer_list<Token> eq, bool isinfix = true);
        static int precedence(OperatorType op);
        std::string infix_str() const;
        std::string postfix_str() const;

    private:
        void dijkstra();
        f64 evaluate(std::map<char, f64> vars);

        std::vector<Token> equation;
        std::vector<Token> infix;
    };
}