#pragma once

#include <iostream>
#include <vector>
#include <map>
#include <sstream>
#include <stack>
#include <cmath>
#include <cstdint>

using f64 = long double;
using i64 = int64_t;
using u64 = uint64_t;
using i32 = int32_t;
using u32 = uint32_t;

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
        union _TokenUnion {
            OperatorType op;
            f64 num;
            char var;
            ParenthesisType paren;
        } token;

        explicit Token(OperatorType op);

        explicit Token(f64 num);
        explicit Token(i64 num);
        explicit Token(u64 num);
        explicit Token(i32 num);
        explicit Token(u32 num);

        explicit Token(char var);
        explicit Token(ParenthesisType paren);
        Token(TokenType t, const _TokenUnion& u);
    };

    template<class T>
    struct Result {
        T value;
        bool success = true;
    };

    std::string to_string(const Token &t);
    Result<f64> execute_operation(f64 x, f64 y, OperatorType op);

    class Equation {
    public:
        Equation(std::initializer_list<Token> eq, bool isinfix = true);
        static int precedence(OperatorType op);
        std::string infix_str() const;
        std::string postfix_str() const;
        Result<f64> evaluate() const;
        Result<f64> evaluate(std::map<char, f64> vars) const;

    private:
        void dijkstra();

        std::vector<Token> equation;
        std::vector<Token> infix;
    };
}