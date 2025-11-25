#include <math.hpp>

math::Token::Token(const TokenType t, const _TokenUnion& u) : type(t), token(u) {}
math::Token::Token(const OperatorType op) : type(TokenType::OP) {
    token.op = op;
}

math::Token::Token(const f64 num) : type(TokenType::NUM) {
    token.num = num;
}
math::Token::Token(const i64 num) : type(TokenType::NUM) {
    token.num = static_cast<f64>(num);
}
math::Token::Token(const u64 num) : type(TokenType::NUM) {
    token.num = static_cast<f64>(num);
}
math::Token::Token(const i32 num) : type(TokenType::NUM) {
    token.num = static_cast<f64>(num);
}
math::Token::Token(const u32 num) : type(TokenType::NUM) {
    token.num = static_cast<f64>(num);
}

math::Token::Token(const char var) : type(TokenType::VAR) {
    token.var = var;
}
math::Token::Token(const ParenthesisType paren) : type(TokenType::PRTH) {
    token.paren = paren;
}

math::Equation::Equation(const std::initializer_list<Token> eq, const bool isinfix) {
    if (isinfix) {
        infix = eq;
        dijkstra();
    } else equation = eq;
}

int math::Equation::precedence(const OperatorType op) {
    if (op == OperatorType::EXP) return 3;
    if (op == OperatorType::MUL || op == OperatorType::DIV) return 2;
    return 1;
}

void math::Equation::dijkstra() {
    std::stack<Token> tmp;

    for (const auto&[type, token] : infix) {
        if (type == TokenType::VAR || type == TokenType::NUM) equation.emplace_back(Token(type, token));
        else if (type == TokenType::PRTH && token.paren == ParenthesisType::OPEN) tmp.push(Token(type, token));
        else if (type == TokenType::PRTH && token.paren == ParenthesisType::CLOSED) {
            Token tk = tmp.top();
            tmp.pop();
            while (!(tk.type == TokenType::PRTH && tk.token.paren == ParenthesisType::OPEN)) {
                equation.emplace_back(tk);

                tk = tmp.top();
                tmp.pop();
            }
        } else if (type == TokenType::OP) {
            while (!tmp.empty() && tmp.top().type == TokenType::OP) {
                if (precedence(tmp.top().token.op) >= precedence(token.op)) {
                    equation.emplace_back(tmp.top());
                    tmp.pop();
                } else break;
            }
            tmp.push(Token(type, token));
        }
    }

    while (!tmp.empty()) {
        equation.emplace_back(tmp.top());
        tmp.pop();
    }
}

std::string math::to_string(const Token& t) {
    switch (t.type) {
        case TokenType::VAR: return std::string(1, t.token.var);
        case TokenType::PRTH: return t.token.paren == ParenthesisType::OPEN ? "(" : ")";
        case TokenType::NUM: return std::to_string(t.token.num);

        case TokenType::OP: {
            switch (t.token.op) {
                case OperatorType::ADD: return "+";
                case OperatorType::SUB: return "-";
                case OperatorType::MUL: return "*";
                case OperatorType::DIV: return "/";
                case OperatorType::EXP: return "^";
            }
        }

        default: {
            std::cerr << "impossible enum again...";
            throw std::invalid_argument("non-existent operator rtpe");
        }
    }
}


std::string math::Equation::infix_str() const {
    std::stringstream ss;
    for (auto t : infix) {
        ss << to_string(t);
    }

    return std::move(ss).str();
}

std::string math::Equation::postfix_str() const {
    std::stringstream ss;
    for (auto t : equation) {
        ss << to_string(t) << " ";
    }

    return std::move(ss).str();
}

math::Result<f64> math::execute_operation(const f64 x, const f64 y, const OperatorType op) {
    switch (op) {
        case OperatorType::ADD: return {x+y};
        case OperatorType::SUB: return {x-y};
        case OperatorType::MUL: return {x*y};
        case OperatorType::DIV: {
            if (y == 0.0) return {0, false};
            return {x/y};
        }
        case OperatorType::EXP: return {std::pow(x, y)};
    }

    return {0, false};
}

math::Result<f64> math::Equation::evaluate() const {
    std::stack<f64> tmp;
    for (const auto&[type, token] : equation) {
        switch (type) {
            case TokenType::VAR: {
                throw std::invalid_argument("no variables provided for equation with variables");
            }

            case TokenType::NUM: {
                tmp.push(token.num);
                break;
            }

            case TokenType::OP: {
                const f64 y = tmp.top();
                tmp.pop();
                const f64 x = tmp.top();
                tmp.pop();

                auto [value, success] = execute_operation(x, y, token.op);
                if (!success) return {0, false};
                tmp.push(value);
                break;
            }

            default: {
                std::cerr << "impossible enum again...";
                throw std::invalid_argument("somehow managed to mess up a private vector...");
            }
        }
    }
    return {std::move(tmp).top()};
}

math::Result<f64> math::Equation::evaluate(std::map<char, f64> vars) const {
    std::stack<f64> tmp;
    for (const auto&[type, token] : equation) {
        switch (type) {
            case TokenType::VAR: {
                try {
                    tmp.push(vars[token.var]);
                } catch (const std::exception& e) {
                    std::cerr << e.what() << std::endl;
                    return {0, false};
                }
                break;
            }

            case TokenType::NUM: {
                tmp.push(token.num);
                break;
            }

            case TokenType::OP: {
                const f64 y = tmp.top();
                tmp.pop();
                const f64 x = tmp.top();
                tmp.pop();

                auto [value, success] = execute_operation(x, y, token.op);
                if (!success) return {0, false};
                tmp.push(value);
                break;
            }

            default: {
                std::cerr << "impossible enum again...";
                throw std::invalid_argument("somehow managed to mess up a private vector...");
            }
        }
    }
    return {std::move(tmp).top()};
}